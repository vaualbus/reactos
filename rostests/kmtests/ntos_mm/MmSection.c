/*
 * PROJECT:         ReactOS kernel-mode tests
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         Kernel-Mode Test Suite Section Object test
 * PROGRAMMER:      Thomas Faber <thomas.faber@reactos.org>
 */

#include <kmt_test.h>

#define CheckObject(Handle, Pointers, Handles) do                   \
{                                                                   \
    PUBLIC_OBJECT_BASIC_INFORMATION ObjectInfo;                     \
    Status = ZwQueryObject(Handle, ObjectBasicInformation,          \
                            &ObjectInfo, sizeof ObjectInfo, NULL);  \
    ok_eq_hex(Status, STATUS_SUCCESS);                              \
    ok_eq_ulong(ObjectInfo.PointerCount, Pointers);                 \
    ok_eq_ulong(ObjectInfo.HandleCount, Handles);                   \
} while (0)

#define CheckSection(SectionObject, SectionFlag) do                     \
{                                                                       \
    SECTION_BASIC_INFORMATION Sbi;                                      \
    HANDLE SectionHandle = NULL;                                        \
    NTSTATUS Status;                                                    \
    if (skip(SectionObject != NULL &&                                   \
             SectionObject != (PVOID)0x5555555555555555ULL,             \
             "No section object\n"))                                    \
        break;                                                          \
    Status = ObOpenObjectByPointer(SectionObject, OBJ_KERNEL_HANDLE,    \
                                   NULL, 0, MmSectionObjectType,        \
                                   KernelMode, &SectionHandle);         \
    ok_eq_hex(Status, STATUS_SUCCESS);                                  \
    ok(SectionHandle != NULL, "Section handle null\n");                 \
    if (!skip(NT_SUCCESS(Status) && SectionHandle,                      \
              "No section handle\n"))                                   \
    {                                                                   \
        Status = ZwQuerySection(SectionHandle, SectionBasicInformation, \
                                &Sbi, sizeof Sbi, NULL);                \
        ok_eq_hex(Status, STATUS_SUCCESS);                              \
        ok_eq_pointer(Sbi.BaseAddress, NULL);                           \
        ok_eq_longlong(Sbi.Size.QuadPart, 1LL);                         \
        ok_eq_hex(Sbi.Attributes, SectionFlag | SEC_FILE);              \
        ZwClose(SectionHandle);                                         \
    }                                                                   \
} while (0)

#define TestMapView(SectionObject, ExpectAtBase, ExpectM) do                    \
{                                                                               \
    NTSTATUS Status;                                                            \
    PVOID BaseAddress = NULL;                                                   \
    SIZE_T ViewSize = 0;                                                        \
    LARGE_INTEGER SectionOffset;                                                \
    if (skip(SectionObject != NULL &&                                           \
             SectionObject != (PVOID)0x5555555555555555ULL,                     \
             "No section object\n"))                                            \
        break;                                                                  \
                                                                                \
    SectionOffset.QuadPart = 0;                                                 \
    Status = MmMapViewOfSection(SectionObject, PsGetCurrentProcess(),           \
                                &BaseAddress, 0, 1, &SectionOffset,             \
                                &ViewSize, ViewUnmap, 0, PAGE_READONLY);        \
    ok_eq_hex(Status, ExpectAtBase ? STATUS_SUCCESS : STATUS_IMAGE_NOT_AT_BASE);\
    if (!skip(NT_SUCCESS(Status), "Section not mapped\n"))                      \
    {                                                                           \
        ok_eq_uint(*(PUCHAR)BaseAddress, ExpectM ? 'M' : 0);                    \
        Status = MmUnmapViewOfSection(PsGetCurrentProcess(), BaseAddress);      \
        ok_eq_hex(Status, STATUS_SUCCESS);                                      \
    }                                                                           \
} while (0)

