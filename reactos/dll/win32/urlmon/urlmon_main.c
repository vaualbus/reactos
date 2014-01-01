/*
 * UrlMon
 *
 * Copyright (c) 2000 Patrik Stridvall
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

#include "urlmon_main.h"

#include <initguid.h>
DEFINE_GUID(CLSID_CUri, 0xDF2FCE13, 0x25EC, 0x45BB, 0x9D,0x4C, 0xCE,0xCD,0x47,0xC2,0x43,0x0C);

LONG URLMON_refCount = 0;
HINSTANCE urlmon_instance;

static HMODULE hCabinet = NULL;
static DWORD urlmon_tls = TLS_OUT_OF_INDEXES;

static void init_session(void);

static struct list tls_list = LIST_INIT(tls_list);

static CRITICAL_SECTION tls_cs;
static CRITICAL_SECTION_DEBUG tls_cs_dbg =
{
    0, 0, &tls_cs,
    { &tls_cs_dbg.ProcessLocksList, &tls_cs_dbg.ProcessLocksList },
      0, 0, { (DWORD_PTR)(__FILE__ ": tls") }
};

static CRITICAL_SECTION tls_cs = { &tls_cs_dbg, -1, 0, 0, 0, 0 };

tls_data_t *get_tls_data(void)
{
    tls_data_t *data;

    if(urlmon_tls == TLS_OUT_OF_INDEXES) {
        DWORD tls = TlsAlloc();
        if(tls == TLS_OUT_OF_INDEXES)
            return NULL;

        tls = InterlockedCompareExchange((LONG*)&urlmon_tls, tls, TLS_OUT_OF_INDEXES);
        if(tls != urlmon_tls)
            TlsFree(tls);
    }

    data = TlsGetValue(urlmon_tls);
    if(!data) {
        data = heap_alloc_zero(sizeof(tls_data_t));
        if(!data)
            return NULL;

        EnterCriticalSection(&tls_cs);
        list_add_tail(&tls_list, &data->entry);
        LeaveCriticalSection(&tls_cs);

        TlsSetValue(urlmon_tls, data);
    }

    return data;
}

static void free_tls_list(void)
{
    tls_data_t *data;

    if(urlmon_tls == TLS_OUT_OF_INDEXES)
        return;

    while(!list_empty(&tls_list)) {
        data = LIST_ENTRY(list_head(&tls_list), tls_data_t, entry);
        list_remove(&data->entry);
        heap_free(data);
    }

    TlsFree(urlmon_tls);
}

static void detach_thread(void)
{
    tls_data_t *data;

    if(urlmon_tls == TLS_OUT_OF_INDEXES)
        return;

    data = TlsGetValue(urlmon_tls);
    if(!data)
        return;

    EnterCriticalSection(&tls_cs);
    list_remove(&data->entry);
    LeaveCriticalSection(&tls_cs);

    if(data->notif_hwnd) {
        WARN("notif_hwnd not destroyed\n");
        DestroyWindow(data->notif_hwnd);
    }

    heap_free(data);
}

static void process_detach(void)
{
    HINTERNET internet_session;

    internet_session = get_internet_session(NULL);
    if(internet_session)
        InternetCloseHandle(internet_session);

    if (hCabinet)
        FreeLibrary(hCabinet);

    free_session();
    free_tls_list();
}

/***********************************************************************
 *		DllMain (URLMON.init)
 */
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID fImpLoad)
{
    TRACE("%p 0x%x %p\n", hinstDLL, fdwReason, fImpLoad);

    URLMON_DllMain( hinstDLL, fdwReason, fImpLoad );

    switch(fdwReason) {
    case DLL_PROCESS_ATTACH:
        urlmon_instance = hinstDLL;
        init_session();
        break;

    case DLL_PROCESS_DETACH:
        if (fImpLoad) break;
        process_detach();
        DeleteCriticalSection(&tls_cs);
        break;

    case DLL_THREAD_DETACH:
        detach_thread();
        break;
    }
    return TRUE;
}

