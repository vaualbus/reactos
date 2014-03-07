/*
 * Copyright 2006-2007 Jacek Caban for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "precomp.h"

#include <winreg.h>
#include <urlmon.h>
#include <shlwapi.h>

typedef struct {
    IInternetProtocol     IInternetProtocol_iface;
    IInternetProtocolInfo IInternetProtocolInfo_iface;

    LONG ref;

    ULONG offset;
    struct chmFile *chm_file;
    struct chmUnitInfo chm_object;
} ITSProtocol;

static inline ITSProtocol *impl_from_IInternetProtocol(IInternetProtocol *iface)
{
    return CONTAINING_RECORD(iface, ITSProtocol, IInternetProtocol_iface);
}

static inline ITSProtocol *impl_from_IInternetProtocolInfo(IInternetProtocolInfo *iface)
{
    return CONTAINING_RECORD(iface, ITSProtocol, IInternetProtocolInfo_iface);
}

static void release_chm(ITSProtocol *This)
{
    if(This->chm_file) {
        chm_close(This->chm_file);
        This->chm_file = NULL;
    }
    This->offset = 0;
}

static HRESULT WINAPI ITSProtocol_QueryInterface(IInternetProtocol *iface, REFIID riid, void **ppv)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);

    *ppv = NULL;
    if(IsEqualGUID(&IID_IUnknown, riid)) {
        TRACE("(%p)->(IID_IUnknown %p)\n", This, ppv);
        *ppv = &This->IInternetProtocol_iface;
    }else if(IsEqualGUID(&IID_IInternetProtocolRoot, riid)) {
        TRACE("(%p)->(IID_IInternetProtocolRoot %p)\n", This, ppv);
        *ppv = &This->IInternetProtocol_iface;
    }else if(IsEqualGUID(&IID_IInternetProtocol, riid)) {
        TRACE("(%p)->(IID_IInternetProtocol %p)\n", This, ppv);
        *ppv = &This->IInternetProtocol_iface;
    }else if(IsEqualGUID(&IID_IInternetProtocolInfo, riid)) {
        TRACE("(%p)->(IID_IInternetProtocolInfo %p)\n", This, ppv);
        *ppv = &This->IInternetProtocolInfo_iface;
    }

    if(*ppv) {
        IInternetProtocol_AddRef(iface);
        return S_OK;
    }

    WARN("not supported interface %s\n", debugstr_guid(riid));
    return E_NOINTERFACE;
}

static ULONG WINAPI ITSProtocol_AddRef(IInternetProtocol *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    LONG ref = InterlockedIncrement(&This->ref);
    TRACE("(%p) ref=%d\n", This, ref);
    return ref;
}

static ULONG WINAPI ITSProtocol_Release(IInternetProtocol *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    LONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p) ref=%d\n", This, ref);

    if(!ref) {
        release_chm(This);
        HeapFree(GetProcessHeap(), 0, This);

        ITSS_UnlockModule();
    }

    return ref;
}

static LPCWSTR skip_schema(LPCWSTR url)
{
    static const WCHAR its_schema[] = {'i','t','s',':'};
    static const WCHAR msits_schema[] = {'m','s','-','i','t','s',':'};
    static const WCHAR mk_schema[] = {'m','k',':','@','M','S','I','T','S','t','o','r','e',':'};

    if(!strncmpiW(its_schema, url, sizeof(its_schema)/sizeof(WCHAR)))
        return url+sizeof(its_schema)/sizeof(WCHAR);
    if(!strncmpiW(msits_schema, url, sizeof(msits_schema)/sizeof(WCHAR)))
        return url+sizeof(msits_schema)/sizeof(WCHAR);
    if(!strncmpiW(mk_schema, url, sizeof(mk_schema)/sizeof(WCHAR)))
        return url+sizeof(mk_schema)/sizeof(WCHAR);

    return NULL;
}

/* Adopted from urlmon */
static void remove_dot_segments(WCHAR *path) {
    const WCHAR *in = path;
    WCHAR *out = path;

    while(1) {
        /* Move the first path segment in the input buffer to the end of
         * the output buffer, and any subsequent characters up to, including
         * the next "/" character (if any) or the end of the input buffer.
         */
        while(*in != '/') {
            if(!(*out++ = *in++))
                return;
        }

        *out++ = *in++;

        while(*in) {
            if(*in != '.')
                break;

            /* Handle ending "/." */
            if(!in[1]) {
                ++in;
                break;
            }

            /* Handle "/./" */
            if(in[1] == '/') {
                in += 2;
                continue;
            }

            /* If we don't have "/../" or ending "/.." */
            if(in[1] != '.' || (in[2] && in[2] != '/'))
                break;

            in += *in ? 3 : 2;

            /* Find the slash preceding out pointer and move out pointer to it */
            if(out > path+1 && *--out == '/')
                --out;
            while(out > path && *(--out) != '/');
            if(*out == '/')
                ++out;
        }
    }
}