static
VOID
TestCreateSection(
    IN HANDLE FileHandle1,
    IN PFILE_OBJECT FileObject1,
    IN HANDLE FileHandle2,
    IN PFILE_OBJECT FileObject2)
{
    NTSTATUS Status = STATUS_SUCCESS;
    PVOID SectionObject;
    LARGE_INTEGER MaximumSize;
    ULONG PointerCount1, PointerCount2;

    KmtStartSeh()
        Status = MmCreateSection(NULL, 0, NULL, NULL, 0, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_SUCCESS);
    ok_eq_hex(Status, STATUS_INVALID_PAGE_PROTECTION);

    if (!KmtIsCheckedBuild)
    {
        /* PAGE_NOACCESS and missing SEC_RESERVE/SEC_COMMIT/SEC_IMAGE assert */
        KmtStartSeh()
            Status = MmCreateSection(NULL, 0, NULL, NULL, PAGE_NOACCESS, SEC_RESERVE, NULL, NULL);
        KmtEndSeh(STATUS_ACCESS_VIOLATION);

        KmtStartSeh()
            Status = MmCreateSection(NULL, 0, NULL, NULL, PAGE_NOACCESS, 0, NULL, NULL);
        KmtEndSeh(STATUS_ACCESS_VIOLATION);
    }

    SectionObject = KmtInvalidPointer;
    KmtStartSeh()
        Status = MmCreateSection(&SectionObject, 0, NULL, NULL, 0, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_SUCCESS);
    ok_eq_hex(Status, STATUS_INVALID_PAGE_PROTECTION);
    ok_eq_pointer(SectionObject, KmtInvalidPointer);

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    KmtStartSeh()
        Status = MmCreateSection(NULL, 0, NULL, NULL, PAGE_READONLY, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_ACCESS_VIOLATION);

    SectionObject = KmtInvalidPointer;
    KmtStartSeh()
        Status = MmCreateSection(&SectionObject, 0, NULL, NULL, PAGE_READONLY, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_ACCESS_VIOLATION);
    ok_eq_pointer(SectionObject, KmtInvalidPointer);

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    SectionObject = KmtInvalidPointer;
    MaximumSize.QuadPart = 0;
    KmtStartSeh()
        Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, NULL, NULL);
    KmtEndSeh(STATUS_SUCCESS);
    ok_eq_hex(Status, STATUS_INVALID_FILE_FOR_SECTION);
    ok_eq_longlong(MaximumSize.QuadPart, 0LL);
    ok_eq_pointer(SectionObject, KmtInvalidPointer);

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    MaximumSize.QuadPart = 0;
    KmtStartSeh()
        Status = MmCreateSection(NULL, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_SUCCESS);
    ok_eq_hex(Status, STATUS_INVALID_PARAMETER_4);
    ok_eq_longlong(MaximumSize.QuadPart, 0LL);

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    MaximumSize.QuadPart = 1;
    KmtStartSeh()
        Status = MmCreateSection(NULL, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_ACCESS_VIOLATION);
    ok_eq_longlong(MaximumSize.QuadPart, 1LL);

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    SectionObject = KmtInvalidPointer;
    MaximumSize.QuadPart = 0;
    KmtStartSeh()
        Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_SUCCESS);
    ok_eq_hex(Status, STATUS_INVALID_PARAMETER_4);
    ok_eq_longlong(MaximumSize.QuadPart, 0LL);
    ok_eq_pointer(SectionObject, KmtInvalidPointer);

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    /* page file section */
    SectionObject = KmtInvalidPointer;
    MaximumSize.QuadPart = 1;
    KmtStartSeh()
        Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, NULL, NULL);
    KmtEndSeh(STATUS_SUCCESS);
    ok_eq_hex(Status, STATUS_SUCCESS);
    ok_eq_longlong(MaximumSize.QuadPart, 1LL);
    ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
    ok(SectionObject != NULL, "Section object pointer NULL\n");

    if (SectionObject && SectionObject != KmtInvalidPointer)
        ObDereferenceObject(SectionObject);

    if (!skip(FileHandle1 != NULL && FileObject1 != NULL &&
              FileHandle2 != NULL && FileObject2 != NULL, "No file handle or object\n"))
    {
        PointerCount1 = 3;
        PointerCount2 = 3;
        /* image section */
        CheckObject(FileHandle2, PointerCount2, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, FileHandle2, NULL);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle2, PointerCount2, 1L);
        CheckSection(SectionObject, SEC_IMAGE);
        TestMapView(SectionObject, FALSE, TRUE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        CheckObject(FileHandle2, PointerCount2, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, NULL, FileObject2);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        ++PointerCount2;
        CheckObject(FileHandle2, PointerCount2, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, TRUE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);
        //--PointerCount2;  // ????

        CheckObject(FileHandle2, PointerCount2, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, FileHandle2, FileObject2);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle2, PointerCount2, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, TRUE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        /* image section with inappropriate file */
        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, FileHandle1, NULL);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_INVALID_IMAGE_NOT_MZ);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok_eq_pointer(SectionObject, KmtInvalidPointer);
        CheckObject(FileHandle1, PointerCount1, 1L);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, NULL, FileObject1);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        ++PointerCount1;
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, FALSE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);
        //--PointerCount1; // ????

        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, FileHandle1, FileObject1);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, FALSE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        /* image section with two different files */
        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, FileHandle1, FileObject2);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckObject(FileHandle2, PointerCount2, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, TRUE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_IMAGE, FileHandle2, FileObject1);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckObject(FileHandle2, PointerCount2, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, FALSE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        /* data file section */
        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, FileHandle1, NULL);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, FALSE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, NULL, FileObject1);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, FALSE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        CheckObject(FileHandle1, PointerCount1, 1L);
        SectionObject = KmtInvalidPointer;
        MaximumSize.QuadPart = 1;
        KmtStartSeh()
            Status = MmCreateSection(&SectionObject, 0, NULL, &MaximumSize, PAGE_READONLY, SEC_RESERVE, FileHandle1, FileObject1);
        KmtEndSeh(STATUS_SUCCESS);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_longlong(MaximumSize.QuadPart, 1LL);
        ok(SectionObject != KmtInvalidPointer, "Section object pointer untouched\n");
        ok(SectionObject != NULL, "Section object pointer NULL\n");
        CheckObject(FileHandle1, PointerCount1, 1L);
        CheckSection(SectionObject, 0);
        TestMapView(SectionObject, TRUE, FALSE);

        if (SectionObject && SectionObject != KmtInvalidPointer)
            ObDereferenceObject(SectionObject);

        CheckObject(FileHandle1, PointerCount1, 1L);
    }
}