const char *debugstr_bindstatus(ULONG status)
{
    switch(status) {
#define X(x) case x: return #x
    X(BINDSTATUS_FINDINGRESOURCE);
    X(BINDSTATUS_CONNECTING);
    X(BINDSTATUS_REDIRECTING);
    X(BINDSTATUS_BEGINDOWNLOADDATA);
    X(BINDSTATUS_DOWNLOADINGDATA);
    X(BINDSTATUS_ENDDOWNLOADDATA);
    X(BINDSTATUS_BEGINDOWNLOADCOMPONENTS);
    X(BINDSTATUS_INSTALLINGCOMPONENTS);
    X(BINDSTATUS_ENDDOWNLOADCOMPONENTS);
    X(BINDSTATUS_USINGCACHEDCOPY);
    X(BINDSTATUS_SENDINGREQUEST);
    X(BINDSTATUS_CLASSIDAVAILABLE);
    X(BINDSTATUS_MIMETYPEAVAILABLE);
    X(BINDSTATUS_CACHEFILENAMEAVAILABLE);
    X(BINDSTATUS_BEGINSYNCOPERATION);
    X(BINDSTATUS_ENDSYNCOPERATION);
    X(BINDSTATUS_BEGINUPLOADDATA);
    X(BINDSTATUS_UPLOADINGDATA);
    X(BINDSTATUS_ENDUPLOADINGDATA);
    X(BINDSTATUS_PROTOCOLCLASSID);
    X(BINDSTATUS_ENCODING);
    X(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE);
    X(BINDSTATUS_CLASSINSTALLLOCATION);
    X(BINDSTATUS_DECODING);
    X(BINDSTATUS_LOADINGMIMEHANDLER);
    X(BINDSTATUS_CONTENTDISPOSITIONATTACH);
    X(BINDSTATUS_FILTERREPORTMIMETYPE);
    X(BINDSTATUS_CLSIDCANINSTANTIATE);
    X(BINDSTATUS_IUNKNOWNAVAILABLE);
    X(BINDSTATUS_DIRECTBIND);
    X(BINDSTATUS_RAWMIMETYPE);
    X(BINDSTATUS_PROXYDETECTING);
    X(BINDSTATUS_ACCEPTRANGES);
    X(BINDSTATUS_COOKIE_SENT);
    X(BINDSTATUS_COMPACT_POLICY_RECEIVED);
    X(BINDSTATUS_COOKIE_SUPPRESSED);
    X(BINDSTATUS_COOKIE_STATE_UNKNOWN);
    X(BINDSTATUS_COOKIE_STATE_ACCEPT);
    X(BINDSTATUS_COOKIE_STATE_REJECT);
    X(BINDSTATUS_COOKIE_STATE_PROMPT);
    X(BINDSTATUS_COOKIE_STATE_LEASH);
    X(BINDSTATUS_COOKIE_STATE_DOWNGRADE);
    X(BINDSTATUS_POLICY_HREF);
    X(BINDSTATUS_P3P_HEADER);
    X(BINDSTATUS_SESSION_COOKIE_RECEIVED);
    X(BINDSTATUS_PERSISTENT_COOKIE_RECEIVED);
    X(BINDSTATUS_SESSION_COOKIES_ALLOWED);
    X(BINDSTATUS_CACHECONTROL);
    X(BINDSTATUS_CONTENTDISPOSITIONFILENAME);
    X(BINDSTATUS_MIMETEXTPLAINMISMATCH);
    X(BINDSTATUS_PUBLISHERAVAILABLE);
    X(BINDSTATUS_DISPLAYNAMEAVAILABLE);
#undef X
    default:
        return wine_dbg_sprintf("(invalid status %u)", status);
    }
}

/***********************************************************************
 *		DllInstall (URLMON.@)
 */
HRESULT WINAPI DllInstall(BOOL bInstall, LPCWSTR cmdline)
{
  FIXME("(%s, %s): stub\n", bInstall?"TRUE":"FALSE",
	debugstr_w(cmdline));

  return S_OK;
}

/***********************************************************************
 *		DllCanUnloadNow (URLMON.@)
 */
HRESULT WINAPI DllCanUnloadNow(void)
{
    return URLMON_refCount != 0 ? S_FALSE : S_OK;
}



/******************************************************************************
 * Urlmon ClassFactory
 */
typedef struct {
    IClassFactory IClassFactory_iface;

    HRESULT (*pfnCreateInstance)(IUnknown *pUnkOuter, LPVOID *ppObj);
} ClassFactory;

