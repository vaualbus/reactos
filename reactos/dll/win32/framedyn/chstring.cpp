/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            dll/win32/framedyn/chstring.cpp
 * PURPOSE:         CHString class implementation
 * PROGRAMMERS:     Pierre Schweitzer (pierre@reactos.org)
 *
 * NOTE:            This implementation is BROKEN on PURPOSE
 *                  The CHString is a mix between std::string and
 *                  UNICODE_STRING. It appears that basically it takes only
 *                  the worse from both approaches.
 *                  I've copied the behavior and implementation of Windows 2k3 even if
 *                  it implies unsafe, wrong or unefficient methods.
 *                  Note that the string at m_pchData might not be null terminated!
 *                  Also, important note, two (or even more) CHString instances might
 *                  have the same m_pchData object! Never forget that while modifying
 *                  a string. You might be modifying the string for everyone.
 *                  This is why a protected method is being used in the code: CopyBeforeWrite
 *                  It copies source first, to ensure we only modify current string
 *                  Side note, all the sizes are actually a number of chars. Only the size
 *                  for implementation is the number of bytes
 *                  Now, you know why this class is deprecated and shouldn't be used
 */

/* INCLUDES ******************************************************************/

#include <chstring.h>
#define NDEBUG
#include <debug.h>

/* PRIVATE FUNCTIONS *********************************************************/

// This is the empty string that defaults strings without text
// This is unsafe. This string show be LPCWSTR
// However we have to assign it to LPWSTR var. So, let's ignore about const,
// as MS does. Normally we check in our code that we don't overwrite this string.
LPWSTR afxPchNil = (LPWSTR)L"\0";
// This is the data that are matching the null string upper
CHStringData afxNullData = {0, 0, 0};
// Exception we may throw in case of allocation failure
CHeap_Exception HeapException(CHeap_Exception::E_ALLOCATION_ERROR);

// Our own delete operator
// It is here basically because MS guys don't known about set_new_handler()
// See operator new
void operator delete(void* ptr)
{
    // In Windows 2k3, they check for ptr being null.
    // ISO, POSIX and even MSDN explains that it is allowed
    // to call free with NULL pointer...
    if (ptr)
    {
        free(ptr);
    }
}

// Implement our own new operator so that we can throw our own exception in case
// of allocation failure.
// It could have been done using set_new_handler(), but well. MS guys didn't do it
// that way. So, let's mimic.
void* operator new(size_t uSize)
{
    void* Buffer;

    Buffer = malloc(uSize);
    if (!Buffer)
    {
        throw HeapException;
    }

    return Buffer;
}

// This is a char to wchar string conversion helper
int mbstowcsz(LPWSTR lpDest, LPCSTR lpSrc, int nLen)
{
    int Conv;

    // If we have nothing to convert or if output doesn't exist, return
    if (nLen == 0 || lpDest == 0)
    {
        return 0;
    }

    // Then, simply convert
    Conv = MultiByteToWideChar(CP_ACP, 0, lpSrc, -1, lpDest, nLen);
    // In case of conversion success, null terminate the string
    if (Conv != 0)
    {
        lpDest[nLen] = 0;
    }

    return Conv;
}

/* PUBLIC FUNCTIONS **********************************************************/

/*
 * @implemented
 */
CHString::CHString()
{
    // Set to empty string
    m_pchData = afxPchNil;
}

/*
 * @implemented
 */
CHString::CHString(WCHAR ch, int nRepeat) throw (CHeap_Exception)
{
    // Allow null initialize, in case something goes wrong
    m_pchData = afxPchNil;

    // If we have a char to insert
    if (nRepeat >= 1)
    {
        // Allocate a buffer big enough
        AllocBuffer(nRepeat);
        // And if possible, repeat char
        if (m_pchData)
        {
            for (int i = 0; i < nRepeat; ++i)
            {
                m_pchData[i] = ch;
            }
        }
    }
}

/*
 * @implemented
 */
