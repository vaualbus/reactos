#include <ntddk.h>

#include <acpi.h>
#include <acpisys.h>

#include <acpi_bus.h>
#include <acpi_drivers.h>

#include <acpiioct.h>
#include <poclass.h>

#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
DriverEntry (
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath
    );

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, Bus_AddDevice)

#endif

extern struct acpi_device *sleep_button;
extern struct acpi_device *power_button;

UNICODE_STRING ProcessorHardwareIds = {0, 0, NULL};
LPWSTR ProcessorNameString = NULL;


NTSTATUS
NTAPI
Bus_AddDevice(
    PDRIVER_OBJECT DriverObject,
    PDEVICE_OBJECT PhysicalDeviceObject
    )

{
    NTSTATUS            status;
    PDEVICE_OBJECT      deviceObject = NULL;
    PFDO_DEVICE_DATA    deviceData = NULL;
    PWCHAR              deviceName = NULL;
#ifndef NDEBUG
    ULONG               nameLength;
#endif

    PAGED_CODE ();

    DPRINT("Add Device: 0x%p\n",  PhysicalDeviceObject);

    DPRINT("#################### Bus_CreateClose Creating FDO Device ####################\n");
    status = IoCreateDevice(DriverObject,
                      sizeof(FDO_DEVICE_DATA),
                      NULL,
                      FILE_DEVICE_ACPI,
                      FILE_DEVICE_SECURE_OPEN,
                      TRUE,
                      &deviceObject);
    if (!NT_SUCCESS(status))
    {
        DPRINT1("IoCreateDevice() failed with status 0x%X\n", status);
        goto End;
    }

    deviceData = (PFDO_DEVICE_DATA) deviceObject->DeviceExtension;
    RtlZeroMemory (deviceData, sizeof (FDO_DEVICE_DATA));

    //
    // Set the initial state of the FDO
    //

    INITIALIZE_PNP_STATE(deviceData->Common);

    deviceData->Common.IsFDO = TRUE;

    deviceData->Common.Self = deviceObject;

    ExInitializeFastMutex (&deviceData->Mutex);

    InitializeListHead (&deviceData->ListOfPDOs);

    // Set the PDO for use with PlugPlay functions

    deviceData->UnderlyingPDO = PhysicalDeviceObject;

    //
    // Set the initial powerstate of the FDO
    //

    deviceData->Common.DevicePowerState = PowerDeviceUnspecified;
    deviceData->Common.SystemPowerState = PowerSystemWorking;

    deviceObject->Flags |= DO_POWER_PAGABLE;

    //
    // Attach our FDO to the device stack.
    // The return value of IoAttachDeviceToDeviceStack is the top of the
    // attachment chain.  This is where all the IRPs should be routed.
    //

    deviceData->NextLowerDriver = IoAttachDeviceToDeviceStack (
                                    deviceObject,
                                    PhysicalDeviceObject);

    if (NULL == deviceData->NextLowerDriver) {

        status = STATUS_NO_SUCH_DEVICE;
        goto End;
    }


#ifndef NDEBUG
    //
    // We will demonstrate here the step to retrieve the name of the PDO
    //

    status = IoGetDeviceProperty (PhysicalDeviceObject,
                                  DevicePropertyPhysicalDeviceObjectName,
                                  0,
                                  NULL,
                                  &nameLength);

    if (status != STATUS_BUFFER_TOO_SMALL)
    {
        DPRINT1("AddDevice:IoGDP failed (0x%x)\n", status);
        goto End;
    }

    deviceName = ExAllocatePoolWithTag (NonPagedPool,
                            nameLength, 'IPCA');

    if (NULL == deviceName) {
        DPRINT1("AddDevice: no memory to alloc for deviceName(0x%x)\n", nameLength);
        status =  STATUS_INSUFFICIENT_RESOURCES;
        goto End;
    }

    status = IoGetDeviceProperty (PhysicalDeviceObject,
                         DevicePropertyPhysicalDeviceObjectName,
                         nameLength,
                         deviceName,
                         &nameLength);

    if (!NT_SUCCESS (status)) {

        DPRINT1("AddDevice:IoGDP(2) failed (0x%x)", status);
        goto End;
    }

    DPRINT("AddDevice: %p to %p->%p (%ws) \n",
                   deviceObject,
                   deviceData->NextLowerDriver,
                   PhysicalDeviceObject,
                   deviceName);

#endif

    //
    // We are done with initializing, so let's indicate that and return.
    // This should be the final step in the AddDevice process.
    //
    deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

End:
    if (deviceName){
        ExFreePoolWithTag(deviceName, 'IPCA');
    }
    if (!NT_SUCCESS(status) && deviceObject){
        if (deviceData && deviceData->NextLowerDriver){
            IoDetachDevice (deviceData->NextLowerDriver);
        }
        IoDeleteDevice (deviceObject);
    }
    return status;

}