static inline ClassFactory *impl_from_IClassFactory(IClassFactory *iface)
{
    return CONTAINING_RECORD(iface, ClassFactory, IClassFactory_iface);
}

static HRESULT WINAPI CF_QueryInterface(IClassFactory *iface, REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    if(IsEqualGUID(riid, &IID_IUnknown)) {
        TRACE("(%p)->(IID_IUnknown %p)\n", iface, ppv);
        *ppv = iface;
    }else if(IsEqualGUID(riid, &IID_IClassFactory)) {
        TRACE("(%p)->(IID_IClassFactory %p)\n", iface, ppv);
        *ppv = iface;
    }

    if(*ppv) {
	IUnknown_AddRef((IUnknown*)*ppv);
	return S_OK;
    }

    WARN("(%p)->(%s,%p),not found\n", iface, debugstr_guid(riid), ppv);
    return E_NOINTERFACE;
}

static ULONG WINAPI CF_AddRef(IClassFactory *iface)
{
    URLMON_LockModule();
    return 2;
}

static ULONG WINAPI CF_Release(IClassFactory *iface)
{
    URLMON_UnlockModule();
    return 1;
}


static HRESULT WINAPI CF_CreateInstance(IClassFactory *iface, IUnknown *pOuter,
                                        REFIID riid, LPVOID *ppobj)
{
    ClassFactory *This = impl_from_IClassFactory(iface);
    HRESULT hres;
    LPUNKNOWN punk;
    
    TRACE("(%p)->(%p,%s,%p)\n",This,pOuter,debugstr_guid(riid),ppobj);

    *ppobj = NULL;
    if(SUCCEEDED(hres = This->pfnCreateInstance(pOuter, (LPVOID *) &punk))) {
        hres = IUnknown_QueryInterface(punk, riid, ppobj);
        IUnknown_Release(punk);
    }
    return hres;
}

static HRESULT WINAPI CF_LockServer(LPCLASSFACTORY iface,BOOL dolock)
{
    TRACE("(%d)\n", dolock);

    if (dolock)
	   URLMON_LockModule();
    else
	   URLMON_UnlockModule();

    return S_OK;
}

static const IClassFactoryVtbl ClassFactoryVtbl =
{
    CF_QueryInterface,
    CF_AddRef,
    CF_Release,
    CF_CreateInstance,
    CF_LockServer
};

static ClassFactory FileProtocolCF =
    { { &ClassFactoryVtbl }, FileProtocol_Construct};
static ClassFactory FtpProtocolCF =
    { { &ClassFactoryVtbl }, FtpProtocol_Construct};
static ClassFactory GopherProtocolCF =
    { { &ClassFactoryVtbl }, GopherProtocol_Construct};
static ClassFactory HttpProtocolCF =
    { { &ClassFactoryVtbl }, HttpProtocol_Construct};
static ClassFactory HttpSProtocolCF =
    { { &ClassFactoryVtbl }, HttpSProtocol_Construct};
static ClassFactory MkProtocolCF =
    { { &ClassFactoryVtbl }, MkProtocol_Construct};
static ClassFactory SecurityManagerCF =
    { { &ClassFactoryVtbl }, SecManagerImpl_Construct};
static ClassFactory ZoneManagerCF =
    { { &ClassFactoryVtbl }, ZoneMgrImpl_Construct};
static ClassFactory StdURLMonikerCF =
    { { &ClassFactoryVtbl }, StdURLMoniker_Construct};
static ClassFactory MimeFilterCF =
    { { &ClassFactoryVtbl }, MimeFilter_Construct};
static ClassFactory CUriCF =
    { { &ClassFactoryVtbl }, Uri_Construct};

struct object_creation_info
{
    const CLSID *clsid;
    IClassFactory *cf;
    LPCWSTR protocol;
};

static const WCHAR wszFile[] = {'f','i','l','e',0};
static const WCHAR wszFtp[]  = {'f','t','p',0};
static const WCHAR wszGopher[]  = {'g','o','p','h','e','r',0};
static const WCHAR wszHttp[] = {'h','t','t','p',0};
static const WCHAR wszHttps[] = {'h','t','t','p','s',0};
static const WCHAR wszMk[]   = {'m','k',0};