CHString::CHString(LPCWSTR lpsz) throw (CHeap_Exception)
{
    // Allow null initialize, in case something goes wrong
    m_pchData = afxPchNil;

    // If we have an input string
    if (lpsz != 0)
    {
        // Get its length
        int Len = SafeStrlen(lpsz);
        // Then, allocate a big enough buffer and copy string
        // Note that here, we don't null terminate the string...
        if (Len)
        {
            AllocBuffer(Len);
            wcsncpy(m_pchData, lpsz, Len);
        }
    }
}

/*
 * @implemented
 */
CHString::CHString(LPCWSTR lpch, int nLength) throw (CHeap_Exception)
{
    // Allow null initialize, in case something goes wrong
    m_pchData = afxPchNil;

    // In case we have a string with a len
    if (lpch != 0 && nLength != 0)
    {
        // Just copy the string
        AllocBuffer(nLength);
        wcsncpy(m_pchData, lpch, nLength);
    }
}

/*
 * @implemented
 */
CHString::CHString(LPCSTR lpsz) throw (CHeap_Exception)
{
    // Allow null initialize, in case something goes wrong
    m_pchData = afxPchNil;

    // If we have input string
    if (lpsz != 0)
    {
        // Get its length
        int Len = strlen(lpsz);
        if (Len)
        {
            // Allocate and convert the string
            AllocBuffer(Len);
            mbstowcsz(m_pchData, lpsz, Len + 1);
            // Releasing buffer here is to allow to
            // update the buffer size. We notify we're
            // done with changing the string: recompute its
            // length, please
            ReleaseBuffer();
        }
    }
}

/*
 * @implemented
 */
CHString::CHString(const unsigned char* lpsz)
{
    // Null init
    Init();
    // And call operator= with const char*, easier
    *this = (LPCSTR)lpsz;
}

/*
 * @implemented
 */
CHString::CHString(const CHString& stringSrc)
{
    // If we have currently no referenced string
    if (stringSrc.GetData()->nRefs < 0)
    {
        // Ensure we have the null string
        m_pchData = afxPchNil;
        // And then call, the copy operator with input string
        *this = stringSrc.m_pchData;
    }
    else
    {
        // Otherwise, just copy the input string
        m_pchData = stringSrc.m_pchData;
        // And increment the number of references
        InterlockedIncrement(&GetData()->nRefs);
        // The whole point here is: Am I forget to release the old
        // data?! MS doesn't release it, but I guess we should...
    }
}

/*
 * @implemented
 */
CHString::~CHString()
{
    // If we have a string
    if (GetData() != &afxNullData)
    {
        // Check whether it's still in use after we release it
        if (InterlockedDecrement(&GetData()->nRefs) == 0)
        {
            // If so, delete it
            delete GetData();
        }
    }
}

/*
 * @implemented
 */
void CHString::AllocBeforeWrite(int nLen) throw (CHeap_Exception)
{
    // In case we have several strings pointing to our memory zone
    // Or we need bigger buffer than actual
    if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
    {
        // Just drop current
        // And allocate a new one which is big enough
        Release();
        AllocBuffer(nLen);
    }
}

/*
 * @implemented
 */