NTSTATUS
NTAPI
ACPIDispatchCreateClose(
   IN PDEVICE_OBJECT DeviceObject,
   IN PIRP Irp)
{
   Irp->IoStatus.Status = STATUS_SUCCESS;
   Irp->IoStatus.Information = 0;

   IoCompleteRequest(Irp, IO_NO_INCREMENT);

   return STATUS_SUCCESS;
}

VOID
NTAPI
ButtonWaitThread(PVOID Context)
{
    PIRP Irp = Context;
    int result;
    struct acpi_bus_event event;
    ULONG ButtonEvent;

    while (ACPI_SUCCESS(result = acpi_bus_receive_event(&event)) &&
           event.type != ACPI_BUTTON_NOTIFY_STATUS);

    if (!ACPI_SUCCESS(result))
    {
       Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
    }
    else
    {
       if (strstr(event.bus_id, "PWRF"))
           ButtonEvent = SYS_BUTTON_POWER;
       else if (strstr(event.bus_id, "SLPF"))
           ButtonEvent = SYS_BUTTON_SLEEP;
       else
           ButtonEvent = 0;

       RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, &ButtonEvent, sizeof(ButtonEvent));
       Irp->IoStatus.Status = STATUS_SUCCESS;
       Irp->IoStatus.Information = sizeof(ULONG);
    }

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
}
    

NTSTATUS
NTAPI
ACPIDispatchDeviceControl(
   IN PDEVICE_OBJECT DeviceObject,
   IN PIRP Irp)
{
    PIO_STACK_LOCATION      irpStack;
    NTSTATUS                status = STATUS_NOT_SUPPORTED;
    PCOMMON_DEVICE_DATA     commonData;
    ULONG Caps = 0;
    HANDLE ThreadHandle;

    PAGED_CODE ();

    irpStack = IoGetCurrentIrpStackLocation (Irp);
    ASSERT (IRP_MJ_DEVICE_CONTROL == irpStack->MajorFunction);

    commonData = (PCOMMON_DEVICE_DATA) DeviceObject->DeviceExtension;

    Irp->IoStatus.Information = 0;

    if (!commonData->IsFDO)
    {
       switch (irpStack->Parameters.DeviceIoControl.IoControlCode)
       {
           case IOCTL_ACPI_EVAL_METHOD:
              status = Bus_PDO_EvalMethod((PPDO_DEVICE_DATA)commonData,
                                          Irp);
              break;

           case IOCTL_GET_SYS_BUTTON_CAPS:
              if (irpStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(ULONG))
              {
                  status = STATUS_BUFFER_TOO_SMALL;
                  break;
              }

              if (wcsstr(((PPDO_DEVICE_DATA)commonData)->HardwareIDs, L"PNP0C0D"))
              {
                  DPRINT1("Lid button reported to power manager\n");
                  Caps |= SYS_BUTTON_LID;
              }
              else if (((PPDO_DEVICE_DATA)commonData)->AcpiHandle == NULL)
              {
                  /* We have to return both at the same time because since we
                   * have a NULL handle we are the fixed feature DO and we will
                   * only be called once (not once per device)
                   */
                  if (power_button)
                  {
                      DPRINT1("Fixed power button reported to power manager\n");
                      Caps |= SYS_BUTTON_POWER;
                  }
                  if (sleep_button)
                  {
                      DPRINT1("Fixed sleep button reported to power manager\n");
                      Caps |= SYS_BUTTON_SLEEP;
                  }
              }
              else if (wcsstr(((PPDO_DEVICE_DATA)commonData)->HardwareIDs, L"PNP0C0C"))
              {
                  DPRINT1("Control method power button reported to power manager\n");
                  Caps |= SYS_BUTTON_POWER;
              }
              else if (wcsstr(((PPDO_DEVICE_DATA)commonData)->HardwareIDs, L"PNP0C0E"))
              {
                  DPRINT1("Control method sleep reported to power manager\n");
                  Caps |= SYS_BUTTON_SLEEP;
              }
              else
              {
                  DPRINT1("IOCTL_GET_SYS_BUTTON_CAPS sent to a non-button device\n");
                  status = STATUS_INVALID_PARAMETER;
              }

              if (Caps != 0)
              {
                  RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, &Caps, sizeof(Caps));
                  Irp->IoStatus.Information = sizeof(Caps);
                  status = STATUS_SUCCESS;
              }
              break;

           case IOCTL_GET_SYS_BUTTON_EVENT:
              PsCreateSystemThread(&ThreadHandle, THREAD_ALL_ACCESS, 0, 0, 0, ButtonWaitThread, Irp);
              ZwClose(ThreadHandle);

              status = STATUS_PENDING;
              break;

           default:
              DPRINT1("Unsupported IOCTL: %x\n", irpStack->Parameters.DeviceIoControl.IoControlCode);
              break;
       }
    }
    else
       DPRINT1("IOCTL sent to the ACPI FDO! Kill the caller!\n");

    if (status != STATUS_PENDING)
    {
       Irp->IoStatus.Status = status;
       IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }
    else
       IoMarkIrpPending(Irp);

    return status;
}