static const struct object_creation_info object_creation[] =
{
    { &CLSID_FileProtocol,            &FileProtocolCF.IClassFactory_iface,    wszFile },
    { &CLSID_FtpProtocol,             &FtpProtocolCF.IClassFactory_iface,     wszFtp  },
    { &CLSID_GopherProtocol,          &GopherProtocolCF.IClassFactory_iface,  wszGopher },
    { &CLSID_HttpProtocol,            &HttpProtocolCF.IClassFactory_iface,    wszHttp },
    { &CLSID_HttpSProtocol,           &HttpSProtocolCF.IClassFactory_iface,   wszHttps },
    { &CLSID_MkProtocol,              &MkProtocolCF.IClassFactory_iface,      wszMk },
    { &CLSID_InternetSecurityManager, &SecurityManagerCF.IClassFactory_iface, NULL    },
    { &CLSID_InternetZoneManager,     &ZoneManagerCF.IClassFactory_iface,     NULL    },
    { &CLSID_StdURLMoniker,           &StdURLMonikerCF.IClassFactory_iface,   NULL    },
    { &CLSID_DeCompMimeFilter,        &MimeFilterCF.IClassFactory_iface,      NULL    },
    { &CLSID_CUri,                    &CUriCF.IClassFactory_iface,            NULL    }
};

static void init_session(void)
{
    unsigned int i;

    for(i=0; i < sizeof(object_creation)/sizeof(object_creation[0]); i++) {
        if(object_creation[i].protocol)
            register_namespace(object_creation[i].cf, object_creation[i].clsid,
                                      object_creation[i].protocol, TRUE);
    }
}

/*******************************************************************************
 * DllGetClassObject [URLMON.@]
 * Retrieves class object from a DLL object
 *
 * NOTES
 *    Docs say returns STDAPI
 *
 * PARAMS
 *    rclsid [I] CLSID for the class object
 *    riid   [I] Reference to identifier of interface for class object
 *    ppv    [O] Address of variable to receive interface pointer for riid
 *
 * RETURNS
 *    Success: S_OK
 *    Failure: CLASS_E_CLASSNOTAVAILABLE, E_OUTOFMEMORY, E_INVALIDARG,
 *             E_UNEXPECTED
 */

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    unsigned int i;
    HRESULT hr;
    
    TRACE("(%s,%s,%p)\n", debugstr_guid(rclsid), debugstr_guid(riid), ppv);
    
    for (i=0; i < sizeof(object_creation)/sizeof(object_creation[0]); i++)
    {
	if (IsEqualGUID(object_creation[i].clsid, rclsid))
	    return IClassFactory_QueryInterface(object_creation[i].cf, riid, ppv);
    }

    hr = URLMON_DllGetClassObject(rclsid, riid, ppv);
    if(SUCCEEDED(hr))
        return hr;

    FIXME("%s: no class found.\n", debugstr_guid(rclsid));
    return CLASS_E_CLASSNOTAVAILABLE;
}

static HRESULT register_inf(BOOL doregister)
{
    HRESULT (WINAPI *pRegInstall)(HMODULE hm, LPCSTR pszSection, const STRTABLEA* pstTable);
    HMODULE hAdvpack;

    static const WCHAR wszAdvpack[] = {'a','d','v','p','a','c','k','.','d','l','l',0};

    hAdvpack = LoadLibraryW(wszAdvpack);
    pRegInstall = (void *)GetProcAddress(hAdvpack, "RegInstall");

    return pRegInstall(hProxyDll, doregister ? "RegisterDll" : "UnregisterDll", NULL);
}

/***********************************************************************
 *		DllRegisterServer (URLMON.@)
 */
HRESULT WINAPI DllRegisterServer(void)
{
    HRESULT hr;

    TRACE("\n");

    hr = URLMON_DllRegisterServer();
    return SUCCEEDED(hr) ? register_inf(TRUE) : hr;
}

/***********************************************************************
 *		DllUnregisterServer (URLMON.@)
 */
HRESULT WINAPI DllUnregisterServer(void)
{
    HRESULT hr;

    TRACE("\n");

    hr = URLMON_DllUnregisterServer();
    return SUCCEEDED(hr) ? register_inf(FALSE) : hr;
}