void CHString::AllocBuffer(int nSize) throw (CHeap_Exception)
{
    // Here we have to allocate a buffer for the string
    // It actually consists in: CHStringData structure
    // with a buffer big enough at its end to store the
    // string.
    CHStringData* Data;

    // Null size is easy allocation
    if (nSize == 0)
    {
        m_pchData = afxPchNil;
        return;
    }

    // We cannot allow negative sizes
    if (nSize < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // Nor too big
    if (nSize > INT_MAX)
    {
        RaiseException(STATUS_INTEGER_OVERFLOW, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // Just allocate big enough buffer, using our own operator new
    Data = (CHStringData *)operator new(nSize * sizeof(WCHAR) + sizeof(CHStringData));
    // In case Data is null, throw an exception
    // Yes, this is stupid! Our operator new is already supposed to through an exception...
    // Thanks MS
    if (!Data)
    {
        throw HeapException;
    }

    Data->nRefs = 1;
    Data->nDataLength = nSize;
    Data->nAllocLength = nSize;
    Data->data()[0] = 0;

    // We only return the string
    // We can find back data with some mathematics
    m_pchData = Data->data();
}

/*
 * @implemented
 */
void CHString::AllocCopy(CHString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const throw (CHeap_Exception)
{
    // Once again, we cannot deal with negative lens
    if (nCopyLen < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    if (nCopyIndex < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    if (nExtraLen < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // In case what we have to copy is null-sized, just set empty string
    if (nCopyLen + nExtraLen == 0)
    {
        dest.m_pchData = afxPchNil;
        return;
    }

    // Otherwise, allocate a buffer in new string which is big enough
    // You can note that we absolutely don't check about any existing
    // (referenced) buffer in dest. Actually, dest is to be EMPTY string.
    // The whole point of this function is to initialize a virgin string by
    // copying data from another. This is needed by Left/Mid/Right
    dest.AllocBuffer(nCopyLen + nExtraLen);
    // And copy our stuff in
    wcsncpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen);
}

/*
 * @implemented
 */
BSTR CHString::AllocSysString() const throw (CHeap_Exception)
{
    BSTR SysString;

    // Just allocate the string
    SysString = SysAllocStringLen(m_pchData, GetData()->nDataLength);
    if (!SysString)
    {
        throw HeapException;
    }

    return SysString;
}

/*
 * @implemented
 */
void CHString::AssignCopy(int nSrcLen, LPCWSTR lpszSrcData) throw (CHeap_Exception)
{
    // Don't allow negative len
    if (nSrcLen < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // We will have to modify a string that might be shared, so duplicate it
    // Ensuring it's big enough to contain our new stuff
    AllocBeforeWrite(nSrcLen);
    if (nSrcLen == 0)
    {
        Release();
        return;
    }

    // Just copy, write down new size, and ensure it's null terminated
    wcsncpy(m_pchData, lpszSrcData, nSrcLen);
    GetData()->nDataLength = nSrcLen;
    m_pchData[nSrcLen] = 0;
}

/*
 * @implemented
 */
int CHString::Collate(LPCWSTR lpsz) const
{
    // Just call the deprecated function here - no matter we are null terminated
    // Did you read my statement about how safe is this implementation?
    return wcscoll(m_pchData, lpsz);
}

/*
 * @implemented
 */
int CHString::Compare(LPCWSTR lpsz) const
{
    // Just call the deprecated function here - no matter we are null terminated
    // Did you read my statement about how safe is this implementation?
    return wcscmp(m_pchData, lpsz);
}

/*
 * @implemented
 */
int CHString::CompareNoCase(LPCWSTR lpsz) const
{
    // Just call the deprecated function here - no matter we are null terminated
    // Did you read my statement about how safe is this implementation?
    return wcsicmp(m_pchData, lpsz);
}

/*
 * @implemented
 */
void CHString::ConcatInPlace(int nSrcLen, LPCWSTR lpszSrcData)
{
    // With null length, there's not that much to concat...
    if (nSrcLen == 0)
    {
        return;
    }

    // Still no negative length
    if (nSrcLen < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // Ensure we wouldn't overflow with the concat
    if (GetData()->nDataLength + nSrcLen > INT_MAX)
    {
        RaiseException(STATUS_INTEGER_OVERFLOW, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // In case we have to modify a shared string OR if it can't fit into current buffer...
    if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
    {
        // Allocate a new buffer! (without forgetting to release old one)
        CHStringData* OldData = GetData();

        // You remember about "InPlace" in the function's name?
        // The cake is a lie
        ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
        Release(OldData);
    }
    else
    {
        // Ensure we don't overflow
        if (nSrcLen > INT_MAX)
        {
            RaiseException(STATUS_INTEGER_OVERFLOW, EXCEPTION_NONCONTINUABLE, 0, 0);
        }

        // Then, just copy and null terminate
        wcsncpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen);
        GetData()->nDataLength += nSrcLen;
        m_pchData[GetData()->nDataLength] = 0;
    }
}

/*
 * @implemented
 */
void CHString::ConcatCopy(int nSrc1Len, LPCWSTR lpszSrc1Data, int nSrc2Len, LPCWSTR lpszSrc2Data) throw (CHeap_Exception)
{
    int TotalLen;

    if (nSrc1Len < 0 || nSrc2Len < 0)
    {
        RaiseException(ERROR_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, 0);
    }

    // If both len are null, do nothing
    TotalLen = nSrc1Len + nSrc2Len;
    if (TotalLen == 0)
    {
        return;
    }

    // Otherwise, allocate a new buffer to hold everything (caller will release previous buffer)
    AllocBuffer(TotalLen);
    // And concat stuff
    wcsncpy(m_pchData, lpszSrc1Data, nSrc1Len);
    wcsncpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len);
}

/*
 * @implemented
 */
void CHString::CopyBeforeWrite() throw (CHeap_Exception)
{
    CHStringData* Data;

    // First, we need to get reference count
    // And we also need to save Data for later copy
    Data = GetData();

    if (Data->nRefs <= 1)
    {
        // If its not used, don't waste time to realloc, it will do the job
        return;
    }

    // Release current data - we are sure it won't be freed upon that point
    // Thanks to the reference count check previously done
    Release();
    // Alloc new buffer and copy old data in it
    AllocBuffer(Data->nDataLength);
    wcsncpy(m_pchData, Data->data(), Data->nDataLength);
}

/*
 * @implemented
 */
void CHString::Empty()
{
    // Already empty
    if (GetData()->nDataLength == 0)
    {
        return;
    }

    // Empty it easily given it's reference count
    if (GetData()->nRefs < 0)
    {
        *this = afxPchNil;
    }
    else
    {
        // Otherwise, just release it
        // It will set back this instance to afxPchNil
        // while decreasing reference count
        Release();
    }
}

/*
 * @implemented
 */
int CHString::Find(WCHAR ch) const
{
    WCHAR *Found;

    // Let's use appropriate helper
    Found = wcschr(m_pchData, ch);
    // We have to return a position, so compute it
    if (Found)
    {
        return (Found - m_pchData);
    }

    // Otherwise, return no position
    return -1;
}

/*
 * @implemented
 */
int CHString::Find(LPCWSTR lpszSub) const
{
    WCHAR *Found;

    // Let's use appropriate helper
    Found = wcsstr(m_pchData, lpszSub);
    // We have to return a position, so compute it
    if (Found)
    {
        return (Found - m_pchData);
    }

    // Otherwise, return no position
    return -1;
}

/*
 * @implemented
 */
int CHString::FindOneOf(LPCWSTR lpszCharSet) const
{
    WCHAR *Found;

    // Let's use appropriate helper
    Found = wcspbrk(m_pchData, lpszCharSet);
    // We have to return a position, so compute it
    if (Found)
    {
        return (Found - m_pchData);
    }

    // Otherwise, return no position
    return -1;
}

/*
 * @implemented
 */
void CHString::Format(UINT nFormatID, ...) throw (CHeap_Exception)
{
    // Deprecated and not implemented any longer - well, this is its implementation
    return;
}

/*
 * @implemented
 */
void CHString::Format(LPCWSTR lpszFormat, ...) throw (CHeap_Exception)
{
    // Forward to FormatV
    va_list ArgsList;

    va_start(ArgsList, lpszFormat);
    FormatV(lpszFormat, ArgsList);
    va_end(ArgsList);
}

/*
 * @implemented
 */
void CHString::FormatMessageW(UINT nFormatID, ...) throw (CHeap_Exception)
{
    // Deprecated and not implemented any longer - well, this is its implementation
    return;
}

/*
 * @unimplemented
 */
void CHString::FormatMessageW(LPCWSTR lpszFormat, ...) throw (CHeap_Exception)
{
    UNIMPLEMENTED;
}

/*
 * @unimplemented
 */
void CHString::FormatV(LPCWSTR lpszFormat, va_list argList)
{
    UNIMPLEMENTED;
}

/*
 * @implemented
 */
void CHString::FreeExtra() throw (CHeap_Exception)
{
    CHStringData* OldData;

    // No extra? Do nothing
    if (GetData()->nDataLength == GetData()->nAllocLength)
    {
        return;
    }

    // Get old buffer
    OldData = GetData();
    // Allocate a new one, at the right size (with no place for \0 :-))
    AllocBuffer(GetData()->nDataLength);
    // Copy old and release it
    wcsncpy(m_pchData, OldData->data(), OldData->nDataLength);
    Release(OldData);
}

/*
 * @implemented
 */
int CHString::GetAllocLength() const
{
    return GetData()->nAllocLength;
}

/*
 * @implemented
 */
WCHAR CHString::GetAt(int nIndex) const
{
    // It's up to you to check the index!
    return m_pchData[nIndex];
}

/*
 * @implemented
 */
LPWSTR CHString::GetBuffer(int nMinBufLength) throw (CHeap_Exception)
{
    LPWSTR OldBuffer = m_pchData;

    // We'll have to allocate a new buffer if it's not big enough
    // or if it's shared by several strings
    if (GetData()->nRefs > 1 || GetData()->nAllocLength < nMinBufLength)
    {
        CHStringData* OldData = GetData();
        int OldLen = GetData()->nDataLength;

        // Ensure we can hold enough
        if (OldLen > nMinBufLength)
        {
            nMinBufLength = OldLen;
        }

        // Allocate new buffer
        AllocBuffer(nMinBufLength);
        // Copy contents
        wcsncpy(m_pchData, OldBuffer, OldLen);
        GetData()->nDataLength = OldLen;

        // Release old
        Release(OldData);
    }

    // Weirdly, here Windows always returns the old buffer
    // Which basically exposes a wrong buffer
    return OldBuffer;
}

/*
 * @implemented
 */
LPWSTR CHString::GetBufferSetLength(int nNewLength) throw (CHeap_Exception)
{
    // Get a buffer big enough
    // We don't care about the return, it will be set in the string
    (void)GetBuffer(nNewLength);
    // Set length, null-terminate and return
    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = 0;
    return m_pchData;
}

/*
 * @implemented
 */
CHStringData* CHString::GetData() const
{
    // In case of empty string, return empty data
    if (m_pchData == afxPchNil)
    {
        return &afxNullData;
    }

    // Otherwise, do maths
    return (CHStringData*)((ULONG_PTR)m_pchData - sizeof(CHStringData));
}

/*
 * @implemented
 */
int CHString::GetLength() const
{
    return GetData()->nDataLength;
}

/*
 * @implemented
 */
void CHString::Init()
{
    m_pchData = afxPchNil;
}

/*
 * @implemented
 */
BOOL CHString::IsEmpty() const
{
    return (GetData()->nDataLength == 0);
}

/*
 * @implemented
 */
CHString CHString::Left(int nCount) const throw (CHeap_Exception)
{
    CHString NewString;

    // Validate input (we can't get more than what we have ;-))
    if (nCount)
    {
        if (nCount > GetData()->nDataLength)
        {
            nCount = GetData()->nDataLength;
        }
    }

    AllocCopy(NewString, nCount, 0, 0);

    return NewString;
}

/*
 * @implemented
 */
int CHString::LoadStringW(UINT nID) throw (CHeap_Exception)
{
    // Deprecated and not implemented any longer - well, this is its implementation
    return 0;
}

/*
 * @implemented
 */
int CHString::LoadStringW(UINT nID, LPWSTR lpszBuf, UINT nMaxBuf) throw (CHeap_Exception)
{
    // Deprecated and not implemented any longer - well, this is its implementation
    return 0;
}

/*
 * @implemented
 */
LPWSTR CHString::LockBuffer()
{
    LPWSTR LockedBuffer;

    // The purpose here is basically to set the nRefs to max int
    LockedBuffer = GetBuffer(0);
    GetData()->nRefs = INT_MAX;

    return LockedBuffer;
}

/*
 * @implemented
 */
void CHString::MakeLower() throw (CHeap_Exception)
{
    // We'll modify string, duplicate it first if needed
    CopyBeforeWrite();

    // Let's use appropriate helper
    _wcslwr(m_pchData);
}

/*
 * @implemented
 */
void CHString::MakeReverse() throw (CHeap_Exception)
{
    // We'll modify string, duplicate it first if needed
    CopyBeforeWrite();

    // Let's use appropriate helper
    _wcsrev(m_pchData);
}

/*
 * @implemented
 */
void CHString::MakeUpper() throw (CHeap_Exception)
{
    // We'll modify string, duplicate it first if needed
    CopyBeforeWrite();

    // Let's use appropriate helper
    _wcsupr(m_pchData);
}

/*
 * @implemented
 */
CHString CHString::Mid(int nFirst) const throw (CHeap_Exception)
{
    // Take string from nFirst up to the end
    return Mid(nFirst, GetData()->nDataLength - nFirst);
}

/*
 * @implemented
 */
CHString CHString::Mid(int nFirst, int nCount) const throw (CHeap_Exception)
{
    CHString NewString;

    // Validate sizes first
    if (nFirst < 0)
    {
        nFirst = 0;
    }

    if (nCount < 0)
    {
        nCount = 0;
    }

    // Ensure we don't go beyond the string
    if (nFirst + nCount > GetData()->nDataLength)
    {
        nCount = GetData()->nDataLength - nFirst;
    }

    // Also ensure we don't read beyond
    // Yes, this should have been done before previous check
    // MS does it that way
    if (nFirst > GetData()->nDataLength)
    {
        nCount = 0;
    }

    AllocCopy(NewString, nCount, nFirst, 0);

    return NewString;
}

/*
 * @implemented
 */
void CHString::Release()
{
    // If null string, nothing to do
    if (GetData() == &afxNullData)
    {
        return;
    }

    // Otherwise, decrement ref count and release if required
    if (InterlockedDecrement(&GetData()->nRefs) == 0)
    {
        delete GetData();
    }

    // In all cases, caller doesn't want string anymore
    // So, switch back to empty string
    m_pchData = afxPchNil;
}

/*
 * @implemented
 */
void WINAPI CHString::Release(CHStringData* pData)
{
    // If empty string, ignore
    if (pData == &afxNullData)
    {
        return;
    }

    // Otherwise, simply and free if needed
    if (InterlockedDecrement(&pData->nRefs) == 0)
    {
        delete pData;
    }
}

/*
 * @implemented
 */
void CHString::ReleaseBuffer(int nNewLength) throw (CHeap_Exception)
{
    CHStringData* Data;

    // We'll modify buffer, so duplicate
    CopyBeforeWrite();

    // If no len provided, get one
    if (nNewLength == -1)
    {
        nNewLength = wcslen(m_pchData);
    }

    // Set appropriate size and null-terminate
    Data = GetData();
    Data->nDataLength = nNewLength;
    Data->data()[nNewLength] = 0;
}

/*
 * @implemented
 */
int CHString::ReverseFind(WCHAR ch) const
{
    WCHAR *Last;

    // Let's use appropriate helper
    Last = wcsrchr(m_pchData, ch);
    // We have to return a position, so compute it
    if (Last)
    {
        return (Last - m_pchData);
    }

    // Otherwise, return no position
    return -1;
}

/*
 * @implemented
 */
CHString CHString::Right(int nCount) const throw (CHeap_Exception)
{
    CHString NewString;

    // Validate input (we can't get more than what we have ;-))
    if (nCount >= 0)
    {
        if (nCount > GetData()->nDataLength)
        {
            nCount = GetData()->nDataLength;
        }
    }

    AllocCopy(NewString, nCount, GetData()->nDataLength - nCount, 0);

    return NewString;
}

/*
 * @implemented
 */
int CHString::SafeStrlen(LPCWSTR lpsz)
{
    // Check we have a string and then get its length
    if (lpsz == 0)
    {
        return 0;
    }

    // Of course, it's not safe at all in case string is not null-terminated.
    // Things that may happen given strings are not to be null-terminated
    // in this class...
    return (int)wcslen(lpsz);
}

/*
 * @implemented
 */
void CHString::SetAt(int nIndex, WCHAR ch) throw (CHeap_Exception)
{
    CopyBeforeWrite();

    m_pchData[nIndex] = ch;
}

/*
 * @implemented
 */
CHString CHString::SpanExcluding(LPCWSTR lpszCharSet) const throw (CHeap_Exception)
{
    int Count;

    // Get position and then, extract
    Count = wcscspn(m_pchData, lpszCharSet);
    return Left(Count);
}

/*
 * @implemented
 */
CHString CHString::SpanIncluding(LPCWSTR lpszCharSet) const throw (CHeap_Exception)
{
    int Count;

    // Get position and then, extract
    Count = wcsspn(m_pchData, lpszCharSet);
    return Left(Count);
}

/*
 * @implemented
 */
void CHString::TrimLeft() throw (CHeap_Exception)
{
    int NewBegin;
    int NewLength;
    WCHAR *CurrentChar;

    // We'll modify, so copy first
    CopyBeforeWrite();

    // Start at the begin of the string
    CurrentChar = m_pchData;
    while (*CurrentChar != 0)
    {
        // Browse string till we find something which is not a space
        if (!iswspace(*CurrentChar))
        {
            break;
        }

        CurrentChar++;
    }

    // Then, calculate new begin (easy) and new length
    // And move memory
    NewBegin = (CurrentChar - m_pchData);
    NewLength = GetData()->nDataLength - NewBegin;
    memmove(m_pchData, CurrentChar, NewLength * sizeof(WCHAR));
    GetData()->nDataLength = NewLength;
}

/*
 * @implemented
 */
void CHString::TrimRight() throw (CHeap_Exception)
{
    WCHAR *CurrentChar;
    WCHAR *CanBeEaten;

    // We'll modify, so copy first
    CopyBeforeWrite();

    // Start at the begin of the string -- WHAT?!
    // Yes, this algorithm is the same that MS is
    // using for its TrimRight.
    // It is highly unefficient. It would have been
    // easier to start at nDataLength and to get back to
    // the begin. Note that it would have been safer as
    // well, in case the caller is using non-null-terminated
    // strings. But, well...
    CurrentChar = m_pchData;
    CanBeEaten = 0;
    while (*CurrentChar != 0)
    {
        // If not a space, reset what we can trim
        if (!iswspace(*CurrentChar))
        {
            CanBeEaten = 0;
        }
        // If it is one, and the first of the spaces serie
        // Keep its position
        else if (CanBeEaten == 0)
        {
            CanBeEaten = CurrentChar;
        }

        CurrentChar++;
    }

    // If nothing to trim, quit
    if (CanBeEaten == 0)
    {
        return;
    }

    // Otherwise, shorten the string
    GetData()->nDataLength = (CanBeEaten - m_pchData);
}

/*
 * @implemented
 */
void CHString::UnlockBuffer()
{
    // Unlock means just put ref back to 1
    // It was previously set to MAX_INT
    if (GetData() != &afxNullData)
    {
        GetData()->nRefs = 1;
    }
}

/*
 * @implemented
 */
const CHString& CHString::operator=(char ch) throw (CHeap_Exception)
{
    *this = (WCHAR)ch;
    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator=(WCHAR ch) throw (CHeap_Exception)
{
    AssignCopy(1, &ch);
    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator=(CHString *p) throw (CHeap_Exception)
{
    *this = *p;
    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator=(LPCSTR lpsz) throw (CHeap_Exception)
{
    int Len;

    // If we have string, get its len
    if (lpsz != 0)
    {
        Len = strlen(lpsz);
    }
    else
    {
        Len = 0;
    }

    // Do this call, even with null len, just to get empty string
    AllocBeforeWrite(Len);
    if (Len == 0)
    {
        Release();
        return *this;
    }

    // Convert and copy
    mbstowcsz(m_pchData, lpsz, Len + 1);
    // Get new size and so on
    ReleaseBuffer();

    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator=(LPCWSTR lpsz) throw (CHeap_Exception)
{
    int Len;

    Len = SafeStrlen(lpsz);
    AssignCopy(Len, lpsz);

    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator=(const CHString& stringSrc) throw (CHeap_Exception)
{
    // Don't copy string on itself
    if (&stringSrc == this)
    {
        return *this;
    }

    // In case we don't have a referenced string here,
    // or if the other is not referenced, just copy here
    if ((GetData()->nRefs < 0 && GetData() != &afxNullData) ||
         stringSrc.GetData()->nRefs < 0)
    {
        AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
        return *this;
    }

    // Otherwise, release current buffer
    Release();
    // And set buffer as stringSrc buffer
    // And increase its reference count
    m_pchData = stringSrc.m_pchData;
    InterlockedIncrement(&GetData()->nRefs);

    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator=(const unsigned char* lpsz) throw (CHeap_Exception)
{
    *this = (LPCSTR)lpsz;
    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator+=(char ch) throw (CHeap_Exception)
{
    *this += (WCHAR)ch;
    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator+=(WCHAR ch) throw (CHeap_Exception)
{
    ConcatInPlace(1, &ch);
    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator+=(LPCWSTR lpsz) throw (CHeap_Exception)
{
    int Len;

    Len = SafeStrlen(lpsz);
    ConcatInPlace(Len, lpsz);

    return *this;
}

/*
 * @implemented
 */
const CHString& CHString::operator+=(const CHString& string) throw (CHeap_Exception)
{
    ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);

    return *this;
}

/*
 * @implemented
 */
WCHAR CHString::operator[](int nIndex) const
{
    return m_pchData[nIndex];
}

/*
 * @implemented
 */
CHString::operator LPCWSTR() const
{
    return m_pchData;
}

/*
 * @implemented
 */
CHString WINAPI operator+(WCHAR ch, const CHString& string) throw (CHeap_Exception)
{
    CHString NewString;

    // Basically concat in a new string
    NewString.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pchData);

    return NewString;
}

/*
 * @implemented
 */
CHString WINAPI operator+(const CHString& string, WCHAR ch) throw (CHeap_Exception)
{
    CHString NewString;

    // Basically concat in a new string
    NewString.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, 1, &ch);

    return NewString;
}

/*
 * @implemented
 */
CHString WINAPI operator+(const CHString& string, LPCWSTR lpsz) throw (CHeap_Exception)
{
    int Len;
    CHString NewString;

    // Get string length
    Len = CHString::SafeStrlen(lpsz);
    // And concat in new string
    NewString.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, Len, lpsz);

    return NewString;
}

/*
 * @implemented
 */
CHString WINAPI operator+(LPCWSTR lpsz, const CHString& string) throw (CHeap_Exception)
{
    int Len;
    CHString NewString;

    // Get string length
    Len = CHString::SafeStrlen(lpsz);
    // And concat in new string
    NewString.ConcatCopy(Len, lpsz, string.GetData()->nDataLength, string.m_pchData);

    return NewString;
}

/*
 * @implemented
 */
CHString WINAPI operator+(const CHString& string1, const CHString& string2) throw (CHeap_Exception)
{
    CHString NewString;

    // Basically concat in a new string
    NewString.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData,
                         string2.GetData()->nDataLength, string2.m_pchData);

    return NewString;
}