static
NTSTATUS
AcpiRegOpenKey(IN HANDLE ParentKeyHandle,
               IN LPCWSTR KeyName,
               IN ACCESS_MASK DesiredAccess,
               OUT HANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING Name;

    RtlInitUnicodeString(&Name, KeyName);

    InitializeObjectAttributes(&ObjectAttributes,
                               &Name,
                               OBJ_CASE_INSENSITIVE,
                               ParentKeyHandle,
                               NULL);

    return ZwOpenKey(KeyHandle,
                     DesiredAccess,
                     &ObjectAttributes);
}

static
NTSTATUS
AcpiRegQueryValue(IN HANDLE KeyHandle,
                  IN LPWSTR ValueName,
                  OUT PULONG Type OPTIONAL,
                  OUT PVOID Data OPTIONAL,
                  IN OUT PULONG DataLength OPTIONAL)
{
    PKEY_VALUE_PARTIAL_INFORMATION ValueInfo;
    UNICODE_STRING Name;
    ULONG BufferLength = 0;
    NTSTATUS Status;

    RtlInitUnicodeString(&Name,
                         ValueName);

    if (DataLength != NULL)
        BufferLength = *DataLength;

    BufferLength += FIELD_OFFSET(KEY_VALUE_PARTIAL_INFORMATION, Data);

    /* Allocate memory for the value */
    ValueInfo = ExAllocatePoolWithTag(PagedPool, BufferLength, 'IPCA');
    if (ValueInfo == NULL)
        return STATUS_NO_MEMORY;

    /* Query the value */
    Status = ZwQueryValueKey(KeyHandle,
                             &Name,
                             KeyValuePartialInformation,
                             ValueInfo,
                             BufferLength,
                             &BufferLength);
    if ((NT_SUCCESS(Status)) || (Status == STATUS_BUFFER_OVERFLOW))
    {
        if (Type != NULL)
            *Type = ValueInfo->Type;

        if (DataLength != NULL)
            *DataLength = ValueInfo->DataLength;
    }

    /* Check if the caller wanted data back, and we got it */
    if ((NT_SUCCESS(Status)) && (Data != NULL))
    {
        /* Copy it */
        RtlMoveMemory(Data,
                      ValueInfo->Data,
                      ValueInfo->DataLength);

        /* if the type is REG_SZ and data is not 0-terminated
         * and there is enough space in the buffer NT appends a \0 */
        if (((ValueInfo->Type == REG_SZ) || (ValueInfo->Type == REG_EXPAND_SZ) || (ValueInfo->Type == REG_MULTI_SZ)) &&
            (ValueInfo->DataLength <= *DataLength - sizeof(WCHAR)))
        {
            WCHAR *ptr = (WCHAR *)((ULONG_PTR)Data + ValueInfo->DataLength);
            if ((ptr > (WCHAR *)Data) && ptr[-1])
                *ptr = 0;
        }
    }

    /* Free the memory and return status */
    ExFreePoolWithTag(ValueInfo, 'IPCA');

    if ((Data == NULL) && (Status == STATUS_BUFFER_OVERFLOW))
        Status = STATUS_SUCCESS;

    return Status;
}