/***********************************************************************
 *		DllRegisterServerEx (URLMON.@)
 */
HRESULT WINAPI DllRegisterServerEx(void)
{
    FIXME("(void): stub\n");

    return E_FAIL;
}

/**************************************************************************
 *                 IsValidURL (URLMON.@)
 * 
 * Determines if a specified string is a valid URL.
 *
 * PARAMS
 *  pBC        [I] ignored, should be NULL.
 *  szURL      [I] string that represents the URL in question.
 *  dwReserved [I] reserved and must be zero.
 *
 * RETURNS
 *  Success: S_OK.
 *  Failure: S_FALSE.
 *  returns E_INVALIDARG if one or more of the args is invalid.
 *
 * TODO:
 *  test functionality against windows to see what a valid URL is.
 */
HRESULT WINAPI IsValidURL(LPBC pBC, LPCWSTR szURL, DWORD dwReserved)
{
    FIXME("(%p, %s, %d): stub\n", pBC, debugstr_w(szURL), dwReserved);

    if (dwReserved || !szURL)
        return E_INVALIDARG;

    return S_OK;
}

/**************************************************************************
 *                 FaultInIEFeature (URLMON.@)
 *
 *  Undocumented.  Appears to be used by native shdocvw.dll.
 */
HRESULT WINAPI FaultInIEFeature( HWND hwnd, uCLSSPEC * pClassSpec,
                                 QUERYCONTEXT *pQuery, DWORD flags )
{
    FIXME("%p %p %p %08x\n", hwnd, pClassSpec, pQuery, flags);
    return E_NOTIMPL;
}

/**************************************************************************
 *                 CoGetClassObjectFromURL (URLMON.@)
 */
HRESULT WINAPI CoGetClassObjectFromURL( REFCLSID rclsid, LPCWSTR szCodeURL, DWORD dwFileVersionMS,
                                        DWORD dwFileVersionLS, LPCWSTR szContentType,
                                        LPBINDCTX pBindCtx, DWORD dwClsContext, LPVOID pvReserved,
                                        REFIID riid, LPVOID *ppv )
{
    FIXME("(%s %s %d %d %s %p %d %p %s %p) Stub!\n", debugstr_guid(rclsid), debugstr_w(szCodeURL),
	dwFileVersionMS, dwFileVersionLS, debugstr_w(szContentType), pBindCtx, dwClsContext, pvReserved,
	debugstr_guid(riid), ppv);
    return E_NOINTERFACE;
}

/***********************************************************************
 *           ReleaseBindInfo (URLMON.@)
 *
 * Release the resources used by the specified BINDINFO structure.
 *
 * PARAMS
 *  pbindinfo [I] BINDINFO to release.
 *
 * RETURNS
 *  Nothing.
 */
void WINAPI ReleaseBindInfo(BINDINFO* pbindinfo)
{
    DWORD size;

    TRACE("(%p)\n", pbindinfo);

    if(!pbindinfo || !(size = pbindinfo->cbSize))
        return;

    CoTaskMemFree(pbindinfo->szExtraInfo);
    ReleaseStgMedium(&pbindinfo->stgmedData);

    if(offsetof(BINDINFO, szExtraInfo) < size)
        CoTaskMemFree(pbindinfo->szCustomVerb);

    if(pbindinfo->pUnk && offsetof(BINDINFO, pUnk) < size)
        IUnknown_Release(pbindinfo->pUnk);

    memset(pbindinfo, 0, size);
    pbindinfo->cbSize = size;
}

/***********************************************************************
 *           CopyStgMedium (URLMON.@)
 */