START_TEST(MmSection)
{
    NTSTATUS Status;
    HANDLE FileHandle1 = NULL, FileHandle2 = NULL;
    PFILE_OBJECT FileObject1 = NULL, FileObject2 = NULL;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING FileName1 = RTL_CONSTANT_STRING(L"\\SystemRoot\\kmtest-MmSection.txt");
    UNICODE_STRING FileName2 = RTL_CONSTANT_STRING(L"\\SystemRoot\\system32\\ntdll.dll");
    LARGE_INTEGER FileOffset;
    UCHAR FileData = 0;

    ok(ExGetPreviousMode() == UserMode, "Previous mode is kernel mode\n");
    /* create a one-byte file that we can use */
    InitializeObjectAttributes(&ObjectAttributes, &FileName1, OBJ_CASE_INSENSITIVE, NULL, NULL);
    Status = ZwCreateFile(&FileHandle1, GENERIC_WRITE | SYNCHRONIZE, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_SUPERSEDE, FILE_NON_DIRECTORY_FILE, NULL, 0);
    ok_eq_hex(Status, STATUS_SUCCESS);
    ok_eq_ulongptr(IoStatusBlock.Information, FILE_CREATED);
    ok(FileHandle1 != NULL, "FileHandle1 is NULL\n");
    if (FileHandle1)
    {
        FileOffset.QuadPart = 0;
        Status = ZwWriteFile(FileHandle1, NULL, NULL, NULL, &IoStatusBlock, &FileData, sizeof FileData, &FileOffset, NULL);
        ok(Status == STATUS_SUCCESS || Status == STATUS_PENDING, "Status = 0x%08lx\n", Status);
        Status = ZwWaitForSingleObject(FileHandle1, FALSE, NULL);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok_eq_ulongptr(IoStatusBlock.Information, 1);
        Status = ZwClose(FileHandle1);
        ok_eq_hex(Status, STATUS_SUCCESS);
        FileHandle1 = NULL;
    }

    InitializeObjectAttributes(&ObjectAttributes, &FileName1, OBJ_CASE_INSENSITIVE, NULL, NULL);
    Status = ZwCreateFile(&FileHandle1, GENERIC_ALL, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_DELETE_ON_CLOSE, NULL, 0);
    ok_eq_hex(Status, STATUS_SUCCESS);
    ok_eq_ulongptr(IoStatusBlock.Information, FILE_OPENED);
    ok(FileHandle1 != NULL, "FileHandle1 is NULL\n");
    CheckObject(FileHandle1, 2L, 1L);

    InitializeObjectAttributes(&ObjectAttributes, &FileName2, OBJ_CASE_INSENSITIVE, NULL, NULL);
    Status = ZwCreateFile(&FileHandle2, GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE, NULL, 0);
    ok_eq_hex(Status, STATUS_SUCCESS);
    ok_eq_ulongptr(IoStatusBlock.Information, FILE_OPENED);
    ok(FileHandle2 != NULL, "FileHandle2 is NULL\n");

    if (!skip(Status == STATUS_SUCCESS && FileHandle1 != NULL, "Failed to open file 1\n"))
    {
        Status = ObReferenceObjectByHandle(FileHandle1, FILE_READ_DATA | FILE_WRITE_DATA,
#ifdef _PROPER_NT_EXPORTS
                                           *IoFileObjectType,
#else
                                           IoFileObjectType,
#endif
                                           KernelMode, (PVOID *)&FileObject1, NULL);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok(FileObject1 != NULL, "FileObject1 is NULL\n");
        CheckObject(FileHandle1, 3L, 1L);
    }

    if (!skip(Status == STATUS_SUCCESS && FileHandle2 != NULL, "Failed to open file 2\n"))
    {
        Status = ObReferenceObjectByHandle(FileHandle2, FILE_READ_DATA | FILE_WRITE_DATA,
#ifdef _PROPER_NT_EXPORTS
                                           *IoFileObjectType,
#else
                                           IoFileObjectType,
#endif
                                           KernelMode, (PVOID *)&FileObject2, NULL);
        ok_eq_hex(Status, STATUS_SUCCESS);
        ok(FileObject2 != NULL, "FileObject2 is NULL\n");
    }

    trace("FileHandle1=%p, FileObject1=%p\n", FileHandle1, FileObject1);
    trace("FileHandle2=%p, FileObject2=%p\n", FileHandle2, FileObject2);
    TestCreateSection(FileHandle1, FileObject1, FileHandle2, FileObject2);

    if (FileObject2)
        ObDereferenceObject(FileObject2);
    if (FileObject1)
        ObDereferenceObject(FileObject1);
    if (FileHandle2)
        ZwClose(FileHandle2);
    if (FileHandle1)
        ZwClose(FileHandle1);
}