static HRESULT report_result(IInternetProtocolSink *sink, HRESULT hres)
{
    IInternetProtocolSink_ReportResult(sink, hres, 0, NULL);
    return hres;
}

static HRESULT WINAPI ITSProtocol_Start(IInternetProtocol *iface, LPCWSTR szUrl,
        IInternetProtocolSink *pOIProtSink, IInternetBindInfo *pOIBindInfo,
        DWORD grfPI, HANDLE_PTR dwReserved)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    BINDINFO bindinfo;
    DWORD bindf = 0, len;
    LPWSTR file_name, mime, object_name, p;
    LPCWSTR ptr;
    struct chmFile *chm_file;
    struct chmUnitInfo chm_object;
    int res;
    HRESULT hres;

    static const WCHAR separator[] = {':',':',0};

    TRACE("(%p)->(%s %p %p %08x %lx)\n", This, debugstr_w(szUrl), pOIProtSink,
            pOIBindInfo, grfPI, dwReserved);

    ptr = skip_schema(szUrl);
    if(!ptr)
        return INET_E_USE_DEFAULT_PROTOCOLHANDLER;

    memset(&bindinfo, 0, sizeof(bindinfo));
    bindinfo.cbSize = sizeof(BINDINFO);
    hres = IInternetBindInfo_GetBindInfo(pOIBindInfo, &bindf, &bindinfo);
    if(FAILED(hres)) {
        WARN("GetBindInfo failed: %08x\n", hres);
        return hres;
    }

    ReleaseBindInfo(&bindinfo);

    len = strlenW(ptr)+3;
    file_name = HeapAlloc(GetProcessHeap(), 0, len*sizeof(WCHAR));
    memcpy(file_name, ptr, len*sizeof(WCHAR));
    hres = UrlUnescapeW(file_name, NULL, &len, URL_UNESCAPE_INPLACE);
    if(FAILED(hres)) {
        WARN("UrlUnescape failed: %08x\n", hres);
        HeapFree(GetProcessHeap(), 0, file_name);
        return hres;
    }

    p = strstrW(file_name, separator);
    if(!p) {
        WARN("invalid url\n");
        HeapFree(GetProcessHeap(), 0, file_name);
        return report_result(pOIProtSink, STG_E_FILENOTFOUND);
    }

    *p = 0;
    chm_file = chm_openW(file_name);
    if(!chm_file) {
        WARN("Could not open chm file\n");
        HeapFree(GetProcessHeap(), 0, file_name);
        return report_result(pOIProtSink, STG_E_FILENOTFOUND);
    }

    object_name = p+2;
    len = strlenW(object_name);

    if(*object_name != '/' && *object_name != '\\') {
        memmove(object_name+1, object_name, (len+1)*sizeof(WCHAR));
        *object_name = '/';
        len++;
    }

    if(object_name[len-1] == '/')
        object_name[--len] = 0;

    for(p=object_name; *p; p++) {
        if(*p == '\\')
            *p = '/';
    }

    remove_dot_segments(object_name);

    TRACE("Resolving %s\n", debugstr_w(object_name));

    memset(&chm_object, 0, sizeof(chm_object));
    res = chm_resolve_object(chm_file, object_name, &chm_object);
    if(res != CHM_RESOLVE_SUCCESS) {
        WARN("Could not resolve chm object\n");
        HeapFree(GetProcessHeap(), 0, file_name);
        chm_close(chm_file);
        return report_result(pOIProtSink, STG_E_FILENOTFOUND);
    }

    IInternetProtocolSink_ReportProgress(pOIProtSink, BINDSTATUS_SENDINGREQUEST,
                                         strrchrW(object_name, '/')+1);

    /* FIXME: Native doesn't use FindMimeFromData */
    hres = FindMimeFromData(NULL, object_name, NULL, 0, NULL, 0, &mime, 0);
    HeapFree(GetProcessHeap(), 0, file_name);
    if(SUCCEEDED(hres)) {
        IInternetProtocolSink_ReportProgress(pOIProtSink, BINDSTATUS_MIMETYPEAVAILABLE, mime);
        CoTaskMemFree(mime);
    }

    release_chm(This); /* Native leaks handle here */
    This->chm_file = chm_file;
    This->chm_object = chm_object;

    hres = IInternetProtocolSink_ReportData(pOIProtSink,
            BSCF_FIRSTDATANOTIFICATION|BSCF_DATAFULLYAVAILABLE,
            chm_object.length, chm_object.length);
    if(FAILED(hres)) {
        WARN("ReportData failed: %08x\n", hres);
        release_chm(This);
        return report_result(pOIProtSink, hres);
    }

    hres = IInternetProtocolSink_ReportProgress(pOIProtSink, BINDSTATUS_BEGINDOWNLOADDATA, NULL);

    return report_result(pOIProtSink, hres);
}