HRESULT WINAPI CopyStgMedium(const STGMEDIUM *src, STGMEDIUM *dst)
{
    TRACE("(%p %p)\n", src, dst);

    if(!src || !dst)
        return E_POINTER;

    *dst = *src;

    switch(dst->tymed) {
    case TYMED_NULL:
        break;
    case TYMED_FILE:
        if(src->u.lpszFileName && !src->pUnkForRelease) {
            DWORD size = (strlenW(src->u.lpszFileName)+1)*sizeof(WCHAR);
            dst->u.lpszFileName = CoTaskMemAlloc(size);
            if(!dst->u.lpszFileName)
                return E_OUTOFMEMORY;
            memcpy(dst->u.lpszFileName, src->u.lpszFileName, size);
        }
        break;
    case TYMED_ISTREAM:
        if(dst->u.pstm)
            IStream_AddRef(dst->u.pstm);
        break;
    case TYMED_ISTORAGE:
        if(dst->u.pstg)
            IStorage_AddRef(dst->u.pstg);
        break;
    case TYMED_HGLOBAL:
        if(dst->u.hGlobal) {
            SIZE_T size = GlobalSize(src->u.hGlobal);
            char *src_ptr, *dst_ptr;

            dst->u.hGlobal = GlobalAlloc(GMEM_FIXED, size);
            if(!dst->u.hGlobal)
                return E_OUTOFMEMORY;
            dst_ptr = GlobalLock(dst->u.hGlobal);
            src_ptr = GlobalLock(src->u.hGlobal);
            memcpy(dst_ptr, src_ptr, size);
            GlobalUnlock(src_ptr);
            GlobalUnlock(dst_ptr);
        }
        break;
    default:
        FIXME("Unimplemented tymed %d\n", src->tymed);
    }

    if(dst->pUnkForRelease)
        IUnknown_AddRef(dst->pUnkForRelease);

    return S_OK;
}

/***********************************************************************
 *           CopyBindInfo (URLMON.@)
 */
HRESULT WINAPI CopyBindInfo(const BINDINFO *pcbiSrc, BINDINFO *pcbiDest)
{
    DWORD size;
    HRESULT hres;

    TRACE("(%p %p)\n", pcbiSrc, pcbiDest);

    if(!pcbiSrc || !pcbiDest)
        return E_POINTER;
    if(!pcbiSrc->cbSize || !pcbiDest->cbSize)
        return E_INVALIDARG;

    size = pcbiDest->cbSize;
    if(size > pcbiSrc->cbSize) {
        memcpy(pcbiDest, pcbiSrc, pcbiSrc->cbSize);
        memset((char*)pcbiDest+pcbiSrc->cbSize, 0, size-pcbiSrc->cbSize);
    } else {
        memcpy(pcbiDest, pcbiSrc, size);
    }
    pcbiDest->cbSize = size;

    size = FIELD_OFFSET(BINDINFO, szExtraInfo)+sizeof(void*);
    if(pcbiSrc->cbSize>=size && pcbiDest->cbSize>=size && pcbiSrc->szExtraInfo) {
        size = (strlenW(pcbiSrc->szExtraInfo)+1)*sizeof(WCHAR);
        pcbiDest->szExtraInfo = CoTaskMemAlloc(size);
        if(!pcbiDest->szExtraInfo)
            return E_OUTOFMEMORY;
        memcpy(pcbiDest->szExtraInfo, pcbiSrc->szExtraInfo, size);
    }

    size = FIELD_OFFSET(BINDINFO, stgmedData)+sizeof(STGMEDIUM);
    if(pcbiSrc->cbSize>=size && pcbiDest->cbSize>=size) {
        hres = CopyStgMedium(&pcbiSrc->stgmedData, &pcbiDest->stgmedData);
        if(FAILED(hres)) {
            CoTaskMemFree(pcbiDest->szExtraInfo);
            return hres;
        }
    }

    size = FIELD_OFFSET(BINDINFO, szCustomVerb)+sizeof(void*);
    if(pcbiSrc->cbSize>=size && pcbiDest->cbSize>=size && pcbiSrc->szCustomVerb) {
        size = (strlenW(pcbiSrc->szCustomVerb)+1)*sizeof(WCHAR);
        pcbiDest->szCustomVerb = CoTaskMemAlloc(size);
        if(!pcbiDest->szCustomVerb) {
            CoTaskMemFree(pcbiDest->szExtraInfo);
            ReleaseStgMedium(&pcbiDest->stgmedData);
            return E_OUTOFMEMORY;
        }
        memcpy(pcbiDest->szCustomVerb, pcbiSrc->szCustomVerb, size);
    }

    size = FIELD_OFFSET(BINDINFO, securityAttributes)+sizeof(SECURITY_ATTRIBUTES);
    if(pcbiDest->cbSize >= size)
        memset(&pcbiDest->securityAttributes, 0, sizeof(SECURITY_ATTRIBUTES));

    if(pcbiSrc->pUnk)
        IUnknown_AddRef(pcbiDest->pUnk);

    return S_OK;
}

