/*
 * Stream on HGLOBAL Tests
 *
 * Copyright 2006 Robert Shearman (for CodeWeavers)
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

#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H

#define COBJMACROS

#include <stdarg.h>

#include <windef.h>
#include <winbase.h>
#include <ole2.h>
//#include "objbase.h"

#include <wine/test.h>

#define ok_ole_success(hr, func) ok(hr == S_OK, func " failed with error 0x%08x\n", hr)

static char const * const *expected_method_list;

#define CHECK_EXPECTED_METHOD(method_name) \
do { \
    ok(*expected_method_list != NULL, "Extra method %s called\n", method_name); \
        if (*expected_method_list) \
        { \
            ok(!strcmp(*expected_method_list, method_name), "Expected %s to be called instead of %s\n", \
               *expected_method_list, method_name); \
                   expected_method_list++; \
        } \
} while(0)

static void test_streamonhglobal(IStream *pStream)
{
    const char data[] = "Test String";
    ULARGE_INTEGER ull;
    LARGE_INTEGER ll;
    char buffer[128];
    ULONG read;
    STATSTG statstg;
    HRESULT hr;

    ull.QuadPart = sizeof(data);
    hr = IStream_SetSize(pStream, ull);
    ok_ole_success(hr, "IStream_SetSize");

    hr = IStream_Write(pStream, data, sizeof(data), NULL);
    ok_ole_success(hr, "IStream_Write");

    ll.QuadPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, NULL);
    ok_ole_success(hr, "IStream_Seek");

    /* should return S_OK, not S_FALSE */
    hr = IStream_Read(pStream, buffer, sizeof(buffer), &read);
    ok_ole_success(hr, "IStream_Read");
    ok(read == sizeof(data), "IStream_Read returned read %d\n", read);

    /* ignores HighPart */
    ull.u.HighPart = -1;
    ull.u.LowPart = 0;
    hr = IStream_SetSize(pStream, ull);
    ok_ole_success(hr, "IStream_SetSize");

    /* IStream_Seek -- NULL position argument */
    ll.u.HighPart = 0;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, NULL);
    ok_ole_success(hr, "IStream_Seek");

    /* IStream_Seek -- valid position argument (seek from current position) */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == sizeof(data), "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- invalid seek argument */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 123;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_END+1, &ull);
    ok(hr == STG_E_SEEKERROR, "IStream_Seek should have returned STG_E_SEEKERROR instead of 0x%08x\n", hr);
    ok(ull.u.LowPart == sizeof(data), "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should not have changed HighPart, got %d\n", ull.u.HighPart);

    /* IStream_Seek -- valid position argument (seek to beginning) */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0, "should have set LowPart to 0 instead of %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- valid position argument (seek to end) */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_END, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0, "should have set LowPart to 0 instead of %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- ignore HighPart in the move value (seek from current position) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = -1;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == sizeof(data), "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- ignore HighPart in the move value (seek to beginning) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = -1;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0, "should have set LowPart to 0 instead of %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- invalid LowPart value (seek before start of stream) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0x80000000;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok(hr == STG_E_SEEKERROR, "IStream_Seek should have returned STG_E_SEEKERROR instead of 0x%08x\n", hr);
    ok(ull.u.LowPart == sizeof(data), "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- valid LowPart value (seek to start of stream) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = -(DWORD)sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0, "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- invalid LowPart value (seek to start of stream-1) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = -(DWORD)sizeof(data)-1;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok(hr == STG_E_SEEKERROR, "IStream_Seek should have returned STG_E_SEEKERROR instead of 0x%08x\n", hr);
    ok(ull.u.LowPart == sizeof(data), "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- valid LowPart value (seek forward to 0x80000000) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0x80000000 - sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0x80000000, "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- invalid LowPart value (seek to beginning) */
    ll.u.HighPart = 0;
    ll.u.LowPart = sizeof(data);
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0x80000000;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok(hr == STG_E_SEEKERROR, "IStream_Seek should have returned STG_E_SEEKERROR instead of 0x%08x\n", hr);
    ok(ull.u.LowPart == sizeof(data), "LowPart set to %d\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- valid LowPart value (seek to beginning) */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0x7FFFFFFF;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0x7FFFFFFF, "should have set LowPart to 0x7FFFFFFF instead of %08x\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- valid LowPart value (seek from current position) */
    ll.u.HighPart = 0;
    ll.u.LowPart = 0;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_SET, &ull);
    ok_ole_success(hr, "IStream_Seek");

    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0x7FFFFFFF;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0x7FFFFFFF, "should have set LowPart to 0x7FFFFFFF instead of %08x\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- second seek allows you to go past 0x7FFFFFFF size */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 9;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok_ole_success(hr, "IStream_Seek");
    ok(ull.u.LowPart == 0x80000008, "should have set LowPart to 0x80000008 instead of %08x\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    /* IStream_Seek -- seek wraps position/size on integer overflow, but not on win8 */
    ull.u.HighPart = 0xCAFECAFE;
    ull.u.LowPart = 0xCAFECAFE;
    ll.u.HighPart = 0;
    ll.u.LowPart = 0x7FFFFFFF;
    hr = IStream_Seek(pStream, ll, STREAM_SEEK_CUR, &ull);
    ok(hr == S_OK || hr == STG_E_SEEKERROR /* win8 */, "IStream_Seek\n");
    if (SUCCEEDED(hr))
        ok(ull.u.LowPart == 0x00000007, "should have set LowPart to 0x00000007 instead of %08x\n", ull.u.LowPart);
    else
        ok(ull.u.LowPart == 0x80000008, "should have set LowPart to 0x80000008 instead of %08x\n", ull.u.LowPart);
    ok(ull.u.HighPart == 0, "should have set HighPart to 0 instead of %d\n", ull.u.HighPart);

    hr = IStream_Commit(pStream, STGC_DEFAULT);
    ok_ole_success(hr, "IStream_Commit");

    hr = IStream_Revert(pStream);
    ok_ole_success(hr, "IStream_Revert");

    hr = IStream_LockRegion(pStream, ull, ull, LOCK_WRITE);
    ok(hr == STG_E_INVALIDFUNCTION, "IStream_LockRegion should have returned STG_E_INVALIDFUNCTION instead of 0x%08x\n", hr);

    hr = IStream_Stat(pStream, &statstg, STATFLAG_DEFAULT);
    ok_ole_success(hr, "IStream_Stat");
    ok(statstg.type == STGTY_STREAM, "statstg.type should have been STGTY_STREAM instead of %d\n", statstg.type);

    /* test OOM condition */
    ull.u.HighPart = -1;
    ull.u.LowPart = -1;
    hr = IStream_SetSize(pStream, ull);
    ok(hr == E_OUTOFMEMORY || broken(hr == S_OK), /* win9x */
       "IStream_SetSize with large size should have returned E_OUTOFMEMORY instead of 0x%08x\n", hr);
}

static HRESULT WINAPI TestStream_QueryInterface(IStream *iface, REFIID riid, void **ppv)
{
    if (IsEqualIID(riid, &IID_IUnknown) ||
        IsEqualIID(riid, &IID_ISequentialStream) ||
        IsEqualIID(riid, &IID_IStream))
    {
        *ppv = iface;
        IStream_AddRef(iface);
        return S_OK;
    }
    *ppv = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI TestStream_AddRef(IStream *iface)
{
    return 2;
}

static ULONG WINAPI TestStream_Release(IStream *iface)
{
    return 1;
}

static HRESULT WINAPI TestStream_Read(IStream *iface, void *pv, ULONG cb, ULONG *pcbRead)
{
    CHECK_EXPECTED_METHOD("TestStream_Read");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_Write(IStream *iface, const void *pv, ULONG cb, ULONG *pcbWritten)
{
    CHECK_EXPECTED_METHOD("TestStream_Write");
    *pcbWritten = 5;
    return S_OK;
}

static HRESULT WINAPI TestStream_Seek(IStream *iface, LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
    CHECK_EXPECTED_METHOD("TestStream_Seek");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_SetSize(IStream *iface, ULARGE_INTEGER libNewSize)
{
    CHECK_EXPECTED_METHOD("TestStream_SetSize");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_CopyTo(IStream *iface, IStream *pStream, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
    CHECK_EXPECTED_METHOD("TestStream_CopyTo");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_Commit(IStream *iface, DWORD grfCommitFlags)
{
    CHECK_EXPECTED_METHOD("TestStream_Commit");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_Revert(IStream *iface)
{
    CHECK_EXPECTED_METHOD("TestStream_Revert");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_LockRegion(IStream *iface, ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    CHECK_EXPECTED_METHOD("TestStream_LockRegion");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_UnlockRegion(IStream *iface, ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    CHECK_EXPECTED_METHOD("TestStream_UnlockRegion");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_Stat(IStream *iface, STATSTG *pstatstg, DWORD grfStatFlag)
{
    CHECK_EXPECTED_METHOD("TestStream_Stat");
    return E_NOTIMPL;
}

static HRESULT WINAPI TestStream_Clone(IStream *iface, IStream **pStream)
{
    CHECK_EXPECTED_METHOD("TestStream_Clone");
    return E_NOTIMPL;
}

static /*const*/ IStreamVtbl StreamVtbl =
{
    TestStream_QueryInterface,
    TestStream_AddRef,
    TestStream_Release,
    TestStream_Read,
    TestStream_Write,
    TestStream_Seek,
    TestStream_SetSize,
    TestStream_CopyTo,
    TestStream_Commit,
    TestStream_Revert,
    TestStream_LockRegion,
    TestStream_UnlockRegion,
    TestStream_Stat,
    TestStream_Clone
};

static IStream Test_Stream = { &StreamVtbl };

static void test_copyto(void)
{
    IStream *pStream, *pStream2;
    HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
    static const char szHello[] = "Hello";
    ULARGE_INTEGER cb;
    static const char *methods_copyto[] =
    {
        "TestStream_Write",
        NULL
    };
    ULONG written;
    ULARGE_INTEGER ullRead;
    ULARGE_INTEGER ullWritten;
    ULARGE_INTEGER libNewPosition;
    static const LARGE_INTEGER llZero;
    char buffer[15];

    ok_ole_success(hr, "CreateStreamOnHGlobal");

    expected_method_list = methods_copyto;

    hr = IStream_Write(pStream, szHello, sizeof(szHello), &written);
    ok_ole_success(hr, "IStream_Write");
    ok(written == sizeof(szHello), "only %d bytes written\n", written);

    hr = IStream_Seek(pStream, llZero, STREAM_SEEK_SET, NULL);
    ok_ole_success(hr, "IStream_Seek");

    cb.QuadPart = sizeof(szHello);
    hr = IStream_CopyTo(pStream, &Test_Stream, cb, &ullRead, &ullWritten);
    ok(ullWritten.QuadPart == 5, "ullWritten was %d instead\n", (ULONG)ullWritten.QuadPart);
    ok(ullRead.QuadPart == sizeof(szHello), "only %d bytes read\n", (ULONG)ullRead.QuadPart);
    ok_ole_success(hr, "IStream_CopyTo");

    ok(!*expected_method_list, "Method sequence starting from %s not called\n", *expected_method_list);

    hr = IStream_Clone(pStream, &pStream2);
    ok_ole_success(hr, "IStream_Clone");

    hr = IStream_Seek(pStream2, llZero, STREAM_SEEK_CUR, &libNewPosition);
    ok_ole_success(hr, "IStream_Seek");
    ok(libNewPosition.QuadPart == sizeof(szHello), "libNewPosition wasn't set correctly for the cloned stream\n");

    hr = IStream_Seek(pStream2, llZero, STREAM_SEEK_SET, NULL);
    ok_ole_success(hr, "IStream_Seek");

    hr = IStream_Read(pStream2, buffer, sizeof(buffer), NULL);
    ok_ole_success(hr, "IStream_Read");
    ok(!strcmp(buffer, szHello), "read data \"%s\" didn't match originally written data\n", buffer);

    IStream_Release(pStream2);
    IStream_Release(pStream);
}

static void test_freed_hglobal(void)
{
    static const char teststring[] = "this is a test string";
    HRESULT hr;
    IStream *pStream;
    HGLOBAL hglobal;
    char *p;
    char buffer[sizeof(teststring) + 8];
    ULARGE_INTEGER ull;
    ULONG read, written;

    hglobal = GlobalAlloc(GMEM_DDESHARE|GMEM_NODISCARD|GMEM_MOVEABLE, strlen(teststring) + 1);
    ok(hglobal != NULL, "GlobalAlloc failed with error %d\n", GetLastError());
    p = GlobalLock(hglobal);
    strcpy(p, teststring);
    GlobalUnlock(hglobal);

    hr = CreateStreamOnHGlobal(hglobal, FALSE, &pStream);
    ok_ole_success(hr, "CreateStreamOnHGlobal");

    hr = IStream_Read(pStream, buffer, sizeof(buffer), &read);
    ok_ole_success(hr, "IStream_Read");
    ok(!strcmp(buffer, teststring), "buffer data %s differs\n", buffer);
    ok(read == sizeof(teststring) ||
       broken(read == ((sizeof(teststring) + 3) & ~3)), /* win9x rounds the size */
       "read should be sizeof(teststring) instead of %d\n", read);

    GlobalFree(hglobal);

    memset(buffer, 0, sizeof(buffer));
    read = -1;
    hr = IStream_Read(pStream, buffer, sizeof(buffer), &read);
    ok_ole_success(hr, "IStream_Read");
    ok(buffer[0] == 0, "buffer data should be untouched\n");
    ok(read == 0, "read should be 0 instead of %d\n", read);

    ull.QuadPart = sizeof(buffer);
    hr = IStream_SetSize(pStream, ull);
    ok(hr == E_OUTOFMEMORY, "IStream_SetSize with invalid HGLOBAL should return E_OUTOFMEMORY instead of 0x%08x\n", hr);

    hr = IStream_Write(pStream, buffer, sizeof(buffer), &written);
    ok(hr == E_OUTOFMEMORY, "IStream_Write with invalid HGLOBAL should return E_OUTOFMEMORY instead of 0x%08x\n", hr);
    ok(written == 0, "written should be 0 instead of %d\n", written);

    IStream_Release(pStream);
}

START_TEST(hglobalstream)
{
    HRESULT hr;
    IStream *pStream;

    hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
    ok_ole_success(hr, "CreateStreamOnHGlobal");

    test_streamonhglobal(pStream);
    IStream_Release(pStream);
    test_copyto();
    test_freed_hglobal();
}