static HRESULT WINAPI ITSProtocol_Continue(IInternetProtocol *iface, PROTOCOLDATA *pProtocolData)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    FIXME("(%p)->(%p)\n", This, pProtocolData);
    return E_NOTIMPL;
}

static HRESULT WINAPI ITSProtocol_Abort(IInternetProtocol *iface, HRESULT hrReason,
        DWORD dwOptions)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    FIXME("(%p)->(%08x %08x)\n", This, hrReason, dwOptions);
    return E_NOTIMPL;
}

static HRESULT WINAPI ITSProtocol_Terminate(IInternetProtocol *iface, DWORD dwOptions)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);

    TRACE("(%p)->(%08x)\n", This, dwOptions);

    return S_OK;
}

static HRESULT WINAPI ITSProtocol_Suspend(IInternetProtocol *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    FIXME("(%p)\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI ITSProtocol_Resume(IInternetProtocol *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    FIXME("(%p)\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI ITSProtocol_Read(IInternetProtocol *iface, void *pv,
        ULONG cb, ULONG *pcbRead)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);

    TRACE("(%p)->(%p %u %p)\n", This, pv, cb, pcbRead);

    if(!This->chm_file)
        return INET_E_DATA_NOT_AVAILABLE;

    *pcbRead = chm_retrieve_object(This->chm_file, &This->chm_object, pv, This->offset, cb);
    This->offset += *pcbRead;

    return *pcbRead ? S_OK : S_FALSE;
}

static HRESULT WINAPI ITSProtocol_Seek(IInternetProtocol *iface, LARGE_INTEGER dlibMove,
        DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);
    FIXME("(%p)->(%d %d %p)\n", This, dlibMove.u.LowPart, dwOrigin, plibNewPosition);
    return E_NOTIMPL;
}

static HRESULT WINAPI ITSProtocol_LockRequest(IInternetProtocol *iface, DWORD dwOptions)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);

    TRACE("(%p)->(%08x)\n", This, dwOptions);

    return S_OK;
}

static HRESULT WINAPI ITSProtocol_UnlockRequest(IInternetProtocol *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocol(iface);

    TRACE("(%p)\n", This);

    return S_OK;
}

static const IInternetProtocolVtbl ITSProtocolVtbl = {
    ITSProtocol_QueryInterface,
    ITSProtocol_AddRef,
    ITSProtocol_Release,
    ITSProtocol_Start,
    ITSProtocol_Continue,
    ITSProtocol_Abort,
    ITSProtocol_Terminate,
    ITSProtocol_Suspend,
    ITSProtocol_Resume,
    ITSProtocol_Read,
    ITSProtocol_Seek,
    ITSProtocol_LockRequest,
    ITSProtocol_UnlockRequest
};

static HRESULT WINAPI ITSProtocolInfo_QueryInterface(IInternetProtocolInfo *iface,
                                              REFIID riid, void **ppv)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);
    return IInternetProtocol_QueryInterface(&This->IInternetProtocol_iface, riid, ppv);
}

static ULONG WINAPI ITSProtocolInfo_AddRef(IInternetProtocolInfo *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);
    return IInternetProtocol_AddRef(&This->IInternetProtocol_iface);
}

static ULONG WINAPI ITSProtocolInfo_Release(IInternetProtocolInfo *iface)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);
    return IInternetProtocol_Release(&This->IInternetProtocol_iface);
}