/***********************************************************************
 *           GetClassFileOrMime (URLMON.@)
 *
 * Determines the class ID from the bind context, file name or MIME type.
 */
HRESULT WINAPI GetClassFileOrMime(LPBC pBC, LPCWSTR pszFilename,
        LPVOID pBuffer, DWORD cbBuffer, LPCWSTR pszMimeType, DWORD dwReserved,
        CLSID *pclsid)
{
    FIXME("(%p, %s, %p, %d, %s, 0x%08x, %p): stub\n", pBC, debugstr_w(pszFilename), pBuffer,
            cbBuffer, debugstr_w(pszMimeType), dwReserved, pclsid);
    return E_NOTIMPL;
}

/***********************************************************************
 * Extract (URLMON.@)
 */
HRESULT WINAPI Extract(void *dest, LPCSTR szCabName)
{
    HRESULT (WINAPI *pExtract)(void *, LPCSTR);

    if (!hCabinet)
        hCabinet = LoadLibraryA("cabinet.dll");

    if (!hCabinet) return HRESULT_FROM_WIN32(GetLastError());
    pExtract = (void *)GetProcAddress(hCabinet, "Extract");
    if (!pExtract) return HRESULT_FROM_WIN32(GetLastError());

    return pExtract(dest, szCabName);
}

/***********************************************************************
 *           IsLoggingEnabledA (URLMON.@)
 */
BOOL WINAPI IsLoggingEnabledA(LPCSTR url)
{
    FIXME("(%s)\n", debugstr_a(url));
    return FALSE;
}

/***********************************************************************
 *           IsLoggingEnabledW (URLMON.@)
 */
BOOL WINAPI IsLoggingEnabledW(LPCWSTR url)
{
    FIXME("(%s)\n", debugstr_w(url));
    return FALSE;
}

/***********************************************************************
 *           IsProtectedModeURL (URLMON.111)
 *    Undocumented, added in IE7
 */
BOOL WINAPI IsProtectedModeURL(const WCHAR *url)
{
    FIXME("stub: %s\n", debugstr_w(url));
    return TRUE;
}

/***********************************************************************
 *           LogSqmBits (URLMON.410)
 *    Undocumented, added in IE8
 */
int WINAPI LogSqmBits(DWORD unk1, DWORD unk2)
{
    FIXME("stub: %d %d\n", unk1, unk2);
    return 0;
}

/***********************************************************************
 *           LogSqmUXCommandOffsetInternal (URLMON.423)
 *    Undocumented, added in IE8
 */
void WINAPI LogSqmUXCommandOffsetInternal(DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4)
{
    FIXME("stub: %d %d %d %d\n", unk1, unk2, unk3, unk4);
}

/***********************************************************************
 *           MapUriToBrowserEmulationState (URLMON.444)
 *    Undocumented, added in IE8
 */
int WINAPI MapUriToBrowserEmulationState(DWORD unk1, DWORD unk2, DWORD unk3)
{
    FIXME("stub: %d %d %d\n", unk1, unk2, unk3);
    return 0;
}

/***********************************************************************
 *           MapBrowserEmulationModeToUserAgent (URLMON.445)
 *    Undocumented, added in IE8
 */
int WINAPI MapBrowserEmulationModeToUserAgent(DWORD unk1, DWORD unk2)
{
    FIXME("stub: %d %d\n", unk1, unk2);
    return 0;
}

/***********************************************************************
 *           FlushUrlmonZonesCache (URLMON.455)
 *    Undocumented, added in IE8
 */
void WINAPI FlushUrlmonZonesCache(void)
{
    FIXME("stub\n");
}

/***********************************************************************
 *            RegisterMediaTypes
 *    Added in IE3, registers known MIME-type strings.
 */
HRESULT WINAPI RegisterMediaTypes(UINT types, LPCSTR *szTypes, CLIPFORMAT *cfTypes)
{
   FIXME("stub: %u %p %p\n", types, szTypes, cfTypes);
   return E_INVALIDARG;
}