static
NTSTATUS
GetProcessorInformation(VOID)
{
    LPWSTR ProcessorIdentifier = NULL;
    LPWSTR ProcessorVendorIdentifier = NULL;
    LPWSTR HardwareIdsBuffer = NULL;
    HANDLE ProcessorHandle = NULL;
    ULONG Length, Level1Length = 0, Level2Length = 0, Level3Length = 0;
    ULONG HardwareIdsLength = 0;
    ULONG i;
    PWCHAR Ptr;
    NTSTATUS Status;

    DPRINT1("GetProcessorInformation()\n");

    Status = AcpiRegOpenKey(NULL,
                            L"\\Registry\\Machine\\Hardware\\Description\\System\\CentralProcessor\\0",
                            KEY_READ,
                            &ProcessorHandle);
    if (!NT_SUCCESS(Status))
        goto done;

    AcpiRegQueryValue(ProcessorHandle,
                      L"Identifier",
                      NULL,
                      NULL,
                      &Length);

    if (Length != 0)
    {
        ProcessorIdentifier = ExAllocatePoolWithTag(PagedPool, Length, 'IPCA');
        if (ProcessorIdentifier == NULL)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto done;
        }

        Status = AcpiRegQueryValue(ProcessorHandle,
                                   L"Identifier",
                                   NULL,
                                   ProcessorIdentifier,
                                   &Length);
        if (!NT_SUCCESS(Status))
            goto done;

        Length = 0;
    }

    AcpiRegQueryValue(ProcessorHandle,
                      L"ProcessorNameString",
                      NULL,
                      NULL,
                      &Length);

    if (Length != 0)
    {
        ProcessorNameString = ExAllocatePoolWithTag(PagedPool, Length, 'IPCA');
        if (ProcessorNameString == NULL)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto done;
        }

        Status = AcpiRegQueryValue(ProcessorHandle,
                                   L"ProcessorNameString",
                                   NULL,
                                   ProcessorNameString,
                                   &Length);
        if (!NT_SUCCESS(Status))
            goto done;

        Length = 0;
    }

    AcpiRegQueryValue(ProcessorHandle,
                      L"VendorIdentifier",
                      NULL,
                      NULL,
                      &Length);

    if (Length != 0)
    {
        ProcessorVendorIdentifier = ExAllocatePoolWithTag(PagedPool, Length, 'IPCA');
        if (ProcessorVendorIdentifier == NULL)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto done;
        }

        Status = AcpiRegQueryValue(ProcessorHandle,
                                   L"VendorIdentifier",
                                   NULL,
                                   ProcessorVendorIdentifier,
                                   &Length);
        if (!NT_SUCCESS(Status))
            goto done;

        Length = 0;
    }

    for (i = 0; i < wcslen(ProcessorIdentifier); i++)
    {
        if (ProcessorIdentifier[i] == L' ')
            ProcessorIdentifier[i] = L'_';
    }

    Ptr = wcsstr(ProcessorIdentifier, L"Stepping");
    if (Ptr != NULL)
    {
        Ptr--;
        Level1Length = (ULONG)(Ptr - ProcessorIdentifier);
    }

    Ptr = wcsstr(ProcessorIdentifier, L"Model");
    if (Ptr != NULL)
    {
        Ptr--;
        Level2Length = (ULONG)(Ptr - ProcessorIdentifier);
    }

    Ptr = wcsstr(ProcessorIdentifier, L"Family");
    if (Ptr != NULL)
    {
        Ptr--;
        Level3Length = (ULONG)(Ptr - ProcessorIdentifier);
    }

    HardwareIdsLength = 5 + wcslen(ProcessorVendorIdentifier) + 3 + Level1Length + 1 +
                        1 + wcslen(ProcessorVendorIdentifier) + 3 + Level1Length + 1 +
                        5 + wcslen(ProcessorVendorIdentifier) + 3 + Level2Length + 1 +
                        1 + wcslen(ProcessorVendorIdentifier) + 3 + Level2Length + 1 +
                        5 + wcslen(ProcessorVendorIdentifier) + 3 + Level3Length + 1 +
                        1 + wcslen(ProcessorVendorIdentifier) + 3 + Level3Length + 1 +
                        2;

    HardwareIdsBuffer = ExAllocatePoolWithTag(PagedPool, HardwareIdsLength * sizeof(WCHAR), 'IPCA');
    if (HardwareIdsBuffer == NULL)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto done;
    }

    Length = 0;
    Length += swprintf(&HardwareIdsBuffer[Length], L"ACPI\\%s_-_%.*s", ProcessorVendorIdentifier, Level1Length, ProcessorIdentifier);
    Length++;

    Length += swprintf(&HardwareIdsBuffer[Length], L"*%s_-_%.*s", ProcessorVendorIdentifier, Level1Length, ProcessorIdentifier);
    Length++;

    Length += swprintf(&HardwareIdsBuffer[Length], L"ACPI\\%s_-_%.*s", ProcessorVendorIdentifier, Level2Length, ProcessorIdentifier);
    Length++;

    Length += swprintf(&HardwareIdsBuffer[Length], L"*%s_-_%.*s", ProcessorVendorIdentifier, Level2Length, ProcessorIdentifier);
    Length++;

    Length += swprintf(&HardwareIdsBuffer[Length], L"ACPI\\%s_-_%.*s", ProcessorVendorIdentifier, Level3Length, ProcessorIdentifier);
    Length++;

    Length += swprintf(&HardwareIdsBuffer[Length], L"*%s_-_%.*s", ProcessorVendorIdentifier, Level3Length, ProcessorIdentifier);
    Length++;
    HardwareIdsBuffer[Length] = UNICODE_NULL;

    ProcessorHardwareIds.Length = HardwareIdsLength * sizeof(WCHAR);
    ProcessorHardwareIds.MaximumLength = ProcessorHardwareIds.Length;
    ProcessorHardwareIds.Buffer = HardwareIdsBuffer;