static HRESULT WINAPI ITSProtocolInfo_ParseUrl(IInternetProtocolInfo *iface, LPCWSTR pwzUrl,
        PARSEACTION ParseAction, DWORD dwParseFlags, LPWSTR pwzResult, DWORD cchResult,
        DWORD *pcchResult, DWORD dwReserved)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);

    TRACE("(%p)->(%s %x %08x %p %d %p %d)\n", This, debugstr_w(pwzUrl), ParseAction,
          dwParseFlags, pwzResult, cchResult, pcchResult, dwReserved);

    switch(ParseAction) {
    case PARSE_CANONICALIZE:
        FIXME("PARSE_CANONICALIZE\n");
        return E_NOTIMPL;
    case PARSE_SECURITY_URL:
        FIXME("PARSE_SECURITY_URL\n");
        return E_NOTIMPL;
    default:
        return INET_E_DEFAULT_ACTION;
    }

    return S_OK;
}

static HRESULT WINAPI ITSProtocolInfo_CombineUrl(IInternetProtocolInfo *iface,
        LPCWSTR pwzBaseUrl, LPCWSTR pwzRelativeUrl, DWORD dwCombineFlags, LPWSTR pwzResult,
        DWORD cchResult, DWORD* pcchResult, DWORD dwReserved)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);
    LPCWSTR base_end, ptr;
    DWORD rel_len;

    static const WCHAR separator[] = {':',':',0};

    TRACE("(%p)->(%s %s %08x %p %d %p %d)\n", This, debugstr_w(pwzBaseUrl),
            debugstr_w(pwzRelativeUrl), dwCombineFlags, pwzResult, cchResult,
            pcchResult, dwReserved);

    base_end = strstrW(pwzBaseUrl, separator);
    if(!base_end)
        return 0x80041001;
    base_end += 2;

    if(!skip_schema(pwzBaseUrl))
        return INET_E_USE_DEFAULT_PROTOCOLHANDLER;

    if(strchrW(pwzRelativeUrl, ':'))
        return STG_E_INVALIDNAME;

    if(pwzRelativeUrl[0] == '#') {
        base_end += strlenW(base_end);
    }else if(pwzRelativeUrl[0] != '/') {
        ptr = strrchrW(base_end, '/');
        if(ptr)
            base_end = ptr+1;
        else
            base_end += strlenW(base_end);
    }

    rel_len = strlenW(pwzRelativeUrl)+1;

    *pcchResult = rel_len + (base_end-pwzBaseUrl);

    if(*pcchResult > cchResult)
        return E_OUTOFMEMORY;

    memcpy(pwzResult, pwzBaseUrl, (base_end-pwzBaseUrl)*sizeof(WCHAR));
    strcpyW(pwzResult + (base_end-pwzBaseUrl), pwzRelativeUrl);

    return S_OK;
}

static HRESULT WINAPI ITSProtocolInfo_CompareUrl(IInternetProtocolInfo *iface, LPCWSTR pwzUrl1,
        LPCWSTR pwzUrl2, DWORD dwCompareFlags)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);
    FIXME("%p)->(%s %s %08x)\n", This, debugstr_w(pwzUrl1), debugstr_w(pwzUrl2), dwCompareFlags);
    return E_NOTIMPL;
}

static HRESULT WINAPI ITSProtocolInfo_QueryInfo(IInternetProtocolInfo *iface, LPCWSTR pwzUrl,
        QUERYOPTION QueryOption, DWORD dwQueryFlags, LPVOID pBuffer, DWORD cbBuffer, DWORD* pcbBuf,
        DWORD dwReserved)
{
    ITSProtocol *This = impl_from_IInternetProtocolInfo(iface);
    FIXME("(%p)->(%s %08x %08x %p %d %p %d)\n", This, debugstr_w(pwzUrl), QueryOption,
          dwQueryFlags, pBuffer, cbBuffer, pcbBuf, dwReserved);
    return E_NOTIMPL;
}

static const IInternetProtocolInfoVtbl ITSProtocolInfoVtbl = {
    ITSProtocolInfo_QueryInterface,
    ITSProtocolInfo_AddRef,
    ITSProtocolInfo_Release,
    ITSProtocolInfo_ParseUrl,
    ITSProtocolInfo_CombineUrl,
    ITSProtocolInfo_CompareUrl,
    ITSProtocolInfo_QueryInfo
};

HRESULT ITSProtocol_create(IUnknown *pUnkOuter, LPVOID *ppobj)
{
    ITSProtocol *ret;

    TRACE("(%p %p)\n", pUnkOuter, ppobj);

    ITSS_LockModule();

    ret = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ITSProtocol));

    ret->IInternetProtocol_iface.lpVtbl = &ITSProtocolVtbl;
    ret->IInternetProtocolInfo_iface.lpVtbl = &ITSProtocolInfoVtbl;
    ret->ref = 1;

    *ppobj = &ret->IInternetProtocol_iface;

    return S_OK;
}