done:
    if (ProcessorHandle != NULL)
        ZwClose(ProcessorHandle);

    if (ProcessorIdentifier != NULL)
        ExFreePoolWithTag(ProcessorIdentifier, 'IPCA');

    if (ProcessorVendorIdentifier != NULL)
        ExFreePoolWithTag(ProcessorVendorIdentifier, 'IPCA');

    if (!NT_SUCCESS(Status))
    {
        if (HardwareIdsBuffer != NULL)
            ExFreePoolWithTag(HardwareIdsBuffer, 'IPCA');
    }

    return Status;
}

NTSTATUS
NTAPI
DriverEntry (
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath
    )
{
    DPRINT("Driver Entry \n");

    GetProcessorInformation();

    //
    // Set entry points into the driver
    //
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ACPIDispatchDeviceControl;
    DriverObject->MajorFunction [IRP_MJ_PNP] = Bus_PnP;
    DriverObject->MajorFunction [IRP_MJ_POWER] = Bus_Power;
    DriverObject->MajorFunction [IRP_MJ_CREATE] = ACPIDispatchCreateClose;
    DriverObject->MajorFunction [IRP_MJ_CLOSE] = ACPIDispatchCreateClose;

    DriverObject->DriverExtension->AddDevice = Bus_AddDevice;

    return STATUS_SUCCESS;
}
