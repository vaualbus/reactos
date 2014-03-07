/*
 * ndis.h
 *
 * Network Device Interface Specification definitions
 *
 * This file is part of the ReactOS DDK package.
 *
 * Contributors:
 *   Created by Casper S. Hornstrup <chorns@users.sourceforge.net>
 *
 * THIS SOFTWARE IS NOT COPYRIGHTED
 *
 * This source code is offered for use in the public domain. You may
 * use, modify or distribute it freely.
 *
 * This code is distributed in the hope that it will be useful but
 * WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 * DISCLAIMED. This includes but is not limited to warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * DEFINES: i386                 - Target platform is i386
 *          NDIS_WRAPPER         - Define only for NDIS library
 *          NDIS_MINIPORT_DRIVER - Define only for NDIS miniport drivers
 *          NDIS40               - Use NDIS 4.0 structures by default
 *          NDIS50               - Use NDIS 5.0 structures by default
 *          NDIS51               - Use NDIS 5.1 structures by default
 *          NDIS40_MINIPORT      - Building NDIS 4.0 miniport driver
 *          NDIS50_MINIPORT      - Building NDIS 5.0 miniport driver
 *          NDIS51_MINIPORT      - Building NDIS 5.1 miniport driver
 */

#ifndef _NDIS_
#define _NDIS_

#ifndef NDIS_WDM
#define NDIS_WDM 0
#endif

#include "ntddk.h"
#include "netpnp.h"
#include "ntstatus.h"
#include "netevent.h"
#include <qos.h>

typedef _Return_type_success_(return >= 0) int NDIS_STATUS, *PNDIS_STATUS;

#include "ntddndis.h"

#if !defined(_WINDEF_H)
typedef unsigned int UINT, *PUINT;
#endif

#ifdef __cplusplus
extern "C" {
#endif

__drv_Mode_impl(NDIS_INCLUDED)

#ifndef __NET_PNP__
#define __NET_PNP__

typedef enum _NET_DEVICE_POWER_STATE {
  NetDeviceStateUnspecified = 0,
  NetDeviceStateD0,
  NetDeviceStateD1,
  NetDeviceStateD2,
  NetDeviceStateD3,
  NetDeviceStateMaximum
} NET_DEVICE_POWER_STATE, *PNET_DEVICE_POWER_STATE;

typedef enum _NET_PNP_EVENT_CODE {
  NetEventSetPower,
  NetEventQueryPower,
  NetEventQueryRemoveDevice,
  NetEventCancelRemoveDevice,
  NetEventReconfigure,
  NetEventBindList,
  NetEventBindsComplete,
  NetEventPnPCapabilities,
  NetEventPause,
  NetEventRestart,
  NetEventPortActivation,
  NetEventPortDeactivation,
  NetEventIMReEnableDevice,
  NetEventMaximum
} NET_PNP_EVENT_CODE, *PNET_PNP_EVENT_CODE;

typedef struct _NET_PNP_EVENT {
  NET_PNP_EVENT_CODE NetEvent;
  PVOID Buffer;
  ULONG BufferLength;
  ULONG_PTR NdisReserved[4];
  ULONG_PTR TransportReserved[4];
  ULONG_PTR TdiReserved[4];
  ULONG_PTR TdiClientReserved[4];
} NET_PNP_EVENT, *PNET_PNP_EVENT;

#endif /* __NET_PNP__ */

#if !defined(NDIS_WRAPPER)

#if (defined(NDIS_MINIPORT_MAJOR_VERSION) ||  \
    (defined(NDIS_MINIPORT_MINOR_VERSION)) || \
    (defined(NDIS_PROTOCOL_MAJOR_VERSION)) || \
    (defined(NDIS_PROTOCOL_MINOR_VERSION)) || \
    (defined(NDIS_FILTER_MAJOR_VERSION)) ||   \
    (defined(NDIS_FILTER_MINOR_VERSION)))
#error "Driver should not redefine NDIS reserved macros"
#endif

#if defined(NDIS_MINIPORT_DRIVER)

#if defined(NDIS620_MINIPORT)
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 20
#elif defined(NDIS61_MINIPORT)
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 1
#elif defined(NDIS60_MINIPORT)
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 0
#elif defined(NDIS51_MINIPORT)
#define NDIS_MINIPORT_MAJOR_VERSION 5
#define NDIS_MINIPORT_MINOR_VERSION 1
#elif defined(NDIS50_MINIPORT)
#define NDIS_MINIPORT_MAJOR_VERSION 5
#define NDIS_MINIPORT_MINOR_VERSION 0
#else
#error "Only NDIS miniport drivers with version >= 5 are supported"
#endif

#if ((NDIS_MINIPORT_MAJOR_VERSION == 6) &&    \
       (NDIS_MINIPORT_MINOR_VERSION != 20) && \
       (NDIS_MINIPORT_MINOR_VERSION != 1) &&  \
       (NDIS_MINIPORT_MINOR_VERSION != 0))
#error "Invalid miniport major/minor version combination"
#elif ((NDIS_MINIPORT_MAJOR_VERSION == 5) &&  \
       (NDIS_MINIPORT_MINOR_VERSION != 1) &&  \
       (NDIS_MINIPORT_MINOR_VERSION != 0))
#error "Invalid miniport major/minor version combination"
#endif

#if  (NDIS_MINIPORT_MAJOR_VERSION == 6) && \
     ((NDIS_MINIPORT_MINOR_VERSION == 20 && NTDDI_VERSION < NTDDI_WIN7)  || \
      (NDIS_MINIPORT_MINOR_VERSION == 1 && NTDDI_VERSION < NTDDI_VISTA) || \
      (NDIS_MINIPORT_MINOR_VERSION == 0 && NTDDI_VERSION < NTDDI_VISTA))
#error "Wrong NDIS/DDI version"
#elif ((NDIS_MINIPORT_MAJOR_VERSION == 5) && \
       (((NDIS_MINIPORT_MINOR_VERSION == 1) && (NTDDI_VERSION < NTDDI_WINXP)) || \
         ((NDIS_MINIPORT_MINOR_VERSION == 0) && (NTDDI_VERSION < NTDDI_WIN2K))))
#error "Wrong NDIS/DDI version"
#endif


#endif /* defined(NDIS_MINIPORT_DRIVER) */

#if defined(NDIS30)
#error "Only NDIS Protocol drivers version 4 or later are supported"
#endif

#if defined(NDIS620)
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 20
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 20
#elif defined(NDIS61)
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 1
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 1
#elif defined(NDIS60)
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 0
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 0
#elif defined(NDIS51)
#define NDIS_PROTOCOL_MAJOR_VERSION 5
#define NDIS_PROTOCOL_MINOR_VERSION 1
#elif defined(NDIS50)
#define NDIS_PROTOCOL_MAJOR_VERSION 5
#define NDIS_PROTOCOL_MINOR_VERSION 0
#elif defined(NDIS40)
#define NDIS_PROTOCOL_MAJOR_VERSION 4
#define NDIS_PROTOCOL_MINOR_VERSION 0
#endif

#if !defined(NDIS_MINIPORT_DRIVER) && !defined(NDIS_PROTOCOL_MAJOR_VERSION)
#define NDIS40
#define NDIS_PROTOCOL_MAJOR_VERSION 4
#define NDIS_PROTOCOL_MINOR_VERSION 0
#endif

#if defined(NDIS_FILTER_MAJOR_VERSION)

#if ((NDIS_FILTER_MAJOR_VERSION == 6) &&  \
     (NDIS_FILTER_MINOR_VERSION != 20) && \
     (NDIS_FILTER_MINOR_VERSION != 1) &&  \
     (NDIS_FILTER_MINOR_VERSION != 0))
#error "Invalid Filter version"
#endif

#endif /* defined(NDIS_FILTER_MAJOR_VERSION) */


#if defined(NDIS_PROTOCOL_MAJOR_VERSION)

#if ((NDIS_PROTOCOL_MAJOR_VERSION == 6) &&  \
     (NDIS_PROTOCOL_MINOR_VERSION != 20) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 1) &&  \
     (NDIS_PROTOCOL_MINOR_VERSION != 0))
#error "Invalid Protocol version"
#elif ((NDIS_PROTOCOL_MAJOR_VERSION == 5) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 1) && (NDIS_PROTOCOL_MINOR_VERSION != 0))
#error "Invalid Protocol version"
#elif ((NDIS_PROTOCOL_MAJOR_VERSION == 4) && (NDIS_PROTOCOL_MINOR_VERSION != 0))
#error "Invalid Protocol major/minor version"
#endif

#if ((NDIS_PROTOCOL_MAJOR_VERSION == 6) && (NTDDI_VERSION < NTDDI_VISTA))
#error "Wrong NDIS/DDI version"
#endif

#endif /* defined(NDIS_PROTOCOL_MAJOR_VERSION) */

#endif /* !defined(NDIS_WRAPPER) */

#if !defined(NDIS_LEGACY_MINIPORT)

#if ((defined(NDIS_MINIPORT_DRIVER) && (NDIS_MINIPORT_MAJOR_VERSION < 6)) || NDIS_WRAPPER)
#define NDIS_LEGACY_MINIPORT    1
#else
#define NDIS_LEGACY_MINIPORT    0
#endif

#endif /* !defined(NDIS_LEGACY_MINIPORT) */

#if !defined(NDIS_LEGACY_PROTOCOL)

#if ((defined(NDIS_PROTOCOL_MAJOR_VERSION) && (NDIS_PROTOCOL_MAJOR_VERSION < 6)) || NDIS_WRAPPER)
#define NDIS_LEGACY_PROTOCOL    1
#else
#define NDIS_LEGACY_PROTOCOL    0
#endif

#endif /* !defined(NDIS_LEGACY_PROTOCOL) */

#if !defined(NDIS_LEGACY_DRIVER)

#if (NDIS_LEGACY_MINIPORT || NDIS_LEGACY_PROTOCOL || NDIS_WRAPPER)
#define NDIS_LEGACY_DRIVER      1
#else
#define NDIS_LEGACY_DRIVER      0
#endif

#endif /* !defined(NDIS_LEGACY_DRIVER) */

#if !defined(NDIS_SUPPORT_NDIS6)

#if ((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) || \
     (defined (NDIS60)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS6      1
#else
#define NDIS_SUPPORT_NDIS6      0
#endif

#endif /* !defined(NDIS_SUPPORT_NDIS6) */

#if !defined(NDIS_SUPPORT_NDIS61)
#if  (((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) && \
       (defined (NDIS_MINIPORT_MINOR_VERSION) && (NDIS_MINIPORT_MINOR_VERSION >= 1))) || \
      (defined (NDIS61)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS61      1
#else
#define NDIS_SUPPORT_NDIS61      0
#endif
#endif /* !defined(NDIS_SUPPORT_NDIS61) */

#if !defined(NDIS_SUPPORT_NDIS620)

#if  (((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) && \
       (defined (NDIS_MINIPORT_MINOR_VERSION) && (NDIS_MINIPORT_MINOR_VERSION >= 20))) || \
      (defined (NDIS620)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS620      1
#else
#define NDIS_SUPPORT_NDIS620      0
#endif

#endif /* !defined(NDIS_SUPPORT_NDIS620) */

#if (NDIS_SUPPORT_NDIS620)
#undef NDIS_SUPPORT_NDIS61
#define NDIS_SUPPORT_NDIS61 1
#endif

#if (NDIS_SUPPORT_NDIS61)
#undef NDIS_SUPPORT_NDIS6
#define NDIS_SUPPORT_NDIS6 1
#endif

#if defined(NDIS61_MINIPORT) || defined(NDIS60_MINIPORT) || defined(NDIS61) || \
    defined(NDIS60) || defined(NDIS_WRAPPER) || defined(NDIS_LEGACY_DRIVER)
#define NDIS_SUPPORT_60_COMPATIBLE_API      1
#else
#define NDIS_SUPPORT_60_COMPATIBLE_API      0
#endif

#if defined(NDIS_WRAPPER)
#define NDISAPI
#else
#define NDISAPI DECLSPEC_IMPORT
#endif

typedef PVOID QUEUED_CLOSE; //FIXME : Doesn't exist in public headers

typedef struct _X_FILTER FDDI_FILTER, *PFDDI_FILTER;
typedef struct _X_FILTER TR_FILTER, *PTR_FILTER;
typedef struct _X_FILTER NULL_FILTER, *PNULL_FILTER;

typedef struct _NDIS_MINIPORT_BLOCK NDIS_MINIPORT_BLOCK, *PNDIS_MINIPORT_BLOCK;

typedef struct _REFERENCE {
  KSPIN_LOCK SpinLock;
  USHORT ReferenceCount;
  BOOLEAN Closing;
} REFERENCE, *PREFERENCE;

/* NDIS base types */

typedef struct _NDIS_SPIN_LOCK {
  KSPIN_LOCK SpinLock;
  KIRQL OldIrql;
} NDIS_SPIN_LOCK, *PNDIS_SPIN_LOCK;

typedef struct _NDIS_EVENT {
  KEVENT Event;
} NDIS_EVENT, *PNDIS_EVENT;

typedef PVOID NDIS_HANDLE, *PNDIS_HANDLE;

typedef ANSI_STRING NDIS_ANSI_STRING, *PNDIS_ANSI_STRING;
typedef UNICODE_STRING NDIS_STRING, *PNDIS_STRING;

typedef MDL NDIS_BUFFER, *PNDIS_BUFFER;

/* NDIS_STATUS constants */
#define NDIS_STATUS_SUCCESS                     ((NDIS_STATUS)STATUS_SUCCESS)
#define NDIS_STATUS_PENDING                     ((NDIS_STATUS)STATUS_PENDING)
#define NDIS_STATUS_NOT_RECOGNIZED              ((NDIS_STATUS)0x00010001L)
#define NDIS_STATUS_NOT_COPIED                  ((NDIS_STATUS)0x00010002L)
#define NDIS_STATUS_NOT_ACCEPTED                ((NDIS_STATUS)0x00010003L)
#define NDIS_STATUS_CALL_ACTIVE                 ((NDIS_STATUS)0x00010007L)
#define NDIS_STATUS_INDICATION_REQUIRED         ((NDIS_STATUS)STATUS_NDIS_INDICATION_REQUIRED)
#define NDIS_STATUS_ONLINE                      ((NDIS_STATUS)0x40010003L)
#define NDIS_STATUS_RESET_START                 ((NDIS_STATUS)0x40010004L)
#define NDIS_STATUS_RESET_END                   ((NDIS_STATUS)0x40010005L)
#define NDIS_STATUS_RING_STATUS                 ((NDIS_STATUS)0x40010006L)
#define NDIS_STATUS_CLOSED                      ((NDIS_STATUS)0x40010007L)
#define NDIS_STATUS_WAN_LINE_UP                 ((NDIS_STATUS)0x40010008L)
#define NDIS_STATUS_WAN_LINE_DOWN               ((NDIS_STATUS)0x40010009L)
#define NDIS_STATUS_WAN_FRAGMENT                ((NDIS_STATUS)0x4001000AL)
#define NDIS_STATUS_MEDIA_CONNECT               ((NDIS_STATUS)0x4001000BL)
#define NDIS_STATUS_MEDIA_DISCONNECT            ((NDIS_STATUS)0x4001000CL)
#define NDIS_STATUS_HARDWARE_LINE_UP            ((NDIS_STATUS)0x4001000DL)
#define NDIS_STATUS_HARDWARE_LINE_DOWN          ((NDIS_STATUS)0x4001000EL)
#define NDIS_STATUS_INTERFACE_UP                ((NDIS_STATUS)0x4001000FL)
#define NDIS_STATUS_INTERFACE_DOWN              ((NDIS_STATUS)0x40010010L)
#define NDIS_STATUS_MEDIA_BUSY                  ((NDIS_STATUS)0x40010011L)
#define NDIS_STATUS_MEDIA_SPECIFIC_INDICATION   ((NDIS_STATUS)0x40010012L)
#define NDIS_STATUS_WW_INDICATION               NDIS_STATUS_MEDIA_SPECIFIC_INDICATION
#define NDIS_STATUS_LINK_SPEED_CHANGE           ((NDIS_STATUS)0x40010013L)
#define NDIS_STATUS_WAN_GET_STATS               ((NDIS_STATUS)0x40010014L)
#define NDIS_STATUS_WAN_CO_FRAGMENT             ((NDIS_STATUS)0x40010015L)
#define NDIS_STATUS_WAN_CO_LINKPARAMS           ((NDIS_STATUS)0x40010016L)
#if NDIS_SUPPORT_NDIS6
#define NDIS_STATUS_LINK_STATE                  ((NDIS_STATUS)0x40010017L)
#define NDIS_STATUS_NETWORK_CHANGE              ((NDIS_STATUS)0x40010018L)
#define NDIS_STATUS_MEDIA_SPECIFIC_INDICATION_EX ((NDIS_STATUS)0x40010019L)
#define NDIS_STATUS_PORT_STATE                  ((NDIS_STATUS)0x40010022L)
#define NDIS_STATUS_OPER_STATUS                 ((NDIS_STATUS)0x40010023L)
#define NDIS_STATUS_PACKET_FILTER               ((NDIS_STATUS)0x40010024L)
#endif /* NDIS_SUPPORT_NDIS6 */
#define NDIS_STATUS_WAN_CO_MTULINKPARAMS        ((NDIS_STATUS)0x40010025L)

#if NDIS_SUPPORT_NDIS6

#define NDIS_STATUS_IP_OPER_STATUS              ((NDIS_STATUS)0x40010026L)

#define NDIS_STATUS_OFFLOAD_PAUSE               ((NDIS_STATUS)0x40020001L)
#define NDIS_STATUS_UPLOAD_ALL                  ((NDIS_STATUS)0x40020002L)
#define NDIS_STATUS_OFFLOAD_RESUME              ((NDIS_STATUS)0x40020003L)
#define NDIS_STATUS_OFFLOAD_PARTIAL_SUCCESS     ((NDIS_STATUS)0x40020004L)
#define NDIS_STATUS_OFFLOAD_STATE_INVALID       ((NDIS_STATUS)0x40020005L)
#define NDIS_STATUS_TASK_OFFLOAD_CURRENT_CONFIG ((NDIS_STATUS)0x40020006L)
#define NDIS_STATUS_TASK_OFFLOAD_HARDWARE_CAPABILITIES ((NDIS_STATUS)0x40020007L)
#define NDIS_STATUS_OFFLOAD_ENCASPULATION_CHANGE ((NDIS_STATUS)0x40020008L)
#define NDIS_STATUS_TCP_CONNECTION_OFFLOAD_HARDWARE_CAPABILITIES ((NDIS_STATUS)0x4002000BL)

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_STATUS_HD_SPLIT_CURRENT_CONFIG     ((NDIS_STATUS)0x4002000CL)
#endif

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_STATUS_RECEIVE_QUEUE_STATE         ((NDIS_STATUS)0x4002000DL)
#endif

#define NDIS_STATUS_OFFLOAD_IM_RESERVED1        ((NDIS_STATUS)0x40020100L)
#define NDIS_STATUS_OFFLOAD_IM_RESERVED2        ((NDIS_STATUS)0x40020101L)
#define NDIS_STATUS_OFFLOAD_IM_RESERVED3        ((NDIS_STATUS)0x40020102L)

#define NDIS_STATUS_DOT11_SCAN_CONFIRM          ((NDIS_STATUS)0x40030000L)
#define NDIS_STATUS_DOT11_MPDU_MAX_LENGTH_CHANGED ((NDIS_STATUS)0x40030001L)
#define NDIS_STATUS_DOT11_ASSOCIATION_START     ((NDIS_STATUS)0x40030002L)
#define NDIS_STATUS_DOT11_ASSOCIATION_COMPLETION ((NDIS_STATUS)0x40030003L)
#define NDIS_STATUS_DOT11_CONNECTION_START      ((NDIS_STATUS)0x40030004L)
#define NDIS_STATUS_DOT11_CONNECTION_COMPLETION ((NDIS_STATUS)0x40030005L)
#define NDIS_STATUS_DOT11_ROAMING_START         ((NDIS_STATUS)0x40030006L)
#define NDIS_STATUS_DOT11_ROAMING_COMPLETION    ((NDIS_STATUS)0x40030007L)
#define NDIS_STATUS_DOT11_DISASSOCIATION        ((NDIS_STATUS)0x40030008L)
#define NDIS_STATUS_DOT11_TKIPMIC_FAILURE       ((NDIS_STATUS)0x40030009L)
#define NDIS_STATUS_DOT11_PMKID_CANDIDATE_LIST  ((NDIS_STATUS)0x4003000AL)
#define NDIS_STATUS_DOT11_PHY_STATE_CHANGED     ((NDIS_STATUS)0x4003000BL)
#define NDIS_STATUS_DOT11_LINK_QUALITY          ((NDIS_STATUS)0x4003000CL)
#define NDIS_STATUS_DOT11_INCOMING_ASSOC_STARTED ((NDIS_STATUS)0x4003000DL)
#define NDIS_STATUS_DOT11_INCOMING_ASSOC_REQUEST_RECEIVED ((NDIS_STATUS)0x4003000EL)
#define NDIS_STATUS_DOT11_INCOMING_ASSOC_COMPLETION ((NDIS_STATUS)0x4003000FL)
#define NDIS_STATUS_DOT11_STOP_AP               ((NDIS_STATUS)0x40030010L)
#define NDIS_STATUS_DOT11_PHY_FREQUENCY_ADOPTED ((NDIS_STATUS)0x40030011L)
#define NDIS_STATUS_DOT11_CAN_SUSTAIN_AP        ((NDIS_STATUS)0x40030012L)

#define NDIS_STATUS_WWAN_DEVICE_CAPS            ((NDIS_STATUS)0x40041000)
#define NDIS_STATUS_WWAN_READY_INFO             ((NDIS_STATUS)0x40041001)
#define NDIS_STATUS_WWAN_RADIO_STATE            ((NDIS_STATUS)0x40041002)
#define NDIS_STATUS_WWAN_PIN_INFO               ((NDIS_STATUS)0x40041003)
#define NDIS_STATUS_WWAN_PIN_LIST               ((NDIS_STATUS)0x40041004)
#define NDIS_STATUS_WWAN_HOME_PROVIDER          ((NDIS_STATUS)0x40041005)
#define NDIS_STATUS_WWAN_PREFERRED_PROVIDERS    ((NDIS_STATUS)0x40041006)
#define NDIS_STATUS_WWAN_VISIBLE_PROVIDERS      ((NDIS_STATUS)0x40041007)
#define NDIS_STATUS_WWAN_REGISTER_STATE         ((NDIS_STATUS)0x40041008)
#define NDIS_STATUS_WWAN_PACKET_SERVICE         ((NDIS_STATUS)0x40041009)
#define NDIS_STATUS_WWAN_SIGNAL_STATE           ((NDIS_STATUS)0x4004100a)
#define NDIS_STATUS_WWAN_CONTEXT_STATE          ((NDIS_STATUS)0x4004100b)
#define NDIS_STATUS_WWAN_PROVISIONED_CONTEXTS   ((NDIS_STATUS)0x4004100c)
#define NDIS_STATUS_WWAN_SERVICE_ACTIVATION     ((NDIS_STATUS)0x4004100d)
#define NDIS_STATUS_WWAN_SMS_CONFIGURATION      ((NDIS_STATUS)0x4004100e)
#define NDIS_STATUS_WWAN_SMS_RECEIVE            ((NDIS_STATUS)0x4004100f)
#define NDIS_STATUS_WWAN_SMS_SEND               ((NDIS_STATUS)0x40041010)
#define NDIS_STATUS_WWAN_SMS_DELETE             ((NDIS_STATUS)0x40041011)
#define NDIS_STATUS_WWAN_SMS_STATUS             ((NDIS_STATUS)0x40041012)
#define NDIS_STATUS_WWAN_DNS_ADDRESS            ((NDIS_STATUS)0x40041013)

#define NDIS_STATUS_WWAN_VENDOR_SPECIFIC        ((NDIS_STATUS)0x40043000)

#endif /* NDIS_SUPPORT_NDIS6 */

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_STATUS_PM_WOL_PATTERN_REJECTED     ((NDIS_STATUS)0x40030051L)
#define NDIS_STATUS_PM_OFFLOAD_REJECTED         ((NDIS_STATUS)0x40030052L)
#define NDIS_STATUS_PM_CAPABILITIES_CHANGE      ((NDIS_STATUS)0x40030053L)
#endif

#define NDIS_STATUS_NOT_RESETTABLE              ((NDIS_STATUS)0x80010001L)
#define NDIS_STATUS_SOFT_ERRORS                 ((NDIS_STATUS)0x80010003L)
#define NDIS_STATUS_HARD_ERRORS                 ((NDIS_STATUS)0x80010004L)
#define NDIS_STATUS_BUFFER_OVERFLOW             ((NDIS_STATUS)STATUS_BUFFER_OVERFLOW)
#define NDIS_STATUS_FAILURE                     ((NDIS_STATUS)STATUS_UNSUCCESSFUL)
#define NDIS_STATUS_RESOURCES                   ((NDIS_STATUS)STATUS_INSUFFICIENT_RESOURCES)
#define NDIS_STATUS_CLOSING                     ((NDIS_STATUS)0xC0010002L)
#define NDIS_STATUS_BAD_VERSION                 ((NDIS_STATUS)0xC0010004L)
#define NDIS_STATUS_BAD_CHARACTERISTICS         ((NDIS_STATUS)0xC0010005L)
#define NDIS_STATUS_ADAPTER_NOT_FOUND           ((NDIS_STATUS)0xC0010006L)
#define NDIS_STATUS_OPEN_FAILED                 ((NDIS_STATUS)0xC0010007L)
#define NDIS_STATUS_DEVICE_FAILED               ((NDIS_STATUS)0xC0010008L)
#define NDIS_STATUS_MULTICAST_FULL              ((NDIS_STATUS)0xC0010009L)
#define NDIS_STATUS_MULTICAST_EXISTS            ((NDIS_STATUS)0xC001000AL)
#define NDIS_STATUS_MULTICAST_NOT_FOUND         ((NDIS_STATUS)0xC001000BL)
#define NDIS_STATUS_REQUEST_ABORTED             ((NDIS_STATUS)0xC001000CL)
#define NDIS_STATUS_RESET_IN_PROGRESS           ((NDIS_STATUS)0xC001000DL)
#define NDIS_STATUS_CLOSING_INDICATING          ((NDIS_STATUS)0xC001000EL)
#define NDIS_STATUS_NOT_SUPPORTED               ((NDIS_STATUS)STATUS_NOT_SUPPORTED)
#define NDIS_STATUS_INVALID_PACKET              ((NDIS_STATUS)0xC001000FL)
#define NDIS_STATUS_OPEN_LIST_FULL              ((NDIS_STATUS)0xC0010010L)
#define NDIS_STATUS_ADAPTER_NOT_READY           ((NDIS_STATUS)0xC0010011L)
#define NDIS_STATUS_ADAPTER_NOT_OPEN            ((NDIS_STATUS)0xC0010012L)
#define NDIS_STATUS_NOT_INDICATING              ((NDIS_STATUS)0xC0010013L)
#define NDIS_STATUS_INVALID_LENGTH              ((NDIS_STATUS)0xC0010014L)
#define NDIS_STATUS_INVALID_DATA                ((NDIS_STATUS)0xC0010015L)
#define NDIS_STATUS_BUFFER_TOO_SHORT            ((NDIS_STATUS)0xC0010016L)
#define NDIS_STATUS_INVALID_OID                 ((NDIS_STATUS)0xC0010017L)
#define NDIS_STATUS_ADAPTER_REMOVED             ((NDIS_STATUS)0xC0010018L)
#define NDIS_STATUS_UNSUPPORTED_MEDIA           ((NDIS_STATUS)0xC0010019L)
#define NDIS_STATUS_GROUP_ADDRESS_IN_USE        ((NDIS_STATUS)0xC001001AL)
#define NDIS_STATUS_FILE_NOT_FOUND              ((NDIS_STATUS)0xC001001BL)
#define NDIS_STATUS_ERROR_READING_FILE          ((NDIS_STATUS)0xC001001CL)
#define NDIS_STATUS_ALREADY_MAPPED              ((NDIS_STATUS)0xC001001DL)
#define NDIS_STATUS_RESOURCE_CONFLICT           ((NDIS_STATUS)0xC001001EL)
#define NDIS_STATUS_NO_CABLE                    ((NDIS_STATUS)0xC001001FL)

#define NDIS_STATUS_INVALID_SAP                 ((NDIS_STATUS)0xC0010020L)
#define NDIS_STATUS_SAP_IN_USE                  ((NDIS_STATUS)0xC0010021L)
#define NDIS_STATUS_INVALID_ADDRESS             ((NDIS_STATUS)0xC0010022L)
#define NDIS_STATUS_VC_NOT_ACTIVATED            ((NDIS_STATUS)0xC0010023L)
#define NDIS_STATUS_DEST_OUT_OF_ORDER           ((NDIS_STATUS)0xC0010024L)
#define NDIS_STATUS_VC_NOT_AVAILABLE            ((NDIS_STATUS)0xC0010025L)
#define NDIS_STATUS_CELLRATE_NOT_AVAILABLE      ((NDIS_STATUS)0xC0010026L)
#define NDIS_STATUS_INCOMPATABLE_QOS            ((NDIS_STATUS)0xC0010027L)
#define NDIS_STATUS_AAL_PARAMS_UNSUPPORTED      ((NDIS_STATUS)0xC0010028L)
#define NDIS_STATUS_NO_ROUTE_TO_DESTINATION     ((NDIS_STATUS)0xC0010029L)

#define NDIS_STATUS_TOKEN_RING_OPEN_ERROR       ((NDIS_STATUS)0xC0011000L)
#define NDIS_STATUS_INVALID_DEVICE_REQUEST      ((NDIS_STATUS)STATUS_INVALID_DEVICE_REQUEST)
#define NDIS_STATUS_NETWORK_UNREACHABLE         ((NDIS_STATUS)STATUS_NETWORK_UNREACHABLE)

#if NDIS_SUPPORT_NDIS6

#define NDIS_STATUS_SEND_ABORTED                ((NDIS_STATUS)STATUS_NDIS_REQUEST_ABORTED)
#define NDIS_STATUS_PAUSED                      ((NDIS_STATUS)STATUS_NDIS_PAUSED)
#define NDIS_STATUS_INTERFACE_NOT_FOUND         ((NDIS_STATUS)STATUS_NDIS_INTERFACE_NOT_FOUND)
#define NDIS_STATUS_INVALID_PARAMETER           ((NDIS_STATUS)STATUS_INVALID_PARAMETER)
#define NDIS_STATUS_UNSUPPORTED_REVISION        ((NDIS_STATUS)STATUS_NDIS_UNSUPPORTED_REVISION)
#define NDIS_STATUS_INVALID_PORT                ((NDIS_STATUS)STATUS_NDIS_INVALID_PORT)
#define NDIS_STATUS_INVALID_PORT_STATE          ((NDIS_STATUS)STATUS_NDIS_INVALID_PORT_STATE)
#define NDIS_STATUS_INVALID_STATE               ((NDIS_STATUS)STATUS_INVALID_DEVICE_STATE)
#define NDIS_STATUS_MEDIA_DISCONNECTED          ((NDIS_STATUS)STATUS_NDIS_MEDIA_DISCONNECTED)
#define NDIS_STATUS_LOW_POWER_STATE             ((NDIS_STATUS)STATUS_NDIS_LOW_POWER_STATE)
#define NDIS_STATUS_DOT11_AUTO_CONFIG_ENABLED   ((NDIS_STATUS)STATUS_NDIS_DOT11_AUTO_CONFIG_ENABLED)
#define NDIS_STATUS_DOT11_MEDIA_IN_USE          ((NDIS_STATUS)STATUS_NDIS_DOT11_MEDIA_IN_USE)
#define NDIS_STATUS_DOT11_POWER_STATE_INVALID   ((NDIS_STATUS)STATUS_NDIS_DOT11_POWER_STATE_INVALID)
#define NDIS_STATUS_UPLOAD_IN_PROGRESS          ((NDIS_STATUS)0xC0231001L)
#define NDIS_STATUS_REQUEST_UPLOAD              ((NDIS_STATUS)0xC0231002L)
#define NDIS_STATUS_UPLOAD_REQUESTED            ((NDIS_STATUS)0xC0231003L)
#define NDIS_STATUS_OFFLOAD_TCP_ENTRIES         ((NDIS_STATUS)0xC0231004L)
#define NDIS_STATUS_OFFLOAD_PATH_ENTRIES        ((NDIS_STATUS)0xC0231005L)
#define NDIS_STATUS_OFFLOAD_NEIGHBOR_ENTRIES    ((NDIS_STATUS)0xC0231006L)
#define NDIS_STATUS_OFFLOAD_IP_ADDRESS_ENTRIES  ((NDIS_STATUS)0xC0231007L)
#define NDIS_STATUS_OFFLOAD_HW_ADDRESS_ENTRIES  ((NDIS_STATUS)0xC0231008L)
#define NDIS_STATUS_OFFLOAD_VLAN_ENTRIES        ((NDIS_STATUS)0xC0231009L)
#define NDIS_STATUS_OFFLOAD_TCP_XMIT_BUFFER     ((NDIS_STATUS)0xC023100AL)
#define NDIS_STATUS_OFFLOAD_TCP_RCV_BUFFER      ((NDIS_STATUS)0xC023100BL)
#define NDIS_STATUS_OFFLOAD_TCP_RCV_WINDOW      ((NDIS_STATUS)0xC023100CL)
#define NDIS_STATUS_OFFLOAD_VLAN_MISMATCH       ((NDIS_STATUS)0xC023100DL)
#define NDIS_STATUS_OFFLOAD_DATA_NOT_ACCEPTED   ((NDIS_STATUS)0xC023100EL)
#define NDIS_STATUS_OFFLOAD_POLICY              ((NDIS_STATUS)0xC023100FL)
#define NDIS_STATUS_OFFLOAD_DATA_PARTIALLY_ACCEPTED ((NDIS_STATUS)0xC0231010L)
#define NDIS_STATUS_OFFLOAD_REQUEST_RESET       ((NDIS_STATUS)0xC0231011L)

#if NDIS_SUPPORT_NDIS620
#define NDIS_STATUS_PM_WOL_PATTERN_LIST_FULL    ((NDIS_STATUS)STATUS_NDIS_PM_WOL_PATTERN_LIST_FULL)
#define NDIS_STATUS_PM_PROTOCOL_OFFLOAD_LIST_FULL ((NDIS_STATUS)STATUS_NDIS_PM_PROTOCOL_OFFLOAD_LIST_FULL)
#endif

#endif /* NDIS_SUPPORT_NDIS6 */

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_STATUS_OFFLOAD_CONNECTION_REJECTED ((NDIS_STATUS)STATUS_NDIS_OFFLOAD_CONNECTION_REJECTED)
#endif

/* NDIS error codes for error logging */

#define NDIS_ERROR_CODE ULONG

#define NDIS_ERROR_CODE_RESOURCE_CONFLICT               EVENT_NDIS_RESOURCE_CONFLICT
#define NDIS_ERROR_CODE_OUT_OF_RESOURCES                EVENT_NDIS_OUT_OF_RESOURCE
#define NDIS_ERROR_CODE_HARDWARE_FAILURE                EVENT_NDIS_HARDWARE_FAILURE
#define NDIS_ERROR_CODE_ADAPTER_NOT_FOUND               EVENT_NDIS_ADAPTER_NOT_FOUND
#define NDIS_ERROR_CODE_INTERRUPT_CONNECT               EVENT_NDIS_INTERRUPT_CONNECT
#define NDIS_ERROR_CODE_DRIVER_FAILURE                  EVENT_NDIS_DRIVER_FAILURE
#define NDIS_ERROR_CODE_BAD_VERSION                     EVENT_NDIS_BAD_VERSION
#define NDIS_ERROR_CODE_TIMEOUT                         EVENT_NDIS_TIMEOUT
#define NDIS_ERROR_CODE_NETWORK_ADDRESS                 EVENT_NDIS_NETWORK_ADDRESS
#define NDIS_ERROR_CODE_UNSUPPORTED_CONFIGURATION       EVENT_NDIS_UNSUPPORTED_CONFIGURATION
#define NDIS_ERROR_CODE_INVALID_VALUE_FROM_ADAPTER      EVENT_NDIS_INVALID_VALUE_FROM_ADAPTER
#define NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER EVENT_NDIS_MISSING_CONFIGURATION_PARAMETER
#define NDIS_ERROR_CODE_BAD_IO_BASE_ADDRESS             EVENT_NDIS_BAD_IO_BASE_ADDRESS
#define NDIS_ERROR_CODE_RECEIVE_SPACE_SMALL             EVENT_NDIS_RECEIVE_SPACE_SMALL
#define NDIS_ERROR_CODE_ADAPTER_DISABLED                EVENT_NDIS_ADAPTER_DISABLED

/* Memory allocation flags. Used by Ndis[Allocate|Free]Memory */
#define NDIS_MEMORY_CONTIGUOUS            0x00000001
#define NDIS_MEMORY_NONCACHED             0x00000002

/* NIC attribute flags. Used by NdisMSetAttributes(Ex) */
#define	NDIS_ATTRIBUTE_IGNORE_PACKET_TIMEOUT    0x00000001
#define NDIS_ATTRIBUTE_IGNORE_REQUEST_TIMEOUT   0x00000002
#define NDIS_ATTRIBUTE_IGNORE_TOKEN_RING_ERRORS 0x00000004
#define NDIS_ATTRIBUTE_BUS_MASTER               0x00000008
#define NDIS_ATTRIBUTE_INTERMEDIATE_DRIVER      0x00000010
#define NDIS_ATTRIBUTE_DESERIALIZE              0x00000020
#define NDIS_ATTRIBUTE_NO_HALT_ON_SUSPEND       0x00000040
#define NDIS_ATTRIBUTE_SURPRISE_REMOVE_OK       0x00000080
#define NDIS_ATTRIBUTE_NOT_CO_NDIS              0x00000100
#define NDIS_ATTRIBUTE_USES_SAFE_BUFFER_APIS    0x00000200

/* Lock */

#if NDIS_SUPPORT_60_COMPATIBLE_API

typedef union _NDIS_RW_LOCK_REFCOUNT {
  UINT RefCount;
  UCHAR cacheLine[16];
} NDIS_RW_LOCK_REFCOUNT;

typedef struct _NDIS_RW_LOCK {
  __MINGW_EXTENSION union {
    __MINGW_EXTENSION struct {
      KSPIN_LOCK SpinLock;
      PVOID Context;
    };
    UCHAR Reserved[16];
  };
  __MINGW_EXTENSION union {
    NDIS_RW_LOCK_REFCOUNT RefCount[MAXIMUM_PROCESSORS]; 
    ULONG RefCountEx[sizeof(NDIS_RW_LOCK_REFCOUNT)/sizeof(ULONG) * MAXIMUM_PROCESSORS];
    __MINGW_EXTENSION struct {
      KSPIN_LOCK RefCountLock;
      volatile ULONG SharedRefCount;
      volatile BOOLEAN WriterWaiting;
    };
  };
} NDIS_RW_LOCK, *PNDIS_RW_LOCK;

typedef struct _LOCK_STATE {
  USHORT LockState;
  KIRQL OldIrql;
} LOCK_STATE, *PLOCK_STATE;

#endif /* NDIS_SUPPORT_60_COMPATIBLE_API */

/* Timer */

_IRQL_requires_(DISPATCH_LEVEL)
_IRQL_requires_same_
_Function_class_(NDIS_TIMER_FUNCTION)
typedef VOID
(NTAPI NDIS_TIMER_FUNCTION)(
  _In_ PVOID SystemSpecific1,
  _In_ PVOID FunctionContext,
  _In_ PVOID SystemSpecific2,
  _In_ PVOID SystemSpecific3);
typedef NDIS_TIMER_FUNCTION *PNDIS_TIMER_FUNCTION;

typedef struct _NDIS_TIMER {
  KTIMER Timer;
  KDPC Dpc;
} NDIS_TIMER, *PNDIS_TIMER;

/* Hardware */

typedef CM_MCA_POS_DATA NDIS_MCA_POS_DATA, *PNDIS_MCA_POS_DATA;
typedef CM_EISA_SLOT_INFORMATION NDIS_EISA_SLOT_INFORMATION, *PNDIS_EISA_SLOT_INFORMATION;
typedef CM_EISA_FUNCTION_INFORMATION NDIS_EISA_FUNCTION_INFORMATION, *PNDIS_EISA_FUNCTION_INFORMATION;
typedef CM_PARTIAL_RESOURCE_LIST NDIS_RESOURCE_LIST, *PNDIS_RESOURCE_LIST;

/* Flag bits */
#define	READABLE_LOCAL_CLOCK                    0x00000001
#define	CLOCK_NETWORK_DERIVED                   0x00000002
#define	CLOCK_PRECISION                         0x00000004
#define	RECEIVE_TIME_INDICATION_CAPABLE         0x00000008
#define	TIMED_SEND_CAPABLE                      0x00000010
#define	TIME_STAMP_CAPABLE                      0x00000020

/* NDIS packet filter bits (OID_GEN_CURRENT_PACKET_FILTER) */
#define NDIS_PACKET_TYPE_DIRECTED               0x00000001
#define NDIS_PACKET_TYPE_MULTICAST              0x00000002
#define NDIS_PACKET_TYPE_ALL_MULTICAST          0x00000004
#define NDIS_PACKET_TYPE_BROADCAST              0x00000008
#define NDIS_PACKET_TYPE_SOURCE_ROUTING         0x00000010
#define NDIS_PACKET_TYPE_PROMISCUOUS            0x00000020
#define NDIS_PACKET_TYPE_SMT                    0x00000040
#define NDIS_PACKET_TYPE_ALL_LOCAL              0x00000080
#define NDIS_PACKET_TYPE_GROUP                  0x00001000
#define NDIS_PACKET_TYPE_ALL_FUNCTIONAL         0x00002000
#define NDIS_PACKET_TYPE_FUNCTIONAL             0x00004000
#define NDIS_PACKET_TYPE_MAC_FRAME              0x00008000

/* NDIS protocol option bits (OID_GEN_PROTOCOL_OPTIONS) */
#define NDIS_PROT_OPTION_ESTIMATED_LENGTH       0x00000001
#define NDIS_PROT_OPTION_NO_LOOPBACK            0x00000002
#define NDIS_PROT_OPTION_NO_RSVD_ON_RCVPKT      0x00000004

/* NDIS MAC option bits (OID_GEN_MAC_OPTIONS) */
#define NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA     0x00000001
#define NDIS_MAC_OPTION_RECEIVE_SERIALIZED      0x00000002
#define NDIS_MAC_OPTION_TRANSFERS_NOT_PEND      0x00000004
#define NDIS_MAC_OPTION_NO_LOOPBACK             0x00000008
#define NDIS_MAC_OPTION_FULL_DUPLEX             0x00000010
#define	NDIS_MAC_OPTION_EOTX_INDICATION         0x00000020
#define	NDIS_MAC_OPTION_8021P_PRIORITY          0x00000040
#define NDIS_MAC_OPTION_RESERVED                0x80000000

#define	NDIS_GUID_TO_OID                  0x00000001
#define	NDIS_GUID_TO_STATUS               0x00000002
#define	NDIS_GUID_ANSI_STRING             0x00000004
#define	NDIS_GUID_UNICODE_STRING          0x00000008
#define	NDIS_GUID_ARRAY                   0x00000010

#if NDIS_LEGACY_DRIVER

/* NDIS_PACKET_PRIVATE.Flags constants */
#define fPACKET_WRAPPER_RESERVED             0x3f
#define fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO 0x40
#define fPACKET_ALLOCATED_BY_NDIS            0x80

#define NDIS_FLAGS_PROTOCOL_ID_MASK          0x0000000f
#define NDIS_FLAGS_MULTICAST_PACKET          0x00000010
#define NDIS_FLAGS_RESERVED2                 0x00000020
#define NDIS_FLAGS_RESERVED3                 0x00000040
#define NDIS_FLAGS_DONT_LOOPBACK             0x00000080
#define NDIS_FLAGS_IS_LOOPBACK_PACKET        0x00000100
#define NDIS_FLAGS_LOOPBACK_ONLY             0x00000200
#define NDIS_FLAGS_RESERVED4                 0x00000400
#define NDIS_FLAGS_DOUBLE_BUFFERED           0x00000800
#define NDIS_FLAGS_SENT_AT_DPC               0x00001000
#define NDIS_FLAGS_USES_SG_BUFFER_LIST       0x00002000
#define NDIS_FLAGS_USES_ORIGINAL_PACKET      0x00004000
#define NDIS_FLAGS_PADDED                    0x00010000
#define NDIS_FLAGS_XLATE_AT_TOP              0x00020000

typedef NDIS_HANDLE PNDIS_PACKET_POOL;

typedef struct _NDIS_PACKET_PRIVATE {
  UINT PhysicalCount;
  UINT TotalLength;
  PNDIS_BUFFER Head;
  PNDIS_BUFFER Tail;
  PNDIS_PACKET_POOL Pool;
  UINT Count;
  ULONG Flags;
  BOOLEAN ValidCounts;
  UCHAR NdisPacketFlags;
  USHORT NdisPacketOobOffset;
} NDIS_PACKET_PRIVATE, *PNDIS_PACKET_PRIVATE;

typedef struct _NDIS_PACKET {
  NDIS_PACKET_PRIVATE Private;
  __MINGW_EXTENSION union {
    __MINGW_EXTENSION struct {
      UCHAR MiniportReserved[2 * sizeof(PVOID)];
      UCHAR WrapperReserved[2 * sizeof(PVOID)];
    };
    __MINGW_EXTENSION struct {
      UCHAR MiniportReservedEx[3 * sizeof(PVOID)];
      UCHAR WrapperReservedEx[sizeof(PVOID)];
    };
    __MINGW_EXTENSION struct {
      UCHAR MacReserved[4 * sizeof(PVOID)];
    };
  };
  ULONG_PTR Reserved[2];
  UCHAR ProtocolReserved[1];
} NDIS_PACKET, *PNDIS_PACKET, **PPNDIS_PACKET;

typedef struct _NDIS_PACKET_STACK {
  ULONG_PTR IMReserved[2];
  ULONG_PTR NdisReserved[4];
} NDIS_PACKET_STACK, *PNDIS_PACKET_STACK;

#endif /* NDIS_LEGACY_DRIVER */

typedef enum _NDIS_CLASS_ID {
  NdisClass802_3Priority,
  NdisClassWirelessWanMbxMailbox,
  NdisClassIrdaPacketInfo,
  NdisClassAtmAALInfo
} NDIS_CLASS_ID;

typedef struct _MEDIA_SPECIFIC_INFORMATION {
  UINT NextEntryOffset;
  NDIS_CLASS_ID ClassId;
  UINT Size;
  UCHAR ClassInformation[1];
} MEDIA_SPECIFIC_INFORMATION, *PMEDIA_SPECIFIC_INFORMATION;

#if NDIS_LEGACY_DRIVER
typedef struct _NDIS_PACKET_OOB_DATA {
  __MINGW_EXTENSION union {
    ULONGLONG TimeToSend;
    ULONGLONG TimeSent;
  };
  ULONGLONG TimeReceived;
  UINT HeaderSize;
  UINT SizeMediaSpecificInfo;
  PVOID MediaSpecificInformation;
  NDIS_STATUS Status;
} NDIS_PACKET_OOB_DATA, *PNDIS_PACKET_OOB_DATA;
#endif

/* Request types used by NdisRequest */
typedef enum _NDIS_REQUEST_TYPE {
  NdisRequestQueryInformation,
  NdisRequestSetInformation,
  NdisRequestQueryStatistics,
  NdisRequestOpen,
  NdisRequestClose,
  NdisRequestSend,
  NdisRequestTransferData,
  NdisRequestReset,
  NdisRequestGeneric1,
  NdisRequestGeneric2,
  NdisRequestGeneric3,
  NdisRequestGeneric4,
#if NDIS_SUPPORT_NDIS6
  NdisRequestMethod,
#endif
} NDIS_REQUEST_TYPE, *PNDIS_REQUEST_TYPE;

#if NDIS_LEGACY_DRIVER
typedef struct _NDIS_REQUEST {
  UCHAR MacReserved[4 * sizeof(PVOID)];
  NDIS_REQUEST_TYPE RequestType;
  union _DATA {
    struct QUERY_INFORMATION {
      NDIS_OID Oid;
      PVOID InformationBuffer;
      UINT InformationBufferLength;
      UINT BytesWritten;
      UINT BytesNeeded;
    } QUERY_INFORMATION;
    struct SET_INFORMATION {
      NDIS_OID Oid;
      PVOID InformationBuffer;
      UINT InformationBufferLength;
      UINT BytesRead;
      UINT BytesNeeded;
    } SET_INFORMATION;
 } DATA;
#if (defined(NDIS50) || defined(NDIS51) || defined(NDIS50_MINIPORT) || defined(NDIS51_MINIPORT))
  UCHAR NdisReserved[9 * sizeof(PVOID)];
  __MINGW_EXTENSION union {
    UCHAR CallMgrReserved[2 * sizeof(PVOID)];
    UCHAR ProtocolReserved[2 * sizeof(PVOID)];
  };
  UCHAR MiniportReserved[2 * sizeof(PVOID)];
#endif
} NDIS_REQUEST, *PNDIS_REQUEST;
#endif /* NDIS_LEGACY_DRIVER */

/* Wide Area Networks definitions */

#if NDIS_LEGACY_DRIVER
typedef struct _NDIS_WAN_PACKET {
  LIST_ENTRY WanPacketQueue;
  PUCHAR CurrentBuffer;
  ULONG CurrentLength;
  PUCHAR StartBuffer;
  PUCHAR EndBuffer;
  PVOID ProtocolReserved1;
  PVOID ProtocolReserved2;
  PVOID ProtocolReserved3;
  PVOID ProtocolReserved4;
  PVOID MacReserved1;
  PVOID MacReserved2;
  PVOID MacReserved3;
  PVOID MacReserved4;
} NDIS_WAN_PACKET, *PNDIS_WAN_PACKET;
#endif

/* DMA channel information */

typedef struct _NDIS_DMA_DESCRIPTION {
  BOOLEAN  DemandMode;
  BOOLEAN  AutoInitialize;
  BOOLEAN  DmaChannelSpecified;
  DMA_WIDTH  DmaWidth;
  DMA_SPEED  DmaSpeed;
  ULONG  DmaPort;
  ULONG  DmaChannel;
} NDIS_DMA_DESCRIPTION, *PNDIS_DMA_DESCRIPTION;

typedef struct _NDIS_DMA_BLOCK {
  PVOID  MapRegisterBase;
  KEVENT  AllocationEvent;
  PADAPTER_OBJECT  SystemAdapterObject;
  PVOID  Miniport;
  BOOLEAN  InProgress;
} NDIS_DMA_BLOCK, *PNDIS_DMA_BLOCK;

typedef UCHAR NDIS_DMA_SIZE;

#define NDIS_DMA_24BITS                         ((NDIS_DMA_SIZE)0)
#define NDIS_DMA_32BITS                         ((NDIS_DMA_SIZE)1)
#define NDIS_DMA_64BITS                         ((NDIS_DMA_SIZE)2)

typedef enum _NDIS_PROCESSOR_TYPE {
  NdisProcessorX86,
  NdisProcessorMips,
  NdisProcessorAlpha,
  NdisProcessorPpc,
  NdisProcessorAmd64,
  NdisProcessorIA64
} NDIS_PROCESSOR_TYPE, *PNDIS_PROCESSOR_TYPE;

typedef enum _NDIS_ENVIRONMENT_TYPE {
  NdisEnvironmentWindows,
  NdisEnvironmentWindowsNt
} NDIS_ENVIRONMENT_TYPE, *PNDIS_ENVIRONMENT_TYPE;

/* Possible hardware architecture */
typedef enum _NDIS_INTERFACE_TYPE {
  NdisInterfaceInternal = Internal,
  NdisInterfaceIsa = Isa,
  NdisInterfaceEisa = Eisa,
  NdisInterfaceMca = MicroChannel,
  NdisInterfaceTurboChannel = TurboChannel,
  NdisInterfacePci = PCIBus,
  NdisInterfacePcMcia = PCMCIABus,
  NdisInterfaceCBus = CBus,
  NdisInterfaceMPIBus = MPIBus,
  NdisInterfaceMPSABus = MPSABus,
  NdisInterfaceProcessorInternal = ProcessorInternal,
  NdisInterfaceInternalPowerBus = InternalPowerBus,
  NdisInterfacePNPISABus = PNPISABus,
  NdisInterfacePNPBus = PNPBus,
  NdisInterfaceUSB,
  NdisInterfaceIrda,
  NdisInterface1394,
  NdisMaximumInterfaceType
} NDIS_INTERFACE_TYPE, *PNDIS_INTERFACE_TYPE;

#define NdisInterruptLevelSensitive       LevelSensitive
#define NdisInterruptLatched              Latched

typedef KINTERRUPT_MODE NDIS_INTERRUPT_MODE, *PNDIS_INTERRUPT_MODE;

typedef enum _NDIS_PARAMETER_TYPE {
  NdisParameterInteger,
  NdisParameterHexInteger,
  NdisParameterString,
  NdisParameterMultiString,
  NdisParameterBinary
} NDIS_PARAMETER_TYPE, *PNDIS_PARAMETER_TYPE;

typedef struct _BINARY_DATA {
  USHORT Length;
  _Field_size_bytes_(Length) PVOID Buffer;
} BINARY_DATA;

typedef struct _NDIS_CONFIGURATION_PARAMETER {
  NDIS_PARAMETER_TYPE ParameterType;
  union {
    ULONG IntegerData;
    NDIS_STRING StringData;
    BINARY_DATA BinaryData;
  } ParameterData;
} NDIS_CONFIGURATION_PARAMETER, *PNDIS_CONFIGURATION_PARAMETER;

typedef PHYSICAL_ADDRESS NDIS_PHYSICAL_ADDRESS, *PNDIS_PHYSICAL_ADDRESS;

typedef struct _NDIS_PHYSICAL_ADDRESS_UNIT {
  NDIS_PHYSICAL_ADDRESS PhysicalAddress;
  UINT Length;
} NDIS_PHYSICAL_ADDRESS_UNIT, *PNDIS_PHYSICAL_ADDRESS_UNIT;

typedef struct _NDIS_WAN_LINE_DOWN {
  UCHAR RemoteAddress[6];
  UCHAR LocalAddress[6];
} NDIS_WAN_LINE_DOWN, *PNDIS_WAN_LINE_DOWN;

typedef struct _NDIS_WAN_LINE_UP {
  ULONG LinkSpeed;
  ULONG MaximumTotalSize;
  NDIS_WAN_QUALITY Quality;
  USHORT SendWindow;
  UCHAR RemoteAddress[6];
  OUT UCHAR LocalAddress[6];
  ULONG ProtocolBufferLength;
  PUCHAR ProtocolBuffer;
  USHORT ProtocolType;
  NDIS_STRING DeviceName;
} NDIS_WAN_LINE_UP, *PNDIS_WAN_LINE_UP;

typedef NTSTATUS
(NTAPI *TDI_REGISTER_CALLBACK)(
  _In_ PUNICODE_STRING DeviceName,
  _Out_ HANDLE *TdiHandle);

typedef NTSTATUS
(NTAPI *TDI_PNP_HANDLER)(
  _In_ PUNICODE_STRING UpperComponent,
  _In_ PUNICODE_STRING LowerComponent,
  _In_ PUNICODE_STRING BindList,
  _In_ PVOID ReconfigBuffer,
  _In_ UINT ReconfigBufferSize,
  _In_ UINT Operation);

typedef struct _OID_LIST    OID_LIST, *POID_LIST;

/* PnP state */

typedef enum _NDIS_PNP_DEVICE_STATE {
  NdisPnPDeviceAdded,
  NdisPnPDeviceStarted,
  NdisPnPDeviceQueryStopped,
  NdisPnPDeviceStopped,
  NdisPnPDeviceQueryRemoved,
  NdisPnPDeviceRemoved,
  NdisPnPDeviceSurpriseRemoved
} NDIS_PNP_DEVICE_STATE;

#define	NDIS_DEVICE_NOT_STOPPABLE                 0x00000001
#define	NDIS_DEVICE_NOT_REMOVEABLE                0x00000002
#define	NDIS_DEVICE_NOT_SUSPENDABLE               0x00000004
#define NDIS_DEVICE_DISABLE_PM                    0x00000008
#define NDIS_DEVICE_DISABLE_WAKE_UP               0x00000010
#define NDIS_DEVICE_DISABLE_WAKE_ON_RECONNECT     0x00000020
#define NDIS_DEVICE_RESERVED                      0x00000040
#define NDIS_DEVICE_DISABLE_WAKE_ON_MAGIC_PACKET  0x00000080
#define NDIS_DEVICE_DISABLE_WAKE_ON_PATTERN_MATCH 0x00000100

/* Protocol types supported by NDIS */
#define	NDIS_PROTOCOL_ID_DEFAULT        0x00
#define	NDIS_PROTOCOL_ID_TCP_IP         0x02
#define	NDIS_PROTOCOL_ID_IPX            0x06
#define	NDIS_PROTOCOL_ID_NBF            0x07
#define	NDIS_PROTOCOL_ID_MAX            0x0F
#define	NDIS_PROTOCOL_ID_MASK           0x0F

typedef ULONG NDIS_AF, *PNDIS_AF;

#define CO_ADDRESS_FAMILY_Q2931           ((NDIS_AF)0x1)
#define CO_ADDRESS_FAMILY_PSCHED          ((NDIS_AF)0x2)
#define CO_ADDRESS_FAMILY_L2TP            ((NDIS_AF)0x3)
#define CO_ADDRESS_FAMILY_IRDA            ((NDIS_AF)0x4)
#define CO_ADDRESS_FAMILY_1394            ((NDIS_AF)0x5)
#define CO_ADDRESS_FAMILY_PPP             ((NDIS_AF)0x6)
#define CO_ADDRESS_FAMILY_INFINIBAND      ((NDIS_AF)0x7)
#define CO_ADDRESS_FAMILY_TAPI            ((NDIS_AF)0x800)
#define CO_ADDRESS_FAMILY_TAPI_PROXY      ((NDIS_AF)0x801)

#define CO_ADDRESS_FAMILY_PROXY           0x80000000

typedef struct _CO_ADDRESS_FAMILY {
  NDIS_AF AddressFamily;
  ULONG MajorVersion;
  ULONG MinorVersion;
} CO_ADDRESS_FAMILY, *PCO_ADDRESS_FAMILY;

typedef struct _CO_SPECIFIC_PARAMETERS {
  ULONG  ParamType;
  ULONG  Length;
  UCHAR  Parameters[1];
} CO_SPECIFIC_PARAMETERS, *PCO_SPECIFIC_PARAMETERS;

typedef struct _CO_CALL_MANAGER_PARAMETERS {
  FLOWSPEC  Transmit;
  FLOWSPEC  Receive;
  CO_SPECIFIC_PARAMETERS  CallMgrSpecific;
} CO_CALL_MANAGER_PARAMETERS, *PCO_CALL_MANAGER_PARAMETERS;

/* CO_MEDIA_PARAMETERS.Flags constants */
#define RECEIVE_TIME_INDICATION           0x00000001
#define USE_TIME_STAMPS                   0x00000002
#define TRANSMIT_VC                       0x00000004
#define RECEIVE_VC                        0x00000008
#define INDICATE_ERRED_PACKETS            0x00000010
#define INDICATE_END_OF_TX                0x00000020
#define RESERVE_RESOURCES_VC              0x00000040
#define	ROUND_DOWN_FLOW                   0x00000080
#define	ROUND_UP_FLOW                     0x00000100

typedef struct _CO_MEDIA_PARAMETERS {
  ULONG  Flags;
  ULONG  ReceivePriority;
  ULONG  ReceiveSizeHint;
  CO_SPECIFIC_PARAMETERS  MediaSpecific;
} CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;

/* CO_CALL_PARAMETERS.Flags constants */
#define PERMANENT_VC                      0x00000001
#define CALL_PARAMETERS_CHANGED           0x00000002
#define QUERY_CALL_PARAMETERS             0x00000004
#define BROADCAST_VC                      0x00000008
#define MULTIPOINT_VC                     0x00000010

typedef struct _CO_CALL_PARAMETERS {
  ULONG  Flags;
  PCO_CALL_MANAGER_PARAMETERS  CallMgrParameters;
  PCO_MEDIA_PARAMETERS  MediaParameters;
} CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;

typedef struct _CO_SAP {
  ULONG SapType;
  ULONG SapLength;
  UCHAR Sap[1];
} CO_SAP, *PCO_SAP;

#if NDIS_LEGACY_DRIVER
typedef struct _NDIS_IPSEC_PACKET_INFO {
  __MINGW_EXTENSION union {
    struct {
      NDIS_HANDLE OffloadHandle;
      NDIS_HANDLE NextOffloadHandle;
    } Transmit;
    struct {
      ULONG SA_DELETE_REQ:1;
      ULONG CRYPTO_DONE:1;
      ULONG NEXT_CRYPTO_DONE:1;
      ULONG CryptoStatus;
    } Receive;
  };
} NDIS_IPSEC_PACKET_INFO, *PNDIS_IPSEC_PACKET_INFO;
#endif

#if (NDIS_SUPPORT_NDIS6 || NDIS60)
typedef struct _NDIS_IPSEC_OFFLOAD_V1_NET_BUFFER_LIST_INFO {
  __MINGW_EXTENSION union {
    struct {
      NDIS_HANDLE OffloadHandle;
    } Transmit;
    struct {
      USHORT SaDeleteReq:1;
      USHORT CryptoDone:1;
      USHORT NextCryptoDone:1;
      USHORT Pad:13;
      USHORT CryptoStatus;
    } Receive;
  };
} NDIS_IPSEC_OFFLOAD_V1_NET_BUFFER_LIST_INFO, *PNDIS_IPSEC_OFFLOAD_V1_NET_BUFFER_LIST_INFO;
#endif

/* NDIS_MAC_FRAGMENT.Errors constants */
#define WAN_ERROR_CRC               			0x00000001
#define WAN_ERROR_FRAMING           			0x00000002
#define WAN_ERROR_HARDWAREOVERRUN   			0x00000004
#define WAN_ERROR_BUFFEROVERRUN     			0x00000008
#define WAN_ERROR_TIMEOUT           			0x00000010
#define WAN_ERROR_ALIGNMENT         			0x00000020

typedef struct _NDIS_MAC_FRAGMENT {
  NDIS_HANDLE  NdisLinkContext;
  ULONG  Errors;
} NDIS_MAC_FRAGMENT, *PNDIS_MAC_FRAGMENT;

typedef struct _NDIS_MAC_LINE_DOWN {
  NDIS_HANDLE  NdisLinkContext;
} NDIS_MAC_LINE_DOWN, *PNDIS_MAC_LINE_DOWN;

typedef struct _NDIS_MAC_LINE_UP {
  ULONG  LinkSpeed;
  NDIS_WAN_QUALITY  Quality;
  USHORT  SendWindow;
  NDIS_HANDLE  ConnectionWrapperID;
  NDIS_HANDLE  NdisLinkHandle;
  NDIS_HANDLE  NdisLinkContext;
} NDIS_MAC_LINE_UP, *PNDIS_MAC_LINE_UP;

typedef struct _NDIS_PACKET_8021Q_INFO {
  __MINGW_EXTENSION union {
    struct {
      UINT32 UserPriority:3;
      UINT32 CanonicalFormatId:1;
      UINT32 VlanId:12;
      UINT32 Reserved:16;
    } TagHeader;
    PVOID Value;
  };
} NDIS_PACKET_8021Q_INFO, *PNDIS_PACKET_8021Q_INFO;

typedef enum _NDIS_PER_PACKET_INFO {
  TcpIpChecksumPacketInfo,
  IpSecPacketInfo,
  TcpLargeSendPacketInfo,
  ClassificationHandlePacketInfo,
  NdisReserved,
  ScatterGatherListPacketInfo,
  Ieee8021QInfo,
  OriginalPacketInfo,
  PacketCancelId,
  OriginalNetBufferList,
  CachedNetBufferList,
  ShortPacketPaddingInfo,
  MaxPerPacketInfo
} NDIS_PER_PACKET_INFO, *PNDIS_PER_PACKET_INFO;

#if NDIS_LEGACY_DRIVER

typedef struct _NDIS_PACKET_EXTENSION {
  PVOID NdisPacketInfo[MaxPerPacketInfo];
} NDIS_PACKET_EXTENSION, *PNDIS_PACKET_EXTENSION;

typedef enum _NDIS_TASK {
  TcpIpChecksumNdisTask,
  IpSecNdisTask,
  TcpLargeSendNdisTask,
  MaxNdisTask
} NDIS_TASK, *PNDIS_TASK;

typedef enum _NDIS_ENCAPSULATION {
  UNSPECIFIED_Encapsulation,
  NULL_Encapsulation,
  IEEE_802_3_Encapsulation,
  IEEE_802_5_Encapsulation,
  LLC_SNAP_ROUTED_Encapsulation,
  LLC_SNAP_BRIDGED_Encapsulation
} NDIS_ENCAPSULATION;

typedef struct _NDIS_ENCAPSULATION_FORMAT {
  NDIS_ENCAPSULATION Encapsulation;
  struct {
    ULONG FixedHeaderSize:1;
    ULONG Reserved:31;
  } Flags;
  ULONG EncapsulationHeaderSize;
} NDIS_ENCAPSULATION_FORMAT, *PNDIS_ENCAPSULATION_FORMAT;

typedef struct _NDIS_TASK_OFFLOAD_HEADER {
  ULONG Version;
  ULONG Size;
  ULONG Reserved;
  ULONG OffsetFirstTask;
  NDIS_ENCAPSULATION_FORMAT EncapsulationFormat;
} NDIS_TASK_OFFLOAD_HEADER, *PNDIS_TASK_OFFLOAD_HEADER;

typedef struct _NDIS_TASK_OFFLOAD {
  ULONG Version;
  ULONG Size;
  NDIS_TASK Task;
  ULONG OffsetNextTask;
  ULONG TaskBufferLength;
  UCHAR TaskBuffer[1];
} NDIS_TASK_OFFLOAD, *PNDIS_TASK_OFFLOAD;

typedef struct _NDIS_TASK_TCP_IP_CHECKSUM {
  struct {
    ULONG IpOptionsSupported:1;
    ULONG TcpOptionsSupported:1;
    ULONG TcpChecksum:1;
    ULONG UdpChecksum:1;
    ULONG IpChecksum:1;
  } V4Transmit;
  struct {
    ULONG IpOptionsSupported:1;
    ULONG TcpOptionsSupported:1;
    ULONG TcpChecksum:1;
    ULONG UdpChecksum:1;
    ULONG IpChecksum:1;
  } V4Receive;
  struct {
    ULONG IpOptionsSupported:1;
    ULONG TcpOptionsSupported:1;
    ULONG TcpChecksum:1;
    ULONG UdpChecksum:1;
  } V6Transmit;
  struct {
    ULONG IpOptionsSupported:1;
    ULONG TcpOptionsSupported:1;
    ULONG TcpChecksum:1;
    ULONG UdpChecksum:1;
  } V6Receive;
} NDIS_TASK_TCP_IP_CHECKSUM, *PNDIS_TASK_TCP_IP_CHECKSUM;

#define NDIS_TASK_TCP_LARGE_SEND_V0 0

typedef struct _NDIS_TASK_TCP_LARGE_SEND {
  ULONG Version;
  ULONG MaxOffLoadSize;
  ULONG MinSegmentCount;
  BOOLEAN TcpOptions;
  BOOLEAN IpOptions;
} NDIS_TASK_TCP_LARGE_SEND, *PNDIS_TASK_TCP_LARGE_SEND;

typedef struct _NDIS_TASK_IPSEC {
  struct {
    ULONG AH_ESP_COMBINED;
    ULONG TRANSPORT_TUNNEL_COMBINED;
    ULONG V4_OPTIONS;
    ULONG RESERVED;
  } Supported;
  struct {
    ULONG MD5:1;
    ULONG SHA_1:1;
    ULONG Transport:1;
    ULONG Tunnel:1;
    ULONG Send:1;
    ULONG Receive:1;
  } V4AH;
  struct {
    ULONG DES:1;
    ULONG RESERVED:1;
    ULONG TRIPLE_DES:1;
    ULONG NULL_ESP:1;
    ULONG Transport:1;
    ULONG Tunnel:1;
    ULONG Send:1;
    ULONG Receive:1;
  } V4ESP;
} NDIS_TASK_IPSEC, *PNDIS_TASK_IPSEC;

#endif /* NDIS_LEGACY_DRIVER */

#define IPSEC_TPT_UDPESP_ENCAPTYPE_IKE                 0x00000001
#define IPSEC_TUN_UDPESP_ENCAPTYPE_IKE                 0x00000002
#define IPSEC_TPTOVERTUN_UDPESP_ENCAPTYPE_IKE          0x00000004
#define IPSEC_TPT_UDPESP_OVER_PURE_TUN_ENCAPTYPE_IKE   0x00000008
#define IPSEC_TPT_UDPESP_ENCAPTYPE_OTHER               0x00000010
#define IPSEC_TUN_UDPESP_ENCAPTYPE_OTHER               0x00000020
#define IPSEC_TPTOVERTUN_UDPESP_ENCAPTYPE_OTHER        0x00000040
#define IPSEC_TPT_UDPESP_OVER_PURE_TUN_ENCAPTYPE_OTHER 0x00000080

#if NDIS_LEGACY_DRIVER

/*
 * PNDIS_PACKET
 * NDIS_GET_ORIGINAL_PACKET(
 *   IN PNDIS_PACKET  Packet);
 */
#define NDIS_GET_ORIGINAL_PACKET(Packet) \
  NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, OriginalPacketInfo)

/*
 * PVOID
 * NDIS_GET_PACKET_CANCEL_ID(
 *   IN PNDIS_PACKET  Packet);
 */
#define NDIS_GET_PACKET_CANCEL_ID(Packet) \
  NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, PacketCancelId)

/*
 * PNDIS_PACKET_EXTENSION
 * NDIS_PACKET_EXTENSION_FROM_PACKET(
 *   IN PNDIS_PACKET  Packet);
 */
#define NDIS_PACKET_EXTENSION_FROM_PACKET(Packet) \
  ((PNDIS_PACKET_EXTENSION)((PUCHAR)(Packet) \
    + (Packet)->Private.NdisPacketOobOffset + sizeof(NDIS_PACKET_OOB_DATA)))

/*
 * PVOID
 * NDIS_PER_PACKET_INFO_FROM_PACKET(
 *   IN OUT  PNDIS_PACKET  Packet,
 *   IN NDIS_PER_PACKET_INFO  InfoType);
 */
#define NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, InfoType) \
  ((PNDIS_PACKET_EXTENSION)((PUCHAR)(Packet) + (Packet)->Private.NdisPacketOobOffset \
    + sizeof(NDIS_PACKET_OOB_DATA)))->NdisPacketInfo[(InfoType)]

/*
 * VOID
 * NDIS_SET_ORIGINAL_PACKET(
 *   IN OUT  PNDIS_PACKET  Packet,
 *   IN PNDIS_PACKET  OriginalPacket);
 */
#define NDIS_SET_ORIGINAL_PACKET(Packet, OriginalPacket) \
  NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, OriginalPacketInfo) = (OriginalPacket)

/*
 * VOID
 * NDIS_SET_PACKET_CANCEL_ID(
 *  IN PNDIS_PACKET  Packet
 *  IN ULONG_PTR  CancelId);
 */
#define NDIS_SET_PACKET_CANCEL_ID(Packet, CancelId) \
  NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, PacketCancelId) = (CancelId)

#define NdisSetPacketCancelId(_Packet, _CancelId) NDIS_SET_PACKET_CANCEL_ID(_Packet, _CancelId)
#define NdisGetPacketCancelId(_Packet) NDIS_GET_PACKET_CANCEL_ID(_Packet)

#endif /* NDIS_LEGACY_DRIVER */

#if NDIS_SUPPORT_NDIS6
typedef struct _NDIS_GENERIC_OBJECT {
  NDIS_OBJECT_HEADER Header;
  PVOID Caller;
  PVOID CallersCaller;
  PDRIVER_OBJECT DriverObject;
} NDIS_GENERIC_OBJECT, *PNDIS_GENERIC_OBJECT;
#endif

/* NDIS_TASK_OFFLOAD_HEADER.Version constants */
#define NDIS_TASK_OFFLOAD_VERSION 1

#define MAX_HASHES                     4
#define TRUNCATED_HASH_LEN             12

#define CRYPTO_SUCCESS                   0
#define CRYPTO_GENERIC_ERROR             1
#define CRYPTO_TRANSPORT_AH_AUTH_FAILED  2
#define CRYPTO_TRANSPORT_ESP_AUTH_FAILED 3
#define CRYPTO_TUNNEL_AH_AUTH_FAILED     4
#define CRYPTO_TUNNEL_ESP_AUTH_FAILED    5
#define CRYPTO_INVALID_PACKET_SYNTAX     6
#define CRYPTO_INVALID_PROTOCOL          7

typedef struct _NDIS_TCP_IP_CHECKSUM_PACKET_INFO {
  __MINGW_EXTENSION union {
    struct {
      ULONG NdisPacketChecksumV4:1;
      ULONG NdisPacketChecksumV6:1;
      ULONG NdisPacketTcpChecksum:1;
      ULONG NdisPacketUdpChecksum:1;
      ULONG NdisPacketIpChecksum:1;
      } Transmit;
    struct {
      ULONG NdisPacketTcpChecksumFailed:1;
      ULONG NdisPacketUdpChecksumFailed:1;
      ULONG NdisPacketIpChecksumFailed:1;
      ULONG NdisPacketTcpChecksumSucceeded:1;
      ULONG NdisPacketUdpChecksumSucceeded:1;
      ULONG NdisPacketIpChecksumSucceeded:1;
      ULONG NdisPacketLoopback:1;
    } Receive;
    ULONG Value;
  };
} NDIS_TCP_IP_CHECKSUM_PACKET_INFO, *PNDIS_TCP_IP_CHECKSUM_PACKET_INFO;

typedef struct _NDIS_WAN_CO_FRAGMENT {
  ULONG Errors;
} NDIS_WAN_CO_FRAGMENT, *PNDIS_WAN_CO_FRAGMENT;

typedef struct _NDIS_WAN_FRAGMENT {
  UCHAR RemoteAddress[6];
  UCHAR LocalAddress[6];
} NDIS_WAN_FRAGMENT, *PNDIS_WAN_FRAGMENT;

typedef struct _WAN_CO_LINKPARAMS {
  ULONG TransmitSpeed;
  ULONG ReceiveSpeed;
  ULONG SendWindow;
} WAN_CO_LINKPARAMS, *PWAN_CO_LINKPARAMS;

typedef struct _NDIS_WAN_GET_STATS {
  UCHAR LocalAddress[6];
  ULONG BytesSent;
  ULONG BytesRcvd;
  ULONG FramesSent;
  ULONG FramesRcvd;
  ULONG CRCErrors;
  ULONG TimeoutErrors;
  ULONG AlignmentErrors;
  ULONG SerialOverrunErrors;
  ULONG FramingErrors;
  ULONG BufferOverrunErrors;
  ULONG BytesTransmittedUncompressed;
  ULONG BytesReceivedUncompressed;
  ULONG BytesTransmittedCompressed;
  ULONG BytesReceivedCompressed;
} NDIS_WAN_GET_STATS, *PNDIS_WAN_GET_STATS;

/* Call Manager */

typedef VOID
(NTAPI *CM_ACTIVATE_VC_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  CallMgrVcContext,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef NDIS_STATUS
(NTAPI *CM_ADD_PARTY_HANDLER)(
  IN NDIS_HANDLE  CallMgrVcContext,
  IN OUT PCO_CALL_PARAMETERS  CallParameters,
  IN NDIS_HANDLE  NdisPartyHandle,
  OUT PNDIS_HANDLE  CallMgrPartyContext);

typedef NDIS_STATUS
(NTAPI *CM_CLOSE_AF_HANDLER)(
  IN NDIS_HANDLE  CallMgrAfContext);

typedef NDIS_STATUS
(NTAPI *CM_CLOSE_CALL_HANDLER)(
  IN NDIS_HANDLE  CallMgrVcContext,
  IN NDIS_HANDLE  CallMgrPartyContext  OPTIONAL,
  IN PVOID  CloseData  OPTIONAL,
  IN UINT  Size  OPTIONAL);

typedef NDIS_STATUS
(NTAPI *CM_DEREG_SAP_HANDLER)(
  IN NDIS_HANDLE  CallMgrSapContext);

typedef VOID
(NTAPI *CM_DEACTIVATE_VC_COMPLETE_HANDLER)(
	IN NDIS_STATUS  Status,
	IN NDIS_HANDLE  CallMgrVcContext);

typedef NDIS_STATUS
(NTAPI *CM_DROP_PARTY_HANDLER)(
  IN NDIS_HANDLE  CallMgrPartyContext,
  IN PVOID  CloseData  OPTIONAL,
  IN UINT  Size  OPTIONAL);

typedef VOID
(NTAPI *CM_INCOMING_CALL_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  CallMgrVcContext,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef NDIS_STATUS
(NTAPI *CM_MAKE_CALL_HANDLER)(
  IN NDIS_HANDLE  CallMgrVcContext,
  IN OUT PCO_CALL_PARAMETERS  CallParameters,
  IN NDIS_HANDLE  NdisPartyHandle	OPTIONAL,
  OUT PNDIS_HANDLE  CallMgrPartyContext  OPTIONAL);

typedef NDIS_STATUS
(NTAPI *CM_MODIFY_CALL_QOS_HANDLER)(
  IN NDIS_HANDLE  CallMgrVcContext,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef NDIS_STATUS
(NTAPI *CM_OPEN_AF_HANDLER)(
	IN NDIS_HANDLE  CallMgrBindingContext,
	IN PCO_ADDRESS_FAMILY  AddressFamily,
	IN NDIS_HANDLE  NdisAfHandle,
	OUT PNDIS_HANDLE  CallMgrAfContext);

typedef NDIS_STATUS
(NTAPI *CM_REG_SAP_HANDLER)(
  IN NDIS_HANDLE  CallMgrAfContext,
  IN PCO_SAP  Sap,
  IN NDIS_HANDLE  NdisSapHandle,
  OUT	PNDIS_HANDLE  CallMgrSapContext);

typedef NDIS_STATUS
(NTAPI *CO_CREATE_VC_HANDLER)(
  IN NDIS_HANDLE  ProtocolAfContext,
  IN NDIS_HANDLE  NdisVcHandle,
  OUT PNDIS_HANDLE  ProtocolVcContext);

typedef NDIS_STATUS
(NTAPI *CO_DELETE_VC_HANDLER)(
  IN NDIS_HANDLE  ProtocolVcContext);

#define PROTOCOL_RESERVED_SIZE_IN_PACKET (4 * sizeof(PVOID))

/* Prototypes for NDIS 5.0 protocol characteristics */

_IRQL_requires_(PASSIVE_LEVEL)
typedef VOID
(NTAPI *CO_SEND_COMPLETE_HANDLER)(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE ProtocolVcContext,
  _In_ PNDIS_PACKET Packet);

_IRQL_requires_max_(DISPATCH_LEVEL)
typedef VOID
(NTAPI *CO_STATUS_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_opt_ NDIS_HANDLE ProtocolVcContext,
  _In_ NDIS_STATUS GeneralStatus,
  _In_ PVOID StatusBuffer,
  _In_ UINT StatusBufferSize);

_IRQL_requires_max_(DISPATCH_LEVEL)
typedef UINT
(NTAPI *CO_RECEIVE_PACKET_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ NDIS_HANDLE ProtocolVcContext,
  _In_ PNDIS_PACKET Packet);

_IRQL_requires_max_(DISPATCH_LEVEL)
typedef NDIS_STATUS
(NTAPI *CO_REQUEST_HANDLER)(
  _In_ NDIS_HANDLE ProtocolAfContext,
  _In_opt_ NDIS_HANDLE ProtocolVcContext,
  _In_opt_ NDIS_HANDLE ProtocolPartyContext,
  _Inout_ PNDIS_REQUEST NdisRequest);

_IRQL_requires_max_(DISPATCH_LEVEL)
typedef VOID
(NTAPI *CO_REQUEST_COMPLETE_HANDLER)(
  _In_ NDIS_STATUS Status,
  _In_opt_ NDIS_HANDLE ProtocolAfContext,
  _In_opt_ NDIS_HANDLE ProtocolVcContext,
  _In_opt_ NDIS_HANDLE ProtocolPartyContext,
  _In_ PNDIS_REQUEST NdisRequest);

typedef struct _NDIS_CALL_MANAGER_CHARACTERISTICS {
	UCHAR  MajorVersion;
	UCHAR  MinorVersion;
	USHORT  Filler;
	UINT  Reserved;
	CO_CREATE_VC_HANDLER  CmCreateVcHandler;
	CO_DELETE_VC_HANDLER  CmDeleteVcHandler;
	CM_OPEN_AF_HANDLER  CmOpenAfHandler;
	CM_CLOSE_AF_HANDLER	 CmCloseAfHandler;
	CM_REG_SAP_HANDLER  CmRegisterSapHandler;
	CM_DEREG_SAP_HANDLER  CmDeregisterSapHandler;
	CM_MAKE_CALL_HANDLER  CmMakeCallHandler;
	CM_CLOSE_CALL_HANDLER  CmCloseCallHandler;
	CM_INCOMING_CALL_COMPLETE_HANDLER  CmIncomingCallCompleteHandler;
	CM_ADD_PARTY_HANDLER  CmAddPartyHandler;
	CM_DROP_PARTY_HANDLER  CmDropPartyHandler;
	CM_ACTIVATE_VC_COMPLETE_HANDLER  CmActivateVcCompleteHandler;
	CM_DEACTIVATE_VC_COMPLETE_HANDLER  CmDeactivateVcCompleteHandler;
	CM_MODIFY_CALL_QOS_HANDLER  CmModifyCallQoSHandler;
	CO_REQUEST_HANDLER  CmRequestHandler;
	CO_REQUEST_COMPLETE_HANDLER  CmRequestCompleteHandler;
} NDIS_CALL_MANAGER_CHARACTERISTICS, *PNDIS_CALL_MANAGER_CHARACTERISTICS;



/* Call Manager clients */

typedef VOID (*CL_OPEN_AF_COMPLETE_HANDLER)(
  IN NDIS_STATUS Status,
  IN NDIS_HANDLE ProtocolAfContext,
  IN NDIS_HANDLE NdisAfHandle);

typedef VOID
(NTAPI *CL_CLOSE_AF_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolAfContext);

typedef VOID
(NTAPI *CL_REG_SAP_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolSapContext,
  IN PCO_SAP  Sap,
  IN NDIS_HANDLE  NdisSapHandle);

typedef VOID
(NTAPI *CL_DEREG_SAP_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolSapContext);

typedef VOID
(NTAPI *CL_MAKE_CALL_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolVcContext,
  IN NDIS_HANDLE  NdisPartyHandle  OPTIONAL,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef VOID
(NTAPI *CL_MODIFY_CALL_QOS_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolVcContext,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef VOID
(NTAPI *CL_CLOSE_CALL_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolVcContext,
  IN NDIS_HANDLE  ProtocolPartyContext  OPTIONAL);

typedef VOID
(NTAPI *CL_ADD_PARTY_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolPartyContext,
  IN NDIS_HANDLE  NdisPartyHandle,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef VOID
(NTAPI *CL_DROP_PARTY_COMPLETE_HANDLER)(
  IN NDIS_STATUS  Status,
  IN NDIS_HANDLE  ProtocolPartyContext);

typedef NDIS_STATUS
(NTAPI *CL_INCOMING_CALL_HANDLER)(
  IN NDIS_HANDLE  ProtocolSapContext,
  IN NDIS_HANDLE  ProtocolVcContext,
  IN OUT PCO_CALL_PARAMETERS  CallParameters);

typedef VOID
(NTAPI *CL_INCOMING_CALL_QOS_CHANGE_HANDLER)(
  IN NDIS_HANDLE  ProtocolVcContext,
  IN PCO_CALL_PARAMETERS  CallParameters);

typedef VOID
(NTAPI *CL_INCOMING_CLOSE_CALL_HANDLER)(
  IN NDIS_STATUS  CloseStatus,
  IN NDIS_HANDLE  ProtocolVcContext,
  IN PVOID  CloseData  OPTIONAL,
  IN UINT  Size  OPTIONAL);

typedef VOID
(NTAPI *CL_INCOMING_DROP_PARTY_HANDLER)(
  IN NDIS_STATUS  DropStatus,
  IN NDIS_HANDLE  ProtocolPartyContext,
  IN PVOID  CloseData  OPTIONAL,
  IN UINT  Size  OPTIONAL);

typedef VOID
(NTAPI *CL_CALL_CONNECTED_HANDLER)(
  IN NDIS_HANDLE  ProtocolVcContext);


typedef struct _NDIS_CLIENT_CHARACTERISTICS {
  UCHAR  MajorVersion;
  UCHAR  MinorVersion;
  USHORT  Filler;
  UINT  Reserved;
  CO_CREATE_VC_HANDLER  ClCreateVcHandler;
  CO_DELETE_VC_HANDLER  ClDeleteVcHandler;
  CO_REQUEST_HANDLER  ClRequestHandler;
  CO_REQUEST_COMPLETE_HANDLER  ClRequestCompleteHandler;
  CL_OPEN_AF_COMPLETE_HANDLER  ClOpenAfCompleteHandler;
  CL_CLOSE_AF_COMPLETE_HANDLER  ClCloseAfCompleteHandler;
  CL_REG_SAP_COMPLETE_HANDLER  ClRegisterSapCompleteHandler;
  CL_DEREG_SAP_COMPLETE_HANDLER  ClDeregisterSapCompleteHandler;
  CL_MAKE_CALL_COMPLETE_HANDLER  ClMakeCallCompleteHandler;
  CL_MODIFY_CALL_QOS_COMPLETE_HANDLER	 ClModifyCallQoSCompleteHandler;
  CL_CLOSE_CALL_COMPLETE_HANDLER  ClCloseCallCompleteHandler;
  CL_ADD_PARTY_COMPLETE_HANDLER  ClAddPartyCompleteHandler;
  CL_DROP_PARTY_COMPLETE_HANDLER  ClDropPartyCompleteHandler;
  CL_INCOMING_CALL_HANDLER  ClIncomingCallHandler;
  CL_INCOMING_CALL_QOS_CHANGE_HANDLER  ClIncomingCallQoSChangeHandler;
  CL_INCOMING_CLOSE_CALL_HANDLER  ClIncomingCloseCallHandler;
  CL_INCOMING_DROP_PARTY_HANDLER  ClIncomingDropPartyHandler;
  CL_CALL_CONNECTED_HANDLER  ClCallConnectedHandler;
} NDIS_CLIENT_CHARACTERISTICS, *PNDIS_CLIENT_CHARACTERISTICS;


/* NDIS protocol structures */

/* Prototypes for NDIS 3.0 protocol characteristics */

typedef VOID
(NTAPI *OPEN_ADAPTER_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ NDIS_STATUS Status,
  _In_ NDIS_STATUS OpenErrorStatus);

typedef VOID
(NTAPI *CLOSE_ADAPTER_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *RESET_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *REQUEST_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ PNDIS_REQUEST NdisRequest,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *STATUS_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ NDIS_STATUS GeneralStatus,
  _In_ PVOID StatusBuffer,
  _In_ UINT StatusBufferSize);

typedef VOID
(NTAPI *STATUS_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext);

typedef VOID
(NTAPI *SEND_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ PNDIS_PACKET Packet,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *WAN_SEND_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ PNDIS_WAN_PACKET Packet,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *TRANSFER_DATA_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ PNDIS_PACKET Packet,
  _In_ NDIS_STATUS Status,
  _In_ UINT BytesTransferred);

typedef VOID
(NTAPI *WAN_TRANSFER_DATA_COMPLETE_HANDLER)(
  VOID);

typedef NDIS_STATUS
(NTAPI *RECEIVE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ NDIS_HANDLE MacReceiveContext,
  _In_ PVOID HeaderBuffer,
  _In_ UINT HeaderBufferSize,
  _In_ PVOID LookAheadBuffer,
  _In_ UINT LookaheadBufferSize,
  _In_ UINT PacketSize);

typedef NDIS_STATUS
(NTAPI *WAN_RECEIVE_HANDLER)(
  _In_ NDIS_HANDLE NdisLinkHandle,
  _In_ PUCHAR Packet,
  _In_ ULONG PacketSize);

typedef VOID
(NTAPI *RECEIVE_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE ProtocolBindingContext);

/* Protocol characteristics for NDIS 3.0 protocols */

#define NDIS30_PROTOCOL_CHARACTERISTICS_S \
  UCHAR  MajorNdisVersion; \
  UCHAR  MinorNdisVersion; \
  USHORT  Filler; \
  _ANONYMOUS_UNION union { \
    UINT  Reserved; \
    UINT  Flags; \
  } DUMMYUNIONNAME; \
  OPEN_ADAPTER_COMPLETE_HANDLER  OpenAdapterCompleteHandler; \
  CLOSE_ADAPTER_COMPLETE_HANDLER  CloseAdapterCompleteHandler; \
  _ANONYMOUS_UNION union { \
    SEND_COMPLETE_HANDLER  SendCompleteHandler; \
    WAN_SEND_COMPLETE_HANDLER  WanSendCompleteHandler; \
  } DUMMYUNIONNAME2; \
  _ANONYMOUS_UNION union { \
    TRANSFER_DATA_COMPLETE_HANDLER  TransferDataCompleteHandler; \
    WAN_TRANSFER_DATA_COMPLETE_HANDLER  WanTransferDataCompleteHandler; \
  } DUMMYUNIONNAME3; \
  RESET_COMPLETE_HANDLER  ResetCompleteHandler; \
  REQUEST_COMPLETE_HANDLER  RequestCompleteHandler; \
  _ANONYMOUS_UNION union { \
    RECEIVE_HANDLER	 ReceiveHandler; \
    WAN_RECEIVE_HANDLER  WanReceiveHandler; \
  } DUMMYUNIONNAME4; \
  RECEIVE_COMPLETE_HANDLER  ReceiveCompleteHandler; \
  STATUS_HANDLER  StatusHandler; \
  STATUS_COMPLETE_HANDLER  StatusCompleteHandler; \
  NDIS_STRING  Name;

typedef struct _NDIS30_PROTOCOL_CHARACTERISTICS {
  NDIS30_PROTOCOL_CHARACTERISTICS_S
} NDIS30_PROTOCOL_CHARACTERISTICS, *PNDIS30_PROTOCOL_CHARACTERISTICS;


/* Prototypes for NDIS 4.0 protocol characteristics */

typedef INT
(NTAPI *RECEIVE_PACKET_HANDLER)(
  IN NDIS_HANDLE ProtocolBindingContext,
  IN PNDIS_PACKET Packet);

typedef VOID
(NTAPI *BIND_HANDLER)(
  OUT PNDIS_STATUS Status,
  IN NDIS_HANDLE BindContext,
  IN PNDIS_STRING DeviceName,
  IN PVOID SystemSpecific1,
  IN PVOID SystemSpecific2);

typedef VOID
(NTAPI *UNBIND_HANDLER)(
  OUT PNDIS_STATUS Status,
  IN NDIS_HANDLE ProtocolBindingContext,
  IN NDIS_HANDLE UnbindContext);

typedef NDIS_STATUS
(NTAPI *PNP_EVENT_HANDLER)(
  IN NDIS_HANDLE ProtocolBindingContext,
  IN PNET_PNP_EVENT NetPnPEvent);

typedef VOID
(NTAPI *UNLOAD_PROTOCOL_HANDLER)(
  VOID);

/* Protocol characteristics for NDIS 4.0 protocols */

typedef struct _NDIS40_PROTOCOL_CHARACTERISTICS {
  UCHAR MajorNdisVersion;
  UCHAR MinorNdisVersion;
  USHORT Filler;
  __MINGW_EXTENSION union {
    UINT Reserved;
    UINT Flags;
  };
  OPEN_ADAPTER_COMPLETE_HANDLER OpenAdapterCompleteHandler;
  CLOSE_ADAPTER_COMPLETE_HANDLER CloseAdapterCompleteHandler;
  __MINGW_EXTENSION union {
    SEND_COMPLETE_HANDLER SendCompleteHandler;
    WAN_SEND_COMPLETE_HANDLER WanSendCompleteHandler;
  };
  __MINGW_EXTENSION union {
    TRANSFER_DATA_COMPLETE_HANDLER TransferDataCompleteHandler;
    WAN_TRANSFER_DATA_COMPLETE_HANDLER WanTransferDataCompleteHandler;
  };
  RESET_COMPLETE_HANDLER ResetCompleteHandler;
  REQUEST_COMPLETE_HANDLER RequestCompleteHandler;
  __MINGW_EXTENSION union {
    RECEIVE_HANDLER ReceiveHandler;
    WAN_RECEIVE_HANDLER WanReceiveHandler;
  };
  RECEIVE_COMPLETE_HANDLER ReceiveCompleteHandler;
  STATUS_HANDLER StatusHandler;
  STATUS_COMPLETE_HANDLER StatusCompleteHandler;
  NDIS_STRING Name;
  RECEIVE_PACKET_HANDLER ReceivePacketHandler;
  BIND_HANDLER BindAdapterHandler;
  UNBIND_HANDLER UnbindAdapterHandler;
  PNP_EVENT_HANDLER PnPEventHandler;
  UNLOAD_PROTOCOL_HANDLER UnloadHandler;
} NDIS40_PROTOCOL_CHARACTERISTICS;

typedef VOID
(NTAPI PROTCOL_CO_AF_REGISTER_NOTIFY)(
  IN NDIS_HANDLE ProtocolBindingContext,
  IN PCO_ADDRESS_FAMILY AddressFamily);
typedef PROTCOL_CO_AF_REGISTER_NOTIFY *CO_AF_REGISTER_NOTIFY_HANDLER;

#if NDIS_LEGACY_PROTOCOL

typedef struct _NDIS50_PROTOCOL_CHARACTERISTICS {
#ifdef __cplusplus
  NDIS40_PROTOCOL_CHARACTERISTICS Ndis40Chars;
#else
  NDIS40_PROTOCOL_CHARACTERISTICS;
#endif
  PVOID ReservedHandlers[4];
  CO_SEND_COMPLETE_HANDLER CoSendCompleteHandler;
  CO_STATUS_HANDLER CoStatusHandler;
  CO_RECEIVE_PACKET_HANDLER CoReceivePacketHandler;
  CO_AF_REGISTER_NOTIFY_HANDLER CoAfRegisterNotifyHandler;
} NDIS50_PROTOCOL_CHARACTERISTICS;

#if (defined(NDIS50) || defined(NDIS51))
typedef NDIS50_PROTOCOL_CHARACTERISTICS NDIS_PROTOCOL_CHARACTERISTICS;
#else
typedef NDIS40_PROTOCOL_CHARACTERISTICS NDIS_PROTOCOL_CHARACTERISTICS;
#endif

typedef NDIS_PROTOCOL_CHARACTERISTICS *PNDIS_PROTOCOL_CHARACTERISTICS;

#endif /* NDIS_LEGACY_PROTOCOL */

/* Prototypes for NDIS_MINIPORT_CHARACTERISTICS */

typedef BOOLEAN
(NTAPI *W_CHECK_FOR_HANG_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef VOID
(NTAPI *W_DISABLE_INTERRUPT_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef VOID
(NTAPI *W_ENABLE_INTERRUPT_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef VOID
(NTAPI *W_HALT_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef VOID
(NTAPI *W_HANDLE_INTERRUPT_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef NDIS_STATUS
(NTAPI *W_INITIALIZE_HANDLER)(
  _Out_ PNDIS_STATUS OpenErrorStatus,
  _Out_ PUINT SelectedMediumIndex,
  _In_ PNDIS_MEDIUM MediumArray,
  _In_ UINT MediumArraySize,
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_HANDLE WrapperConfigurationContext);

typedef VOID
(NTAPI *W_ISR_HANDLER)(
  _Out_ PBOOLEAN InterruptRecognized,
  _Out_ PBOOLEAN QueueMiniportHandleInterrupt,
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef NDIS_STATUS
(NTAPI *W_QUERY_INFORMATION_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_OID Oid,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength,
  _Out_ PULONG BytesWritten,
  _Out_ PULONG BytesNeeded);

typedef NDIS_STATUS
(NTAPI *W_RECONFIGURE_HANDLER)(
  _Out_ PNDIS_STATUS OpenErrorStatus,
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_HANDLE WrapperConfigurationContext);

typedef NDIS_STATUS
(NTAPI *W_RESET_HANDLER)(
  _Out_ PBOOLEAN AddressingReset,
  _In_ NDIS_HANDLE MiniportAdapterContext);

typedef NDIS_STATUS
(NTAPI *W_SEND_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ PNDIS_PACKET Packet,
  _In_ UINT Flags);

typedef NDIS_STATUS
(NTAPI *WM_SEND_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_HANDLE NdisLinkHandle,
  _In_ PNDIS_WAN_PACKET Packet);

typedef NDIS_STATUS
(NTAPI *W_SET_INFORMATION_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_OID Oid,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength,
  _Out_ PULONG BytesRead,
  _Out_ PULONG BytesNeeded);

typedef NDIS_STATUS
(NTAPI *W_TRANSFER_DATA_HANDLER)(
  _Out_ PNDIS_PACKET Packet,
  _Out_ PUINT BytesTransferred,
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_HANDLE MiniportReceiveContext,
  _In_ UINT ByteOffset,
  _In_ UINT BytesToTransfer);

typedef NDIS_STATUS
(NTAPI *WM_TRANSFER_DATA_HANDLER)(VOID);

typedef VOID
(NTAPI *ADAPTER_SHUTDOWN_HANDLER)(
  _In_ PVOID ShutdownContext);

typedef VOID
(NTAPI *W_RETURN_PACKET_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ PNDIS_PACKET Packet);

typedef VOID
(NTAPI *W_SEND_PACKETS_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ PPNDIS_PACKET PacketArray,
  _In_ UINT NumberOfPackets);

typedef VOID
(NTAPI *W_ALLOCATE_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ PVOID VirtualAddress,
  _In_ PNDIS_PHYSICAL_ADDRESS PhysicalAddress,
  _In_ ULONG Length,
  _In_ PVOID Context);

/* NDIS structures available only to miniport drivers */

#define NDIS30_MINIPORT_CHARACTERISTICS_S \
  UCHAR  MajorNdisVersion; \
  UCHAR  MinorNdisVersion; \
  UINT  Reserved; \
  W_CHECK_FOR_HANG_HANDLER  CheckForHangHandler; \
  W_DISABLE_INTERRUPT_HANDLER  DisableInterruptHandler; \
  W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler; \
  W_HALT_HANDLER  HaltHandler; \
  W_HANDLE_INTERRUPT_HANDLER  HandleInterruptHandler; \
  W_INITIALIZE_HANDLER  InitializeHandler; \
  W_ISR_HANDLER  ISRHandler; \
  W_QUERY_INFORMATION_HANDLER  QueryInformationHandler; \
  W_RECONFIGURE_HANDLER  ReconfigureHandler; \
  W_RESET_HANDLER  ResetHandler; \
  W_SEND_HANDLER  SendHandler; \
  W_SET_INFORMATION_HANDLER  SetInformationHandler; \
  W_TRANSFER_DATA_HANDLER  TransferDataHandler;

typedef struct _NDIS30_MINIPORT_CHARACTERISTICS {
  NDIS30_MINIPORT_CHARACTERISTICS_S
} NDIS30_MINIPORT_CHARACTERISTICS, *PSNDIS30_MINIPORT_CHARACTERISTICS;

#ifdef __cplusplus

#define NDIS40_MINIPORT_CHARACTERISTICS_S \
  NDIS30_MINIPORT_CHARACTERISTICS  Ndis30Chars; \
  W_RETURN_PACKET_HANDLER  ReturnPacketHandler; \
  W_SEND_PACKETS_HANDLER  SendPacketsHandler; \
  W_ALLOCATE_COMPLETE_HANDLER  AllocateCompleteHandler;

#else /* !__cplusplus */

#define NDIS40_MINIPORT_CHARACTERISTICS_S \
  NDIS30_MINIPORT_CHARACTERISTICS_S \
  W_RETURN_PACKET_HANDLER  ReturnPacketHandler; \
  W_SEND_PACKETS_HANDLER  SendPacketsHandler; \
  W_ALLOCATE_COMPLETE_HANDLER  AllocateCompleteHandler;

#endif /* !__cplusplus */

typedef struct _NDIS40_MINIPORT_CHARACTERISTICS {
  NDIS40_MINIPORT_CHARACTERISTICS_S
} NDIS40_MINIPORT_CHARACTERISTICS, *PNDIS40_MINIPORT_CHARACTERISTICS;

/* Extensions for NDIS 5.0 miniports */

_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_CREATE_VC)
typedef NDIS_STATUS
(NTAPI MINIPORT_CO_CREATE_VC)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_HANDLE NdisVcHandle,
  _Out_ PNDIS_HANDLE MiniportVcContext);
typedef MINIPORT_CO_CREATE_VC *W_CO_CREATE_VC_HANDLER;

_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_DELETE_VC)
typedef NDIS_STATUS
(NTAPI MINIPORT_CO_DELETE_VC)(
  _In_ NDIS_HANDLE MiniportVcContext);
typedef MINIPORT_CO_DELETE_VC *W_CO_DELETE_VC_HANDLER;

_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_ACTIVATE_VC)
typedef NDIS_STATUS
(NTAPI MINIPORT_CO_ACTIVATE_VC)(
  _In_ NDIS_HANDLE MiniportVcContext,
  _Inout_ PCO_CALL_PARAMETERS CallParameters);
typedef MINIPORT_CO_ACTIVATE_VC *W_CO_ACTIVATE_VC_HANDLER;

_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_DEACTIVATE_VC)
typedef NDIS_STATUS
(NTAPI MINIPORT_CO_DEACTIVATE_VC)(
  _In_ NDIS_HANDLE MiniportVcContext);
typedef MINIPORT_CO_DEACTIVATE_VC *W_CO_DEACTIVATE_VC_HANDLER;

typedef VOID
(NTAPI *W_CO_SEND_PACKETS_HANDLER)(
  _In_ NDIS_HANDLE MiniportVcContext,
  _In_ PPNDIS_PACKET PacketArray,
  _In_ UINT NumberOfPackets);

typedef NDIS_STATUS
(NTAPI *W_CO_REQUEST_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_opt_ NDIS_HANDLE MiniportVcContext,
  _Inout_ PNDIS_REQUEST NdisRequest);

#ifdef __cplusplus

#define NDIS50_MINIPORT_CHARACTERISTICS_S \
  NDIS40_MINIPORT_CHARACTERISTICS  Ndis40Chars; \
  W_CO_CREATE_VC_HANDLER  CoCreateVcHandler; \
  W_CO_DELETE_VC_HANDLER  CoDeleteVcHandler; \
  W_CO_ACTIVATE_VC_HANDLER  CoActivateVcHandler; \
  W_CO_DEACTIVATE_VC_HANDLER  CoDeactivateVcHandler; \
  W_CO_SEND_PACKETS_HANDLER  CoSendPacketsHandler; \
  W_CO_REQUEST_HANDLER  CoRequestHandler;

#else /* !__cplusplus */

#define NDIS50_MINIPORT_CHARACTERISTICS_S \
  NDIS40_MINIPORT_CHARACTERISTICS_S \
  W_CO_CREATE_VC_HANDLER  CoCreateVcHandler; \
  W_CO_DELETE_VC_HANDLER  CoDeleteVcHandler; \
  W_CO_ACTIVATE_VC_HANDLER  CoActivateVcHandler; \
  W_CO_DEACTIVATE_VC_HANDLER  CoDeactivateVcHandler; \
  W_CO_SEND_PACKETS_HANDLER  CoSendPacketsHandler; \
  W_CO_REQUEST_HANDLER  CoRequestHandler;

#endif /* !__cplusplus */

typedef struct _NDIS50_MINIPORT_CHARACTERISTICS {
   NDIS50_MINIPORT_CHARACTERISTICS_S
} NDIS50_MINIPORT_CHARACTERISTICS, *PSNDIS50_MINIPORT_CHARACTERISTICS;

/* Extensions for NDIS 5.1 miniports */

typedef VOID
(NTAPI *W_CANCEL_SEND_PACKETS_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ PVOID CancelId);

typedef VOID
(NTAPI *W_PNP_EVENT_NOTIFY_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_DEVICE_PNP_EVENT PnPEvent,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength);

typedef VOID
(NTAPI *W_MINIPORT_SHUTDOWN_HANDLER)(
  _In_ PVOID ShutdownContext);

#ifdef __cplusplus

#define NDIS51_MINIPORT_CHARACTERISTICS_S \
  NDIS50_MINIPORT_CHARACTERISTICS  Ndis50Chars; \
  W_CANCEL_SEND_PACKETS_HANDLER  CancelSendPacketsHandler; \
  W_PNP_EVENT_NOTIFY_HANDLER  PnPEventNotifyHandler; \
  W_MINIPORT_SHUTDOWN_HANDLER  AdapterShutdownHandler; \
  PVOID Reserved1; \
  PVOID Reserved2; \
  PVOID Reserved3; \
  PVOID Reserved4;

#else

#define NDIS51_MINIPORT_CHARACTERISTICS_S \
  NDIS50_MINIPORT_CHARACTERISTICS_S \
  W_CANCEL_SEND_PACKETS_HANDLER  CancelSendPacketsHandler; \
  W_PNP_EVENT_NOTIFY_HANDLER  PnPEventNotifyHandler; \
  W_MINIPORT_SHUTDOWN_HANDLER  AdapterShutdownHandler; \
  PVOID Reserved1; \
  PVOID Reserved2; \
  PVOID Reserved3; \
  PVOID Reserved4;

#endif

typedef struct _NDIS51_MINIPORT_CHARACTERISTICS {
  NDIS51_MINIPORT_CHARACTERISTICS_S
} NDIS51_MINIPORT_CHARACTERISTICS, *PSNDIS51_MINIPORT_CHARACTERISTICS;

#if defined(NDIS51_MINIPORT)
typedef struct _NDIS_MINIPORT_CHARACTERISTICS {
  NDIS51_MINIPORT_CHARACTERISTICS_S
} NDIS_MINIPORT_CHARACTERISTICS, *PNDIS_MINIPORT_CHARACTERISTICS;
#elif defined(NDIS50_MINIPORT)
typedef struct _NDIS_MINIPORT_CHARACTERISTICS {
  NDIS50_MINIPORT_CHARACTERISTICS_S
} NDIS_MINIPORT_CHARACTERISTICS, *PNDIS_MINIPORT_CHARACTERISTICS;
#elif defined(NDIS40_MINIPORT)
typedef struct _NDIS_MINIPORT_CHARACTERISTICS {
  NDIS40_MINIPORT_CHARACTERISTICS_S
} NDIS_MINIPORT_CHARACTERISTICS, *PNDIS_MINIPORT_CHARACTERISTICS;
#else /* NDIS30 */
typedef struct _NDIS_MINIPORT_CHARACTERISTICS {
  NDIS30_MINIPORT_CHARACTERISTICS_S
} NDIS_MINIPORT_CHARACTERISTICS, *PNDIS_MINIPORT_CHARACTERISTICS;
#endif

typedef struct _NDIS_MINIPORT_INTERRUPT {
  PKINTERRUPT InterruptObject;
  KSPIN_LOCK DpcCountLock;
  PVOID Reserved;
  W_ISR_HANDLER MiniportIsr;
  W_HANDLE_INTERRUPT_HANDLER MiniportDpc;
  KDPC InterruptDpc;
  PNDIS_MINIPORT_BLOCK Miniport;
  UCHAR DpcCount;
  BOOLEAN Filler1;
  KEVENT DpcsCompletedEvent;
  BOOLEAN SharedInterrupt;
  BOOLEAN IsrRequested;
} NDIS_MINIPORT_INTERRUPT, *PNDIS_MINIPORT_INTERRUPT;

/* Structures available only to full MAC drivers */

typedef BOOLEAN
(NTAPI *PNDIS_INTERRUPT_SERVICE)(
  IN PVOID  InterruptContext);

typedef VOID
(NTAPI *PNDIS_DEFERRED_PROCESSING)(
  IN PVOID  SystemSpecific1,
  IN PVOID  InterruptContext,
  IN PVOID  SystemSpecific2,
  IN PVOID  SystemSpecific3);

typedef struct _NDIS_WRAPPER_HANDLE NDIS_WRAPPER_HANDLE, *PNDIS_WRAPPER_HANDLE;
typedef struct _NDIS_PROTOCOL_BLOCK NDIS_PROTOCOL_BLOCK, *PNDIS_PROTOCOL_BLOCK;
typedef struct _NDIS_OPEN_BLOCK NDIS_OPEN_BLOCK, *PNDIS_OPEN_BLOCK;
typedef struct _NDIS_M_DRIVER_BLOCK NDIS_M_DRIVER_BLOCK, *PNDIS_M_DRIVER_BLOCK;
typedef struct _NDIS_OFFLOAD NDIS_OFFLOAD, *PNDIS_OFFLOAD;
typedef struct _NDIS_AF_LIST NDIS_AF_LIST, *PNDIS_AF_LIST;
typedef struct _X_FILTER ETH_FILTER, *PETH_FILTER;
#if NDIS_SUPPORT_NDIS6
typedef USHORT NET_FRAME_TYPE, *PNET_FRAME_TYPE;
#endif

typedef struct _NDIS_MINIPORT_TIMER {
  KTIMER  Timer;
  KDPC  Dpc;
  PNDIS_TIMER_FUNCTION  MiniportTimerFunction;
  PVOID  MiniportTimerContext;
  PNDIS_MINIPORT_BLOCK  Miniport;
  struct _NDIS_MINIPORT_TIMER  *NextDeferredTimer;
} NDIS_MINIPORT_TIMER, *PNDIS_MINIPORT_TIMER;

typedef struct _NDIS_INTERRUPT {
  PKINTERRUPT  InterruptObject;
  KSPIN_LOCK  DpcCountLock;
  PNDIS_INTERRUPT_SERVICE  MacIsr;
  PNDIS_DEFERRED_PROCESSING  MacDpc;
  KDPC  InterruptDpc;
  PVOID  InterruptContext;
  UCHAR  DpcCount;
  BOOLEAN	 Removing;
  KEVENT  DpcsCompletedEvent;
} NDIS_INTERRUPT, *PNDIS_INTERRUPT;


typedef enum _NDIS_WORK_ITEM_TYPE {
  NdisWorkItemRequest,
  NdisWorkItemSend,
  NdisWorkItemReturnPackets,
  NdisWorkItemResetRequested,
  NdisWorkItemResetInProgress,
  NdisWorkItemHalt,
  NdisWorkItemSendLoopback,
  NdisWorkItemMiniportCallback,
  NdisMaxWorkItems
} NDIS_WORK_ITEM_TYPE, *PNDIS_WORK_ITEM_TYPE;

#define	NUMBER_OF_WORK_ITEM_TYPES         NdisMaxWorkItems
#define	NUMBER_OF_SINGLE_WORK_ITEMS       6

typedef struct _NDIS_MINIPORT_WORK_ITEM {
	SINGLE_LIST_ENTRY  Link;
	NDIS_WORK_ITEM_TYPE  WorkItemType;
	PVOID  WorkItemContext;
} NDIS_MINIPORT_WORK_ITEM, *PNDIS_MINIPORT_WORK_ITEM;

struct _NDIS_WORK_ITEM;
typedef VOID (*NDIS_PROC)(struct _NDIS_WORK_ITEM *, PVOID);

typedef struct _NDIS_WORK_ITEM {
  PVOID Context;
  NDIS_PROC Routine;
  UCHAR WrapperReserved[8*sizeof(PVOID)];
} NDIS_WORK_ITEM, *PNDIS_WORK_ITEM;

typedef struct _NDIS_BIND_PATHS {
	UINT  Number;
	NDIS_STRING  Paths[1];
} NDIS_BIND_PATHS, *PNDIS_BIND_PATHS;


typedef VOID
(NTAPI *ETH_RCV_COMPLETE_HANDLER)(
  _In_ PETH_FILTER Filter);

typedef VOID
(NTAPI *ETH_RCV_INDICATE_HANDLER)(
  _In_ PETH_FILTER Filter,
  _In_ NDIS_HANDLE MacReceiveContext,
  _In_ PCHAR Address,
  _In_ PVOID HeaderBuffer,
  _In_ UINT HeaderBufferSize,
  _In_ PVOID LookaheadBuffer,
  _In_ UINT LookaheadBufferSize,
  _In_ UINT PacketSize);

typedef VOID
(NTAPI *FDDI_RCV_COMPLETE_HANDLER)(
  IN PFDDI_FILTER  Filter);

typedef VOID
(NTAPI *FDDI_RCV_INDICATE_HANDLER)(
  IN PFDDI_FILTER  Filter,
  IN NDIS_HANDLE  MacReceiveContext,
  IN PCHAR  Address,
  IN UINT  AddressLength,
  IN PVOID  HeaderBuffer,
  IN UINT  HeaderBufferSize,
  IN PVOID  LookaheadBuffer,
  IN UINT  LookaheadBufferSize,
  IN UINT  PacketSize);

typedef VOID
(NTAPI *FILTER_PACKET_INDICATION_HANDLER)(
  _In_ NDIS_HANDLE Miniport,
  _In_ PPNDIS_PACKET PacketArray,
  _In_ UINT NumberOfPackets);

typedef VOID
(NTAPI *TR_RCV_COMPLETE_HANDLER)(
  _In_ PTR_FILTER Filter);

typedef VOID
(NTAPI *TR_RCV_INDICATE_HANDLER)(
  _In_ PTR_FILTER Filter,
  _In_ NDIS_HANDLE MacReceiveContext,
  _In_ PVOID HeaderBuffer,
  _In_ UINT HeaderBufferSize,
  _In_ PVOID LookaheadBuffer,
  _In_ UINT LookaheadBufferSize,
  _In_ UINT PacketSize);

typedef VOID
(NTAPI *WAN_RCV_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_HANDLE NdisLinkContext);

typedef VOID
(NTAPI *WAN_RCV_HANDLER)(
  _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_HANDLE NdisLinkContext,
  _In_ PUCHAR Packet,
  _In_ ULONG PacketSize);

typedef VOID
(FASTCALL *NDIS_M_DEQUEUE_WORK_ITEM)(
  IN PNDIS_MINIPORT_BLOCK  Miniport,
  IN NDIS_WORK_ITEM_TYPE  WorkItemType,
  OUT PVOID  *WorkItemContext);

typedef NDIS_STATUS
(FASTCALL *NDIS_M_QUEUE_NEW_WORK_ITEM)(
  IN PNDIS_MINIPORT_BLOCK  Miniport,
  IN NDIS_WORK_ITEM_TYPE  WorkItemType,
  IN PVOID  WorkItemContext);

typedef NDIS_STATUS
(FASTCALL *NDIS_M_QUEUE_WORK_ITEM)(
  IN PNDIS_MINIPORT_BLOCK  Miniport,
  IN NDIS_WORK_ITEM_TYPE  WorkItemType,
  IN PVOID  WorkItemContext);

typedef VOID
(NTAPI *NDIS_M_REQ_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *NDIS_M_RESET_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_STATUS Status,
  _In_ BOOLEAN AddressingReset);

typedef VOID
(NTAPI *NDIS_M_SEND_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ PNDIS_PACKET Packet,
  _In_ NDIS_STATUS Status);

typedef VOID
(NTAPI *NDIS_M_SEND_RESOURCES_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

typedef BOOLEAN
(FASTCALL *NDIS_M_START_SENDS)(
  _In_ PNDIS_MINIPORT_BLOCK Miniport);

typedef VOID
(NTAPI *NDIS_M_STATUS_HANDLER)(
  _In_ NDIS_HANDLE MiniportHandle,
  _In_ NDIS_STATUS GeneralStatus,
  _In_ PVOID StatusBuffer,
  _In_ UINT StatusBufferSize);

typedef VOID
(NTAPI *NDIS_M_STS_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

typedef VOID
(NTAPI *NDIS_M_TD_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ PNDIS_PACKET Packet,
  _In_ NDIS_STATUS Status,
  _In_ UINT BytesTransferred);

typedef VOID (NTAPI *NDIS_WM_SEND_COMPLETE_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ PVOID Packet,
  _In_ NDIS_STATUS Status);


#if ARCNET

#define ARC_SEND_BUFFERS                  8
#define ARC_HEADER_SIZE                   4

typedef struct _NDIS_ARC_BUF {
  NDIS_HANDLE  ArcnetBufferPool;
  PUCHAR  ArcnetLookaheadBuffer;
  UINT  NumFree;
  ARC_BUFFER_LIST ArcnetBuffers[ARC_SEND_BUFFERS];
} NDIS_ARC_BUF, *PNDIS_ARC_BUF;

#endif /* ARCNET */

typedef struct _NDIS_LOG {
  PNDIS_MINIPORT_BLOCK  Miniport;
  KSPIN_LOCK  LogLock;
  PIRP  Irp;
  UINT  TotalSize;
  UINT  CurrentSize;
  UINT  InPtr;
  UINT  OutPtr;
  UCHAR  LogBuf[1];
} NDIS_LOG, *PNDIS_LOG;

#if ARCNET
#define FILTERDBS_ARCNET_S \
  PARC_FILTER  ArcDB;
#else /* !ARCNET */
#define FILTERDBS_ARCNET_S \
  PVOID  XXXDB;
#endif /* !ARCNET */

#define FILTERDBS_S \
  _ANONYMOUS_UNION union { \
    PETH_FILTER  EthDB; \
    PNULL_FILTER  NullDB; \
  } DUMMYUNIONNAME; \
  PTR_FILTER  TrDB; \
  PFDDI_FILTER  FddiDB; \
  FILTERDBS_ARCNET_S

typedef struct _FILTERDBS {
  FILTERDBS_S
} FILTERDBS, *PFILTERDBS;

struct _NDIS_MINIPORT_BLOCK {
  NDIS_OBJECT_HEADER Header;
  PNDIS_MINIPORT_BLOCK  NextMiniport;
  PNDIS_M_DRIVER_BLOCK  DriverHandle;
  NDIS_HANDLE  MiniportAdapterContext;
  UNICODE_STRING  MiniportName;
  PNDIS_BIND_PATHS  BindPaths;
  NDIS_HANDLE  OpenQueue;
  REFERENCE  ShortRef;
  NDIS_HANDLE  DeviceContext;
  UCHAR  Padding1;
  UCHAR  LockAcquired;
  UCHAR  PmodeOpens;
  UCHAR  AssignedProcessor;
  KSPIN_LOCK  Lock;
  PNDIS_REQUEST  MediaRequest;
  PNDIS_MINIPORT_INTERRUPT  Interrupt;
  ULONG  Flags;
  ULONG  PnPFlags;
  LIST_ENTRY  PacketList;
  PNDIS_PACKET  FirstPendingPacket;
  PNDIS_PACKET  ReturnPacketsQueue;
  ULONG  RequestBuffer;
  PVOID  SetMCastBuffer;
  PNDIS_MINIPORT_BLOCK  PrimaryMiniport;
  PVOID  WrapperContext;
  PVOID  BusDataContext;
  ULONG  PnPCapabilities;
  PCM_RESOURCE_LIST  Resources;
  NDIS_TIMER  WakeUpDpcTimer;
  UNICODE_STRING  BaseName;
  UNICODE_STRING  SymbolicLinkName;
  ULONG  CheckForHangSeconds;
  USHORT  CFHangTicks;
  USHORT  CFHangCurrentTick;
  NDIS_STATUS  ResetStatus;
  NDIS_HANDLE  ResetOpen;
  FILTERDBS_S
  FILTER_PACKET_INDICATION_HANDLER  PacketIndicateHandler;
  NDIS_M_SEND_COMPLETE_HANDLER  SendCompleteHandler;
  NDIS_M_SEND_RESOURCES_HANDLER  SendResourcesHandler;
  NDIS_M_RESET_COMPLETE_HANDLER  ResetCompleteHandler;
  NDIS_MEDIUM  MediaType;
  ULONG  BusNumber;
  NDIS_INTERFACE_TYPE  BusType;
  NDIS_INTERFACE_TYPE  AdapterType;
  PDEVICE_OBJECT  DeviceObject;
  PDEVICE_OBJECT  PhysicalDeviceObject;
  PDEVICE_OBJECT  NextDeviceObject;
  PMAP_REGISTER_ENTRY  MapRegisters;
  PNDIS_AF_LIST  CallMgrAfList;
  PVOID  MiniportThread;
  PVOID  SetInfoBuf;
  USHORT  SetInfoBufLen;
  USHORT  MaxSendPackets;
  NDIS_STATUS  FakeStatus;
  PVOID  LockHandler;
  PUNICODE_STRING  pAdapterInstanceName;
  PNDIS_MINIPORT_TIMER  TimerQueue;
  UINT  MacOptions;
  PNDIS_REQUEST  PendingRequest;
  UINT  MaximumLongAddresses;
  UINT  MaximumShortAddresses;
  UINT  CurrentLookahead;
  UINT  MaximumLookahead;
  W_HANDLE_INTERRUPT_HANDLER  HandleInterruptHandler;
  W_DISABLE_INTERRUPT_HANDLER  DisableInterruptHandler;
  W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
  W_SEND_PACKETS_HANDLER  SendPacketsHandler;
  NDIS_M_START_SENDS  DeferredSendHandler;
  ETH_RCV_INDICATE_HANDLER  EthRxIndicateHandler;
  TR_RCV_INDICATE_HANDLER  TrRxIndicateHandler;
  FDDI_RCV_INDICATE_HANDLER  FddiRxIndicateHandler;
  ETH_RCV_COMPLETE_HANDLER  EthRxCompleteHandler;
  TR_RCV_COMPLETE_HANDLER  TrRxCompleteHandler;
  FDDI_RCV_COMPLETE_HANDLER  FddiRxCompleteHandler;
  NDIS_M_STATUS_HANDLER  StatusHandler;
  NDIS_M_STS_COMPLETE_HANDLER  StatusCompleteHandler;
  NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;
  NDIS_M_REQ_COMPLETE_HANDLER  QueryCompleteHandler;
  NDIS_M_REQ_COMPLETE_HANDLER  SetCompleteHandler;
  NDIS_WM_SEND_COMPLETE_HANDLER  WanSendCompleteHandler;
  WAN_RCV_HANDLER  WanRcvHandler;
  WAN_RCV_COMPLETE_HANDLER  WanRcvCompleteHandler;
#if defined(NDIS_WRAPPER)
  PNDIS_MINIPORT_BLOCK  NextGlobalMiniport;
  SINGLE_LIST_ENTRY  WorkQueue[NUMBER_OF_WORK_ITEM_TYPES];
  SINGLE_LIST_ENTRY  SingleWorkItems[NUMBER_OF_SINGLE_WORK_ITEMS];
  UCHAR  SendFlags;
  UCHAR  TrResetRing;
  UCHAR  ArcnetAddress;
  UCHAR  XState;
  _ANONYMOUS_UNION union {
#if ARCNET
    PNDIS_ARC_BUF  ArcBuf;
#endif
    PVOID  BusInterface;
  } DUMMYUNIONNAME;
  PNDIS_LOG  Log;
  ULONG  SlotNumber;
  PCM_RESOURCE_LIST  AllocatedResources;
  PCM_RESOURCE_LIST  AllocatedResourcesTranslated;
  SINGLE_LIST_ENTRY  PatternList;
  NDIS_PNP_CAPABILITIES  PMCapabilities;
  DEVICE_CAPABILITIES  DeviceCaps;
  ULONG  WakeUpEnable;
  DEVICE_POWER_STATE  CurrentDevicePowerState;
  PIRP  pIrpWaitWake;
  SYSTEM_POWER_STATE  WaitWakeSystemState;
  LARGE_INTEGER  VcIndex;
  KSPIN_LOCK  VcCountLock;
  LIST_ENTRY  WmiEnabledVcs;
  PNDIS_GUID  pNdisGuidMap;
  PNDIS_GUID  pCustomGuidMap;
  USHORT  VcCount;
  USHORT  cNdisGuidMap;
  USHORT  cCustomGuidMap;
  USHORT  CurrentMapRegister;
  PKEVENT  AllocationEvent;
  USHORT  BaseMapRegistersNeeded;
  USHORT  SGMapRegistersNeeded;
  ULONG  MaximumPhysicalMapping;
  NDIS_TIMER  MediaDisconnectTimer;
  USHORT  MediaDisconnectTimeOut;
  USHORT  InstanceNumber;
  NDIS_EVENT  OpenReadyEvent;
  NDIS_PNP_DEVICE_STATE  PnPDeviceState;
  NDIS_PNP_DEVICE_STATE  OldPnPDeviceState;
  PGET_SET_DEVICE_DATA  SetBusData;
  PGET_SET_DEVICE_DATA  GetBusData;
  KDPC  DeferredDpc;
#if 0
  /* FIXME: */
  NDIS_STATS  NdisStats;
#else
  ULONG  NdisStats;
#endif
  PNDIS_PACKET  IndicatedPacket[MAXIMUM_PROCESSORS];
  PKEVENT  RemoveReadyEvent;
  PKEVENT  AllOpensClosedEvent;
  PKEVENT  AllRequestsCompletedEvent;
  ULONG  InitTimeMs;
  NDIS_MINIPORT_WORK_ITEM  WorkItemBuffer[NUMBER_OF_SINGLE_WORK_ITEMS];
  PDMA_ADAPTER  SystemAdapterObject;
  ULONG  DriverVerifyFlags;
  POID_LIST  OidList;
  USHORT  InternalResetCount;
  USHORT  MiniportResetCount;
  USHORT  MediaSenseConnectCount;
  USHORT  MediaSenseDisconnectCount;
  PNDIS_PACKET  *xPackets;
  ULONG  UserModeOpenReferences;
  _ANONYMOUS_UNION union {
    PVOID  SavedSendHandler;
    PVOID  SavedWanSendHandler;
  } DUMMYUNIONNAME2;
  PVOID  SavedSendPacketsHandler;
  PVOID  SavedCancelSendPacketsHandler;
  W_SEND_PACKETS_HANDLER  WSendPacketsHandler;
  ULONG  MiniportAttributes;
  PDMA_ADAPTER  SavedSystemAdapterObject;
  USHORT  NumOpens;
  USHORT  CFHangXTicks;
  ULONG  RequestCount;
  ULONG  IndicatedPacketsCount;
  ULONG  PhysicalMediumType;
  PNDIS_REQUEST  LastRequest;
  LONG  DmaAdapterRefCount;
  PVOID  FakeMac;
  ULONG  LockDbg;
  ULONG  LockDbgX;
  PVOID  LockThread;
  ULONG  InfoFlags;
  KSPIN_LOCK  TimerQueueLock;
  PKEVENT  ResetCompletedEvent;
  PKEVENT  QueuedBindingCompletedEvent;
  PKEVENT  DmaResourcesReleasedEvent;
  FILTER_PACKET_INDICATION_HANDLER  SavedPacketIndicateHandler;
  ULONG  RegisteredInterrupts;
  PNPAGED_LOOKASIDE_LIST  SGListLookasideList;
  ULONG  ScatterGatherListSize;
#endif /* _NDIS_ */
};

#if NDIS_LEGACY_DRIVER

typedef NDIS_STATUS
(NTAPI *WAN_SEND_HANDLER)(
  _In_ NDIS_HANDLE MacBindingHandle,
  _In_ NDIS_HANDLE LinkHandle,
  _In_ PVOID Packet);

typedef VOID
(NTAPI *SEND_PACKETS_HANDLER)(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ PPNDIS_PACKET PacketArray,
  _In_ UINT NumberOfPackets);

typedef NDIS_STATUS
(NTAPI *SEND_HANDLER)(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ PNDIS_PACKET Packet);

typedef NDIS_STATUS
(NTAPI *TRANSFER_DATA_HANDLER)(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ NDIS_HANDLE MacReceiveContext,
  _In_ UINT ByteOffset,
  _In_ UINT BytesToTransfer,
  _Out_ PNDIS_PACKET Packet,
  _Out_ PUINT BytesTransferred);

typedef NDIS_STATUS
(NTAPI *RESET_HANDLER)(
  _In_ NDIS_HANDLE NdisBindingHandle);

typedef NDIS_STATUS
(NTAPI *REQUEST_HANDLER)(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ PNDIS_REQUEST NdisRequest);

#endif /* NDIS_LEGACY_DRIVER */

#if defined(NDIS_WRAPPER)
#define NDIS_COMMON_OPEN_BLOCK_WRAPPER_S \
  ULONG Flags; \
  ULONG References; \
  KSPIN_LOCK SpinLock; \
  NDIS_HANDLE  FilterHandle; \
  ULONG  ProtocolOptions; \
  USHORT  CurrentLookahead; \
  USHORT  ConnectDampTicks; \
  USHORT  DisconnectDampTicks; \
  W_SEND_HANDLER  WSendHandler; \
  W_TRANSFER_DATA_HANDLER  WTransferDataHandler; \
  W_SEND_PACKETS_HANDLER  WSendPacketsHandler; \
  W_CANCEL_SEND_PACKETS_HANDLER  CancelSendPacketsHandler; \
  ULONG  WakeUpEnable; \
  PKEVENT  CloseCompleteEvent; \
  QUEUED_CLOSE  QC; \
  ULONG  AfReferences; \
  PNDIS_OPEN_BLOCK  NextGlobalOpen;
#else
#define NDIS_COMMON_OPEN_BLOCK_WRAPPER_S
#endif

#define NDIS_COMMON_OPEN_BLOCK_S \
  PVOID  MacHandle; \
  NDIS_HANDLE  BindingHandle; \
  PNDIS_MINIPORT_BLOCK  MiniportHandle; \
  PNDIS_PROTOCOL_BLOCK  ProtocolHandle; \
  NDIS_HANDLE  ProtocolBindingContext; \
  PNDIS_OPEN_BLOCK  MiniportNextOpen; \
  PNDIS_OPEN_BLOCK  ProtocolNextOpen; \
  NDIS_HANDLE  MiniportAdapterContext; \
  BOOLEAN  Reserved1; \
  BOOLEAN  Reserved2; \
  BOOLEAN  Reserved3; \
  BOOLEAN  Reserved4; \
  PNDIS_STRING  BindDeviceName; \
  KSPIN_LOCK  Reserved5; \
  PNDIS_STRING  RootDeviceName; \
  _ANONYMOUS_UNION union { \
    SEND_HANDLER  SendHandler; \
    WAN_SEND_HANDLER  WanSendHandler; \
  } DUMMYUNIONNAME; \
  TRANSFER_DATA_HANDLER  TransferDataHandler; \
  SEND_COMPLETE_HANDLER  SendCompleteHandler; \
  TRANSFER_DATA_COMPLETE_HANDLER  TransferDataCompleteHandler; \
  RECEIVE_HANDLER  ReceiveHandler; \
  RECEIVE_COMPLETE_HANDLER  ReceiveCompleteHandler; \
  WAN_RECEIVE_HANDLER  WanReceiveHandler; \
  REQUEST_COMPLETE_HANDLER  RequestCompleteHandler; \
  RECEIVE_PACKET_HANDLER  ReceivePacketHandler; \
  SEND_PACKETS_HANDLER  SendPacketsHandler; \
  RESET_HANDLER  ResetHandler; \
  REQUEST_HANDLER  RequestHandler; \
  RESET_COMPLETE_HANDLER  ResetCompleteHandler; \
  STATUS_HANDLER  StatusHandler; \
  STATUS_COMPLETE_HANDLER  StatusCompleteHandler; \
  NDIS_COMMON_OPEN_BLOCK_WRAPPER_S

typedef struct _NDIS_COMMON_OPEN_BLOCK {
  NDIS_COMMON_OPEN_BLOCK_S
} NDIS_COMMON_OPEN_BLOCK;

struct _NDIS_OPEN_BLOCK
{
#ifdef __cplusplus
  NDIS_COMMON_OPEN_BLOCK NdisCommonOpenBlock;
#else
  NDIS_COMMON_OPEN_BLOCK_S
#endif
};

#include <xfilter.h>

#define NDIS_M_MAX_LOOKAHEAD           526

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisInitializeTimer(
  _Inout_ PNDIS_TIMER Timer,
  _In_ PNDIS_TIMER_FUNCTION TimerFunction,
  _In_opt_ _Points_to_data_ PVOID FunctionContext);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCancelTimer(
  _In_ PNDIS_TIMER Timer,
  _Out_ _At_(*TimerCancelled, _Must_inspect_result_) PBOOLEAN TimerCancelled);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisSetTimer(
  _In_ PNDIS_TIMER Timer,
  _In_ UINT MillisecondsToDelay);

NDISAPI
VOID
NTAPI
NdisSetPeriodicTimer(
  _In_ PNDIS_TIMER NdisTimer,
  _In_ UINT MillisecondsPeriod);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisSetTimerEx(
  _In_ PNDIS_TIMER NdisTimer,
  _In_ UINT MillisecondsToDelay,
  _In_ PVOID FunctionContext);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
PVOID
NTAPI
NdisGetRoutineAddress(
  _In_ PNDIS_STRING NdisRoutineName);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
UINT
NTAPI
NdisGetVersion(VOID);

#if NDIS_LEGACY_DRIVER

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisAllocateBuffer(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_BUFFER *Buffer,
  _In_opt_ NDIS_HANDLE PoolHandle,
  _In_reads_bytes_(Length) PVOID VirtualAddress,
  _In_ UINT Length);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisAllocateBufferPool(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE PoolHandle,
  _In_ UINT NumberOfDescriptors);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisFreeBufferPool(
  _In_ NDIS_HANDLE PoolHandle);

/*
NDISAPI
VOID
NTAPI
NdisFreeBuffer(
  IN PNDIS_BUFFER Buffer);
*/
#define NdisFreeBuffer IoFreeMdl

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisAllocatePacketPool(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE PoolHandle,
  _In_ UINT NumberOfDescriptors,
  _In_ UINT ProtocolReservedLength);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisAllocatePacketPoolEx(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE PoolHandle,
  _In_ UINT NumberOfDescriptors,
  _In_ UINT NumberOfOverflowDescriptors,
  _In_ UINT ProtocolReservedLength);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisSetPacketPoolProtocolId(
  _In_ NDIS_HANDLE PacketPoolHandle,
  _In_ UINT ProtocolId);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
UINT
NTAPI
NdisPacketPoolUsage(
  _In_ NDIS_HANDLE PoolHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
UINT
NTAPI
NdisPacketSize(
  _In_ UINT ProtocolReservedSize);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_HANDLE
NTAPI
NdisGetPoolFromPacket(
  _In_ PNDIS_PACKET Packet);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
PNDIS_PACKET_STACK
NTAPI
NdisIMGetCurrentPacketStack(
  _In_ PNDIS_PACKET Packet,
  _Out_ BOOLEAN *StacksRemaining);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisFreePacketPool(
  _In_ NDIS_HANDLE PoolHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisFreePacket(
  _In_ PNDIS_PACKET Packet);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisDprFreePacket(
  _In_ PNDIS_PACKET Packet);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisDprFreePacketNonInterlocked(
  _In_ PNDIS_PACKET Packet);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisAllocatePacket(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_PACKET *Packet,
  _In_ NDIS_HANDLE PoolHandle);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisDprAllocatePacket(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_PACKET *Packet,
  _In_ NDIS_HANDLE PoolHandle);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisDprAllocatePacketNonInterlocked(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_PACKET *Packet,
  _In_ NDIS_HANDLE PoolHandle);

/*
 * VOID
 * NdisReinitializePacket(
 *   IN OUT  PNDIS_PACKET  Packet);
 */
#define NdisReinitializePacket(Packet) {        \
  (Packet)->Private.Head = (PNDIS_BUFFER)NULL;  \
  (Packet)->Private.ValidCounts = FALSE;        \
}

/*
NDISAPI
VOID
NTAPI
NdisQueryBuffer(
  IN PNDIS_BUFFER Buffer,
  OUT PVOID *VirtualAddress OPTIONAL,
  OUT PUINT Length);
*/
#define NdisQueryBuffer(_Buffer, _VirtualAddress, _Length) {         \
  if (ARGUMENT_PRESENT(_VirtualAddress)) {                           \
    *(PVOID *)(_VirtualAddress) = MmGetSystemAddressForMdl(_Buffer); \
  }                                                                  \
  *(_Length) = MmGetMdlByteCount(_Buffer);                           \
}

NDISAPI
VOID
NTAPI
NdisGetFirstBufferFromPacket(
  IN PNDIS_PACKET _Packet,
  OUT PNDIS_BUFFER *_FirstBuffer,
  OUT PVOID *_FirstBufferVA,
  OUT PUINT _FirstBufferLength,
  OUT PUINT _TotalBufferLength);

/*
 * VOID
 * NdisGetFirstBufferFromPacketSafe(
 * IN PNDIS_PACKET _Packet,
 * OUT PNDIS_BUFFER * _FirstBuffer,
 * OUT PVOID * _FirstBufferVA,
 * OUT PUINT _FirstBufferLength,
 * OUT PUINT _TotalBufferLength),
 * IN MM_PAGE_PRIORITY _Priority)
 */
#define NdisGetFirstBufferFromPacketSafe(_Packet,                             \
                                     _FirstBuffer,                            \
                                     _FirstBufferVA,                          \
                                     _FirstBufferLength,                      \
                                     _TotalBufferLength,                      \
                                     _Priority)                               \
{                                                                             \
  PNDIS_BUFFER _Buffer;                                                       \
                                                                              \
  _Buffer         = (_Packet)->Private.Head;                                  \
  *(_FirstBuffer) = _Buffer;                                                  \
  if (_Buffer != NULL) {                                                      \
    *(_FirstBufferVA)     = MmGetSystemAddressForMdlSafe(_Buffer, _Priority); \
    *(_FirstBufferLength) = MmGetMdlByteCount(_Buffer);                       \
    _Buffer = _Buffer->Next;                                                  \
    *(_TotalBufferLength) = *(_FirstBufferLength);                            \
    while (_Buffer != NULL) {                                                 \
      *(_TotalBufferLength) += MmGetMdlByteCount(_Buffer);                    \
      _Buffer = _Buffer->Next;                                                \
    }                                                                         \
  }                                                                           \
  else {                                                                      \
    *(_FirstBufferVA) = 0;                                                    \
    *(_FirstBufferLength) = 0;                                                \
    *(_TotalBufferLength) = 0;                                                \
  }                                                                           \
}

/*
 * VOID
 * NdisRecalculatePacketCounts(
 *   IN OUT PNDIS_PACKET Packet);
 */
#define NdisRecalculatePacketCounts(Packet) {     \
  PNDIS_BUFFER _Buffer = (Packet)->Private.Head;  \
  if (_Buffer != NULL) {                          \
    while (_Buffer->Next != NULL) {               \
      _Buffer = _Buffer->Next;                    \
    }                                             \
    (Packet)->Private.Tail = _Buffer;             \
  }                                               \
  (Packet)->Private.ValidCounts = FALSE;          \
}

/*
 * VOID
 * NdisChainBufferAtFront(
 *   IN OUT PNDIS_PACKET Packet,
 *   IN OUT PNDIS_BUFFER Buffer)
 */
#define NdisChainBufferAtFront(Packet,        \
                               Buffer)        \
{                                             \
  PNDIS_BUFFER _NdisBuffer = (Buffer);        \
                                              \
  while (_NdisBuffer->Next != NULL)           \
    _NdisBuffer = _NdisBuffer->Next;          \
                                              \
  if ((Packet)->Private.Head == NULL)         \
    (Packet)->Private.Tail = _NdisBuffer;     \
                                              \
  _NdisBuffer->Next = (Packet)->Private.Head; \
  (Packet)->Private.Head = (Buffer);          \
  (Packet)->Private.ValidCounts = FALSE;      \
}

/*
 * VOID
 * NdisChainBufferAtBack(
 *   IN OUT PNDIS_PACKET Packet,
 *   IN OUT PNDIS_BUFFER Buffer)
 */
#define NdisChainBufferAtBack(Packet,           \
                              Buffer)           \
{                                               \
  PNDIS_BUFFER _NdisBuffer = (Buffer);           \
                                                \
  while (_NdisBuffer->Next != NULL)              \
    _NdisBuffer = _NdisBuffer->Next;              \
                                                \
  _NdisBuffer->Next = NULL;                      \
                                                \
  if ((Packet)->Private.Head != NULL)           \
    (Packet)->Private.Tail->Next = (Buffer);    \
  else                                          \
    (Packet)->Private.Head = (Buffer);          \
                                                \
  (Packet)->Private.Tail = _NdisBuffer;          \
  (Packet)->Private.ValidCounts = FALSE;        \
}

NDISAPI
VOID
NTAPI
NdisUnchainBufferAtFront(
  IN OUT PNDIS_PACKET Packet,
  OUT PNDIS_BUFFER *Buffer);

NDISAPI
VOID
NTAPI
NdisUnchainBufferAtBack(
  IN OUT PNDIS_PACKET Packet,
  OUT PNDIS_BUFFER *Buffer);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCopyFromPacketToPacket(
  _In_ PNDIS_PACKET Destination,
  _In_ UINT DestinationOffset,
  _In_ UINT BytesToCopy,
  _In_ PNDIS_PACKET Source,
  _In_ UINT SourceOffset,
  _Out_ PUINT BytesCopied);

NDISAPI
VOID
NTAPI
NdisCopyFromPacketToPacketSafe(
  IN PNDIS_PACKET Destination,
  IN UINT DestinationOffset,
  IN UINT BytesToCopy,
  IN PNDIS_PACKET Source,
  IN UINT SourceOffset,
  OUT PUINT BytesCopied,
  IN MM_PAGE_PRIORITY Priority);

_IRQL_requires_max_(DISPATCH_LEVEL)
__drv_preferredFunction("NdisAllocateMemoryWithTag", "Obsolete")
NDISAPI
NDIS_STATUS
NTAPI
NdisAllocateMemory(
  _At_(*VirtualAddress, __drv_allocatesMem(Mem)) _Outptr_result_bytebuffer_(Length)
    PVOID *VirtualAddress,
  _In_ UINT Length,
  _In_ UINT MemoryFlags,
  _In_ NDIS_PHYSICAL_ADDRESS HighestAcceptableAddress);

#define NdisInitializeWorkItem(_WI_, _R_, _C_) { \
  (_WI_)->Context = _C_;                         \
  (_WI_)->Routine = _R_;                         \
}

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisScheduleWorkItem(
  _In_ __drv_aliasesMem PNDIS_WORK_ITEM WorkItem);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisSetPacketStatus(
  _In_ PNDIS_PACKET Packet,
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE Handle,
  _In_ ULONG Code);

#endif /* NDIS_LEGACY_DRIVER */

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisOpenFile(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE FileHandle,
  _Out_ PUINT FileLength,
  _In_ PNDIS_STRING FileName,
  _In_ NDIS_PHYSICAL_ADDRESS HighestAcceptableAddress);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisCloseFile(
  _In_ NDIS_HANDLE FileHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMapFile(
  _Out_ PNDIS_STATUS Status,
  _Out_ PVOID *MappedBuffer,
  _In_ NDIS_HANDLE FileHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisUnmapFile(
  _In_ NDIS_HANDLE FileHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
ULONG
NTAPI
NdisGetSharedDataAlignment(VOID);

#define NdisFlushBuffer(Buffer,WriteToDevice) \
  KeFlushIoBuffers((Buffer),!(WriteToDevice), TRUE)

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCopyBuffer(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_BUFFER *Buffer,
  _In_ NDIS_HANDLE PoolHandle,
  _In_ PVOID MemoryDescriptor,
  _In_ UINT Offset,
  _In_ UINT Length);

/*
 * VOID
 * NdisCopyLookaheadData(
 *   IN PVOID Destination,
 *   IN PVOID Source,
 *   IN ULONG Length,
 *   IN ULONG ReceiveFlags);
 */

#if defined(_M_IX86) || defined(_M_AMD64)
#define NdisCopyLookaheadData(Destination, Source, Length, MacOptions) \
  RtlCopyMemory(Destination, Source, Length)
#else
#define NdisCopyLookaheadData(Destination, Source, Length, MacOptions) \
  { \
    if ((MacOptions) & NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA) \
    { \
      RtlCopyMemory(_Destination, _Source, _Length); \
    } \
    else \
    { \
      PUCHAR _Src = (PUCHAR)(Source); \
      PUCHAR _Dest = (PUCHAR)(Destination); \
      PUCHAR _End = _Dest + (Length); \
      while (_Dest < _End) \
        *_Dest++ = *_Src++; \
    } \
  }
#endif

/*
NDISAPI
VOID
NTAPI
NdisAdjustBufferLength(
  IN PNDIS_BUFFER Buffer,
  IN UINT Length);
*/
#define NdisAdjustBufferLength(Buffer, Length) \
  (((Buffer)->ByteCount) = (Length))

#if NDIS_SUPPORT_NDIS6
#define NdisAdjustMdlLength(_Mdl, _Length) \
  (((_Mdl)->ByteCount) = (_Length))
#endif

/*
NDISAPI
ULONG
NTAPI
NdisBufferLength(
  IN PNDIS_BUFFER Buffer);
*/
#define NdisBufferLength MmGetMdlByteCount

/*
NDISAPI
PVOID
NTAPI
NdisBufferVirtualAddress(
  IN PNDIS_BUFFER Buffer);
*/
#define NdisBufferVirtualAddress MmGetSystemAddressForMdl

#define NdisBufferVirtualAddressSafe MmGetSystemAddressForMdlSafe

NDISAPI
ULONG
NTAPI
NDIS_BUFFER_TO_SPAN_PAGES(
  IN PNDIS_BUFFER Buffer);

/*
NDISAPI
VOID
NTAPI
NdisGetBufferPhysicalArraySize(
  IN PNDIS_BUFFER Buffer,
  OUT PUINT ArraySize);
*/
#define NdisGetBufferPhysicalArraySize(Buffer, ArraySize) \
  (*(ArraySize) = NDIS_BUFFER_TO_SPAN_PAGES(Buffer))

/*
NDISAPI
VOID
NTAPI
NdisQueryBufferOffset(
  IN PNDIS_BUFFER Buffer,
  OUT PUINT Offset,
  OUT PUINT Length);
*/
#define NdisQueryBufferOffset(_Buffer, _Offset, _Length) { \
  *(_Offset) = MmGetMdlByteOffset(_Buffer);                \
  *(_Length) = MmGetMdlByteCount(_Buffer);                 \
}

/*
 * PVOID
 * NDIS_BUFFER_LINKAGE(
 *   IN PNDIS_BUFFER Buffer);
 */
#define NDIS_BUFFER_LINKAGE(Buffer) (Buffer)->Next

/*
 * VOID
 * NdisGetNextBuffer(
 *   IN PNDIS_BUFFER CurrentBuffer,
 *   OUT PNDIS_BUFFER * NextBuffer)
 */
#define NdisGetNextBuffer(CurrentBuffer,  \
                          NextBuffer)     \
{                                         \
  *(NextBuffer) = (CurrentBuffer)->Next;  \
}

#if NDIS_LEGACY_DRIVER

#define NDIS_PACKET_FIRST_NDIS_BUFFER(_Packet) ((_Packet)->Private.Head)
#define NDIS_PACKET_LAST_NDIS_BUFFER(_Packet) ((_Packet)->Private.Tail)
#define NDIS_PACKET_VALID_COUNTS(_Packet) ((_Packet)->Private.ValidCounts)

/*
 * UINT
 * NdisGetPacketFlags(
 *   IN PNDIS_PACKET  Packet);
 */
#define NdisGetPacketFlags(Packet) (Packet)->Private.Flags

/*
 * ULONG
 * NDIS_GET_PACKET_PROTOCOL_TYPE(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_GET_PACKET_PROTOCOL_TYPE(_Packet) \
  ((_Packet)->Private.Flags & NDIS_PROTOCOL_ID_MASK)

/*
 * PNDIS_PACKET_OOB_DATA
 * NDIS_OOB_DATA_FROM_PACKET(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_OOB_DATA_FROM_PACKET(_Packet)    \
  (PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) + \
  (_Packet)->Private.NdisPacketOobOffset)

/*
 * ULONG
 * NDIS_GET_PACKET_HEADER_SIZE(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_GET_PACKET_HEADER_SIZE(_Packet)   \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) + \
  (_Packet)->Private.NdisPacketOobOffset))->HeaderSize

/*
 * NDIS_STATUS
 * NDIS_GET_PACKET_STATUS(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_GET_PACKET_STATUS(_Packet)        \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) + \
  (_Packet)->Private.NdisPacketOobOffset))->Status

/*
 * ULONGLONG
 * NDIS_GET_PACKET_TIME_TO_SEND(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_GET_PACKET_TIME_TO_SEND(_Packet)   \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +  \
  (_Packet)->Private.NdisPacketOobOffset))->TimeToSend

/*
 * ULONGLONG
 * NDIS_GET_PACKET_TIME_SENT(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_GET_PACKET_TIME_SENT(_Packet)      \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +  \
  (_Packet)->Private.NdisPacketOobOffset))->TimeSent

/*
 * ULONGLONG
 * NDIS_GET_PACKET_TIME_RECEIVED(
 *   IN PNDIS_PACKET Packet);
 */
#define NDIS_GET_PACKET_TIME_RECEIVED(_Packet)  \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +  \
  (_Packet)->Private.NdisPacketOobOffset))->TimeReceived

/*
 * VOID
 * NDIS_GET_PACKET_MEDIA_SPECIFIC_INFO(
 *   IN PNDIS_PACKET Packet,
 *   IN PPVOID pMediaSpecificInfo,
 *   IN PUINT pSizeMediaSpecificInfo);
 */
#define NDIS_GET_PACKET_MEDIA_SPECIFIC_INFO(_Packet,                                  \
                                            _pMediaSpecificInfo,                      \
                                            _pSizeMediaSpecificInfo)                  \
{                                                                                     \
  if (!((_Packet)->Private.NdisPacketFlags & fPACKET_ALLOCATED_BY_NDIS) ||            \
      !((_Packet)->Private.NdisPacketFlags & fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO))   \
    {                                                                                 \
      *(_pMediaSpecificInfo) = NULL;                                                  \
      *(_pSizeMediaSpecificInfo) = 0;                                                 \
    }                                                                                 \
  else                                                                                \
    {                                                                                 \
      *(_pMediaSpecificInfo) = ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +           \
        (_Packet)->Private.NdisPacketOobOffset))->MediaSpecificInformation;           \
      *(_pSizeMediaSpecificInfo) = ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +       \
        (_Packet)->Private.NdisPacketOobOffset))->SizeMediaSpecificInfo;              \
    }                                                                                 \
}

/*
 * VOID
 * NDIS_SET_PACKET_HEADER_SIZE(
 *   IN PNDIS_PACKET Packet,
 *   IN UINT HdrSize);
 */
#define NDIS_SET_PACKET_HEADER_SIZE(_Packet, _HdrSize)              \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +                      \
  (_Packet)->Private.NdisPacketOobOffset))->HeaderSize = (_HdrSize)

/*
 * VOID
 * NDIS_SET_PACKET_STATUS(
 *   IN PNDIS_PACKET Packet,
 *   IN NDIS_STATUS Status);
 */
#define NDIS_SET_PACKET_STATUS(_Packet, _Status)  \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
  (_Packet)->Private.NdisPacketOobOffset))->Status = (_Status)

/*
 * VOID
 * NDIS_SET_PACKET_TIME_TO_SEND(
 *   IN PNDIS_PACKET Packet,
 *   IN ULONGLONG TimeToSend);
 */
#define NDIS_SET_PACKET_TIME_TO_SEND(_Packet, _TimeToSend)  \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +              \
  (_Packet)->Private.NdisPacketOobOffset))->TimeToSend = (_TimeToSend)

/*
 * VOID
 * NDIS_SET_PACKET_TIME_SENT(
 *   IN PNDIS_PACKET Packet,
 *   IN ULONGLONG TimeSent);
 */
#define NDIS_SET_PACKET_TIME_SENT(_Packet, _TimeSent) \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +        \
  (_Packet)->Private.NdisPacketOobOffset))->TimeSent = (_TimeSent)

/*
 * VOID
 * NDIS_SET_PACKET_TIME_RECEIVED(
 *   IN PNDIS_PACKET Packet,
 *   IN ULONGLONG TimeReceived);
 */
#define NDIS_SET_PACKET_TIME_RECEIVED(_Packet, _TimeReceived) \
  ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +                \
  (_Packet)->Private.NdisPacketOobOffset))->TimeReceived = (_TimeReceived)

/*
 * VOID
 * NDIS_SET_PACKET_MEDIA_SPECIFIC_INFO(
 *   IN PNDIS_PACKET Packet,
 *   IN PVOID MediaSpecificInfo,
 *   IN UINT SizeMediaSpecificInfo);
 */
#define NDIS_SET_PACKET_MEDIA_SPECIFIC_INFO(_Packet,                      \
                                            _MediaSpecificInfo,           \
                                            _SizeMediaSpecificInfo)       \
{                                                                         \
  if ((_Packet)->Private.NdisPacketFlags & fPACKET_ALLOCATED_BY_NDIS)     \
    {                                                                     \
      (_Packet)->Private.NdisPacketFlags |= fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO; \
      ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +                        \
        (_Packet)->Private.NdisPacketOobOffset))->MediaSpecificInformation = \
          (_MediaSpecificInfo);                                           \
      ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +                        \
        (_Packet)->Private.NdisPacketOobOffset))->SizeMediaSpecificInfo = \
          (_SizeMediaSpecificInfo);                                       \
    }                                                                     \
}

/*
 * VOID
 * NdisSetPacketFlags(
 *   IN PNDIS_PACKET  Packet,
 *   IN UINT  Flags);
 */
#define NdisSetPacketFlags(Packet, Flags) (Packet)->Private.Flags |= (Flags)

/*
 * VOID
 * NdisClearPacketFlags(
 *   IN PNDIS_PACKET  Packet,
 *   IN UINT  Flags);
 */
#define NdisClearPacketFlags(Packet, Flags) (Packet)->Private.Flags &= ~(Flags)

/*
 * VOID
 * NdisQueryPacket(
 *   IN PNDIS_PACKET Packet,
 *   OUT PUINT PhysicalBufferCount OPTIONAL,
 *   OUT PUINT BufferCount OPTIONAL,
 *   OUT PNDIS_BUFFER *FirstBuffer OPTIONAL,
 *   OUT PUINT TotalPacketLength OPTIONAL);
 */
static __inline
VOID
NdisQueryPacket(
  IN PNDIS_PACKET Packet,
  OUT PUINT PhysicalBufferCount OPTIONAL,
  OUT PUINT BufferCount OPTIONAL,
  OUT PNDIS_BUFFER *FirstBuffer OPTIONAL,
  OUT PUINT TotalPacketLength OPTIONAL)
{
  if (FirstBuffer)
    *FirstBuffer = Packet->Private.Head;
  if (TotalPacketLength || BufferCount || PhysicalBufferCount) {
    if (!Packet->Private.ValidCounts) {
      UINT Offset;
      UINT PacketLength;
      PNDIS_BUFFER NdisBuffer;
      UINT _PhysicalBufferCount = 0;
      UINT _TotalPacketLength = 0;
      UINT Count = 0;

      for (NdisBuffer = Packet->Private.Head;
           NdisBuffer != (PNDIS_BUFFER)NULL;
           NdisBuffer = NdisBuffer->Next) {
        _PhysicalBufferCount += NDIS_BUFFER_TO_SPAN_PAGES(NdisBuffer);
        NdisQueryBufferOffset(NdisBuffer, &Offset, &PacketLength);
        _TotalPacketLength += PacketLength;
        Count++;
      }
      Packet->Private.PhysicalCount = _PhysicalBufferCount;
      Packet->Private.TotalLength = _TotalPacketLength;
      Packet->Private.Count = Count;
      Packet->Private.ValidCounts = TRUE;
    }

    if (PhysicalBufferCount)
      *PhysicalBufferCount = Packet->Private.PhysicalCount;

    if (BufferCount)
      *BufferCount = Packet->Private.Count;

    if (TotalPacketLength)
      *TotalPacketLength = Packet->Private.TotalLength;
  }
}

/*
 * VOID
 * NdisQueryPacketLength(
 *   IN PNDIS_PACKET Packet,
 *   OUT PUINT PhysicalBufferCount OPTIONAL,
 *   OUT PUINT BufferCount OPTIONAL,
 *   OUT PNDIS_BUFFER *FirstBuffer OPTIONAL,
 *   OUT PUINT TotalPacketLength OPTIONAL);
 */
#define NdisQueryPacketLength(_Packet,                              \
                              _TotalPacketLength)                   \
{                                                                   \
  if (!(_Packet)->Private.ValidCounts) {                            \
    NdisQueryPacket(_Packet, NULL, NULL, NULL, _TotalPacketLength); \
  }                                                                 \
  else *(_TotalPacketLength) = (_Packet)->Private.TotalLength;      \
}

#endif /* NDIS_LEGACY_DRIVER */

/* Memory management routines */

/*
NDISAPI
VOID
NTAPI
NdisCreateLookaheadBufferFromSharedMemory(
  IN PVOID pSharedMemory,
  IN UINT LookaheadLength,
  OUT PVOID *pLookaheadBuffer);
*/
#define NdisCreateLookaheadBufferFromSharedMemory(_S, _L, _B) ((*(_B)) = (_S))

NDISAPI
VOID
NTAPI
NdisDestroyLookaheadBufferFromSharedMemory(
  IN PVOID pLookaheadBuffer);

#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_ARM) || defined(_M_PPC)

/*
 * VOID
 * NdisMoveMappedMemory(
 *   OUT PVOID  Destination,
 *   IN PVOID  Source,
 *   IN ULONG  Length);
 */
#define NdisMoveMappedMemory(Destination, Source, Length) \
  RtlCopyMemory(Destination, Source, Length)

/*
 * VOID
 * NdisZeroMappedMemory(
 *   IN PVOID  Destination,
 *   IN ULONG  Length);
 */
#define NdisZeroMappedMemory(Destination, Length) \
  RtlZeroMemory(Destination, Length)

#else

#define NdisMoveMappedMemory(Destination, Source, Length) \
{ \
  PUCHAR _Dest = Destination, _Src = Source, _End = _Dest + Length; \
  while (_Dest < _End) \
    *_Dest++ = _Src++; \
}

#define NdisZeroMappedMemory(Destination, Length) \
{ \
  PUCHAR _Dest = Destination, _End = _Dest + Length; \
  while (_Dest < _End) \
    *_Dest++ = 0; \
}

#endif /* _M_IX86 or _M_AMD64 */

/*
 * VOID
 * NdisMoveFromMappedMemory(
 *   OUT PVOID  Destination,
 *   IN PVOID  Source,
 *   IN ULONG  Length);
 */
#define NdisMoveFromMappedMemory(Destination, Source, Length) \
  NdisMoveMappedMemory(Destination, Source, Length)

/*
 * VOID
 * NdisMoveToMappedMemory(
 *   OUT PVOID  Destination,
 *   IN PVOID  Source,
 *   IN ULONG  Length);
 */
#define NdisMoveToMappedMemory(Destination, Source, Length) \
  NdisMoveMappedMemory(Destination, Source, Length)

/*
 * VOID
 * NdisMUpdateSharedMemory(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN ULONG  Length,
 *   IN PVOID  VirtualAddress,
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define NdisMUpdateSharedMemory(_H, _L, _V, _P) \
  NdisUpdateSharedMemory(_H, _L, _V, _P)

_When_(MemoryFlags==0, _IRQL_requires_max_(DISPATCH_LEVEL))
_When_(MemoryFlags==NDIS_MEMORY_CONTIGUOUS, _IRQL_requires_(PASSIVE_LEVEL))
_When_(MemoryFlags==NDIS_MEMORY_NONCACHED, _IRQL_requires_max_(APC_LEVEL))
NDISAPI
VOID
NTAPI
NdisFreeMemory(
  _In_reads_bytes_(Length) __drv_freesMem(Mem) PVOID VirtualAddress,
  _In_ UINT Length,
  _In_ _Pre_satisfies_(MemoryFlags == 0 || MemoryFlags == NDIS_MEMORY_NONCACHED || MemoryFlags == NDIS_MEMORY_CONTIGUOUS)
    UINT MemoryFlags);

NDISAPI
VOID
NTAPI
NdisFreeMemoryWithTag(
  IN PVOID VirtualAddress,
  IN ULONG Tag);

NDISAPI
VOID
NTAPI
NdisImmediateReadSharedMemory(
  IN NDIS_HANDLE WrapperConfigurationContext,
  IN ULONG       SharedMemoryAddress,
  OUT PUCHAR      Buffer,
  IN ULONG       Length);

NDISAPI
VOID
NTAPI
NdisImmediateWriteSharedMemory(
  IN NDIS_HANDLE WrapperConfigurationContext,
  IN ULONG       SharedMemoryAddress,
  IN PUCHAR      Buffer,
  IN ULONG       Length);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMAllocateSharedMemory(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ ULONG Length,
  _In_ BOOLEAN Cached,
  _Outptr_result_bytebuffer_(Length) _At_(*VirtualAddress, _Must_inspect_result_)
    PVOID *VirtualAddress,
  _Out_ _At_(*PhysicalAddress, _Must_inspect_result_)
    PNDIS_PHYSICAL_ADDRESS PhysicalAddress);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMAllocateSharedMemoryAsync(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ ULONG Length,
  _In_ BOOLEAN Cached,
  _In_ PVOID Context);

#if defined(NDIS50)

#define NdisUpdateSharedMemory(NdisAdapterHandle, \
                               Length,            \
                               VirtualAddress,    \
                               PhysicalAddress)

#else

NDISAPI
VOID
NTAPI
NdisUpdateSharedMemory(
  IN NDIS_HANDLE             NdisAdapterHandle,
  IN ULONG                   Length,
  IN PVOID                   VirtualAddress,
  IN NDIS_PHYSICAL_ADDRESS   PhysicalAddress);

#endif /* defined(NDIS50) */

/*
 * ULONG
 * NdisGetPhysicalAddressHigh(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define NdisGetPhysicalAddressHigh(PhysicalAddress) \
  ((PhysicalAddress).HighPart)

/*
 * VOID
 * NdisSetPhysicalAddressHigh(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress,
 *   IN ULONG  Value);
 */
#define NdisSetPhysicalAddressHigh(PhysicalAddress, Value) \
  ((PhysicalAddress).HighPart) = (Value)

/*
 * ULONG
 * NdisGetPhysicalAddressLow(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress);
 */
#define NdisGetPhysicalAddressLow(PhysicalAddress) \
  ((PhysicalAddress).LowPart)


/*
 * VOID
 * NdisSetPhysicalAddressLow(
 *   IN NDIS_PHYSICAL_ADDRESS  PhysicalAddress,
 *   IN ULONG  Value);
 */
#define NdisSetPhysicalAddressLow(PhysicalAddress, Value) \
  ((PhysicalAddress).LowPart) = (Value)

/*
 * VOID
 * NDIS_PHYSICAL_ADDRESS_CONST(
 *   IN ULONG  Low,
 *   IN LONG  High);
 */
#define NDIS_PHYSICAL_ADDRESS_CONST(Low, High)  \
    { {(ULONG)(Low), (LONG)(High)} }

/*
 * ULONG
 * NdisEqualMemory(
 *  IN CONST VOID  *Source1,
 *  IN CONST VOID  *Source2,
 *  IN ULONG  Length);
 */
#define NdisEqualMemory(Source1, Source2, Length) \
  RtlEqualMemory(Source1, Source2, Length)

/*
 * VOID
 * NdisFillMemory(
 *   IN PVOID  Destination,
 *   IN ULONG  Length,
 *   IN UCHAR  Fill);
 */
#define NdisFillMemory(Destination, Length, Fill) \
  RtlFillMemory(Destination, Length, Fill)

/*
 * VOID
 * NdisMoveMemory(
 *   OUT  PVOID  Destination,
 *   IN PVOID  Source,
 *   IN ULONG  Length);
 */
#define NdisMoveMemory(Destination, Source, Length) \
  RtlCopyMemory(Destination, Source, Length)


/*
 * VOID
 * NdisRetrieveUlong(
 *   IN PULONG  DestinationAddress,
 *   IN PULONG  SourceAddress);
 */
#define NdisRetrieveUlong(DestinationAddress, SourceAddress) \
  RtlRetrieveUlong(DestinationAddress, SourceAddress)


/*
 * VOID
 * NdisStoreUlong(
 *   IN PULONG  DestinationAddress,
 *   IN ULONG  Value);
 */
#define NdisStoreUlong(DestinationAddress, Value) \
  RtlStoreUlong(DestinationAddress, Value)


/*
 * VOID
 * NdisZeroMemory(
 *   IN PVOID  Destination,
 *   IN ULONG  Length)
 */
#define NdisZeroMemory(Destination, Length) \
  RtlZeroMemory(Destination, Length)

typedef VOID
(NTAPI *NDIS_BLOCK_INITIALIZER) (
    IN  PUCHAR  Block,
    IN  SIZE_T  NumberOfBytes
    );

/* Configuration routines */

#if NDIS_LEGACY_DRIVER
_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
NDISAPI
VOID
NTAPI
NdisOpenConfiguration(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE ConfigurationHandle,
  _In_ NDIS_HANDLE WrapperConfigurationContext);
#endif

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
NDISAPI
VOID
NTAPI
NdisReadNetworkAddress(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _Outptr_result_bytebuffer_to_(*NetworkAddressLength, *NetworkAddressLength)
    PVOID *NetworkAddress,
  _Out_ PUINT NetworkAddressLength,
  _In_ NDIS_HANDLE ConfigurationHandle);

NDISAPI
VOID
NTAPI
NdisReadEisaSlotInformation(
  OUT PNDIS_STATUS  Status,
  IN NDIS_HANDLE  WrapperConfigurationContext,
  OUT PUINT  SlotNumber,
  OUT PNDIS_EISA_FUNCTION_INFORMATION  EisaData);

NDISAPI
VOID
NTAPI
NdisReadEisaSlotInformationEx(
  OUT PNDIS_STATUS  Status,
  IN NDIS_HANDLE  WrapperConfigurationContext,
  OUT PUINT  SlotNumber,
  OUT PNDIS_EISA_FUNCTION_INFORMATION  *EisaData,
  OUT PUINT  NumberOfFunctions);

#if NDIS_LEGACY_MINIPORT

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
ULONG
NTAPI
NdisReadPciSlotInformation(
  _In_ NDIS_HANDLE NdisAdapterHandle,
  _In_ ULONG SlotNumber,
  _In_ ULONG Offset,
  _Out_writes_bytes_(Length) PVOID Buffer,
  _In_ ULONG Length);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
ULONG
NTAPI
NdisWritePciSlotInformation(
  _In_ NDIS_HANDLE NdisAdapterHandle,
  _In_ ULONG SlotNumber,
  _In_ ULONG Offset,
  _In_reads_bytes_(Length) PVOID Buffer,
  _In_ ULONG Length);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
ULONG
NTAPI
NdisReadPcmciaAttributeMemory(
  _In_ NDIS_HANDLE NdisAdapterHandle,
  _In_ ULONG Offset,
  _Out_writes_bytes_(Length) PVOID Buffer,
  _In_ ULONG Length);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
ULONG
NTAPI
NdisWritePcmciaAttributeMemory(
  _In_ NDIS_HANDLE NdisAdapterHandle,
  _In_ ULONG Offset,
  _In_reads_bytes_(Length) PVOID Buffer,
  _In_ ULONG Length);

#endif /* NDIS_LEGACY_MINIPORT */

/* String management routines */

/*
NDISAPI
NDIS_STATUS
NTAPI
NdisAnsiStringToUnicodeString(
  IN OUT PNDIS_STRING DestinationString,
  IN PNDIS_ANSI_STRING SourceString);
*/
#define NdisAnsiStringToUnicodeString(_us, _as) RtlAnsiStringToUnicodeString(_us, _as, FALSE)

/*
 * BOOLEAN
 * NdisEqualString(
 *   IN PNDIS_STRING String1,
 *   IN PNDIS_STRING String2,
 *   IN BOOLEAN CaseInsensitive);
 */
#define NdisEqualString RtlEqualString

#define NdisEqualUnicodeString RtlEqualUnicodeString

/*
NDISAPI
VOID
NTAPI
NdisInitAnsiString(
  IN OUT PNDIS_ANSI_STRING DestinationString,
  IN PCSTR SourceString);
*/
#define NdisInitAnsiString RtlInitString

NDISAPI
VOID
NTAPI
NdisInitUnicodeString(
  IN OUT PNDIS_STRING  DestinationString,
  IN PCWSTR  SourceString);

/*
NDISAPI
NDIS_STATUS
NTAPI
NdisUnicodeStringToAnsiString(
  IN OUT PNDIS_ANSI_STRING DestinationString,
  IN PNDIS_STRING SourceString);
*/
#define NdisUnicodeStringToAnsiString(_as, _us) RtlUnicodeStringToAnsiString(_as, _us, FALSE)

#define NdisFreeString(_s) NdisFreeMemory((_s).Buffer, (_s).MaximumLength, 0)
#define NdisPrintString(_s) DbgPrint("%ls", (_s).Buffer)

/* Spin lock reoutines */

/*
NDISAPI
VOID
NTAPI
NdisAllocateSpinLock(
  IN PNDIS_SPIN_LOCK SpinLock);
*/
#define NdisAllocateSpinLock(_SpinLock) KeInitializeSpinLock(&(_SpinLock)->SpinLock)

/*
NDISAPI
VOID
NTAPI
NdisFreeSpinLock(
  IN PNDIS_SPIN_LOCK  SpinLock);
*/
#define NdisFreeSpinLock(_SpinLock)

/*
NDISAPI
VOID
NTAPI
NdisAcquireSpinLock(
  IN PNDIS_SPIN_LOCK  SpinLock);
*/
#define NdisAcquireSpinLock(_SpinLock) KeAcquireSpinLock(&(_SpinLock)->SpinLock, &(_SpinLock)->OldIrql)

/*
NDISAPI
VOID
NTAPI
NdisReleaseSpinLock(
  IN PNDIS_SPIN_LOCK  SpinLock);
*/
#define NdisReleaseSpinLock(_SpinLock) KeReleaseSpinLock(&(_SpinLock)->SpinLock,(_SpinLock)->OldIrql)

/*
NDISAPI
VOID
NTAPI
NdisDprAcquireSpinLock(
  IN PNDIS_SPIN_LOCK  SpinLock);
*/
#define NdisDprAcquireSpinLock(_SpinLock) KeAcquireSpinLockAtDpcLevel(&(_SpinLock)->SpinLock)

/*
NDISAPI
VOID
NTAPI
NdisDprReleaseSpinLock(
  IN PNDIS_SPIN_LOCK  SpinLock);
*/
#define NdisDprReleaseSpinLock(_SpinLock) KeReleaseSpinLockFromDpcLevel(&(_SpinLock)->SpinLock)

/* I/O routines */

/*
 * VOID
 * NdisRawReadPortBufferUchar(
 *   IN ULONG Port,
 *   OUT PUCHAR Buffer,
 *   IN ULONG Length);
 */
#define NdisRawReadPortBufferUchar(Port, Buffer, Length) \
  READ_PORT_BUFFER_UCHAR((PUCHAR)(Port), (PUCHAR)(Buffer), (Length))

/*
 * VOID
 * NdisRawReadPortBufferUlong(
 *   IN ULONG Port,
 *   OUT PULONG Buffer,
 *   IN ULONG Length);
 */
#define NdisRawReadPortBufferUlong(Port, Buffer, Length) \
  READ_PORT_BUFFER_ULONG((PULONG)(Port), (PULONG)(Buffer), (Length))

/*
 * VOID
 * NdisRawReadPortBufferUshort(
 *   IN ULONG Port,
 *   OUT PUSHORT Buffer,
 *   IN ULONG Length);
 */
#define NdisRawReadPortBufferUshort(Port, Buffer, Length) \
  READ_PORT_BUFFER_USHORT((PUSHORT)(Port), (PUSHORT)(Buffer), (Length))

/*
 * VOID
 * NdisRawReadPortUchar(
 *   IN ULONG Port,
 *   OUT PUCHAR Data);
 */
#define NdisRawReadPortUchar(Port, Data) \
  *(Data) = READ_PORT_UCHAR((PUCHAR)(Port))

/*
 * VOID
 * NdisRawReadPortUlong(
 *   IN ULONG Port,
 *   OUT PULONG Data);
 */
#define NdisRawReadPortUlong(Port, Data) \
  *(Data) = READ_PORT_ULONG((PULONG)(Port))

/*
 * VOID
 * NdisRawReadPortUshort(
 *   IN ULONG Port,
 *   OUT PUSHORT Data);
 */
#define NdisRawReadPortUshort(Port, Data) \
  *(Data) = READ_PORT_USHORT((PUSHORT)(Port))

/*
 * VOID
 * NdisRawWritePortBufferUchar(
 *   IN ULONG Port,
 *   IN PUCHAR Buffer,
 *   IN ULONG Length);
 */
#define NdisRawWritePortBufferUchar(Port, Buffer, Length) \
  WRITE_PORT_BUFFER_UCHAR((PUCHAR)(Port), (PUCHAR)(Buffer), (Length))

/*
 * VOID
 * NdisRawWritePortBufferUlong(
 *   IN ULONG Port,
 *   IN PULONG Buffer,
 *   IN ULONG Length);
 */
#define NdisRawWritePortBufferUlong(Port, Buffer, Length) \
  WRITE_PORT_BUFFER_ULONG((PULONG)(Port), (PULONG)(Buffer), (Length))

/*
 * VOID
 * NdisRawWritePortBufferUshort(
 *   IN ULONG Port,
 *   IN PUSHORT Buffer,
 *   IN ULONG Length);
 */
#define NdisRawWritePortBufferUshort(Port, Buffer, Length) \
  WRITE_PORT_BUFFER_USHORT((PUSHORT)(Port), (PUSHORT)(Buffer), (Length))

/*
 * VOID
 * NdisRawWritePortUchar(
 *   IN ULONG Port,
 *   IN UCHAR Data);
 */
#define NdisRawWritePortUchar(Port, Data) \
  WRITE_PORT_UCHAR((PUCHAR)(Port), (UCHAR)(Data))

/*
 * VOID
 * NdisRawWritePortUlong(
 *   IN ULONG Port,
 *   IN ULONG Data);
 */
#define NdisRawWritePortUlong(Port, Data) \
  WRITE_PORT_ULONG((PULONG)(Port), (ULONG)(Data))

/*
 * VOID
 * NdisRawWritePortUshort(
 *   IN ULONG Port,
 *   IN USHORT Data);
 */
#define NdisRawWritePortUshort(Port, Data) \
  WRITE_PORT_USHORT((PUSHORT)(Port), (USHORT)(Data))


/*
 * VOID
 * NdisReadRegisterUchar(
 *   IN PUCHAR Register,
 *   OUT PUCHAR Data);
 */
#define NdisReadRegisterUchar(Register, Data) \
  *(Data) = *(Register)

/*
 * VOID
 * NdisReadRegisterUlong(
 *   IN PULONG  Register,
 *   OUT PULONG  Data);
 */
#define NdisReadRegisterUlong(Register, Data)   \
  *(Data) = *(Register)

/*
 * VOID
 * NdisReadRegisterUshort(
 *   IN PUSHORT Register,
 *   OUT PUSHORT Data);
 */
#define NdisReadRegisterUshort(Register, Data) \
    *(Data) = *(Register)

/*
 * VOID
 * NdisReadRegisterUchar(
 *   IN PUCHAR Register,
 *   IN UCHAR Data);
 */
#define NdisWriteRegisterUchar(Register, Data) \
  WRITE_REGISTER_UCHAR((Register), (Data))

/*
 * VOID
 * NdisReadRegisterUlong(
 *   IN PULONG Register,
 *   IN ULONG Data);
 */
#define NdisWriteRegisterUlong(Register, Data) \
  WRITE_REGISTER_ULONG((Register), (Data))

/*
 * VOID
 * NdisReadRegisterUshort(
 *   IN PUSHORT Register,
 *   IN USHORT Data);
 */
#define NdisWriteRegisterUshort(Register, Data) \
  WRITE_REGISTER_USHORT((Register), (Data))


/* Linked lists */

/*
 * VOID
 * NdisInitializeListHead(
 *   IN PLIST_ENTRY ListHead);
 */
#define NdisInitializeListHead InitializeListHead

/*
 * PLIST_ENTRY
 * NdisInterlockedInsertHeadList(
 *   IN PLIST_ENTRY  ListHead,
 *   IN PLIST_ENTRY  ListEntry,
 *   IN PNDIS_SPIN_LOCK  SpinLock);
 */
#define NdisInterlockedInsertHeadList(_ListHead, _ListEntry, _SpinLock) \
  ExInterlockedInsertHeadList(_ListHead, _ListEntry, &(_SpinLock)->SpinLock)

/*
 * PLIST_ENTRY
 * NdisInterlockedInsertTailList(
 *   IN PLIST_ENTRY  ListHead,
 *   IN PLIST_ENTRY  ListEntry,
 *   IN PNDIS_SPIN_LOCK  SpinLock);
 */
#define NdisInterlockedInsertTailList(_ListHead, _ListEntry, _SpinLock) \
  ExInterlockedInsertTailList(_ListHead, _ListEntry, &(_SpinLock)->SpinLock)

/*
 * PLIST_ENTRY
 * NdisInterlockedRemoveHeadList(
 *   IN PLIST_ENTRY  ListHead,
 *   IN PNDIS_SPIN_LOCK  SpinLock);
*/
#define NdisInterlockedRemoveHeadList(_ListHead, _SpinLock) \
  ExInterlockedRemoveHeadList(_ListHead, &(_SpinLock)->SpinLock)

/*
 * VOID
 * NdisInitializeSListHead(
 *   IN PSLIST_HEADER SListHead);
 */
#define NdisInitializeSListHead(SListHead) ExInitializeSListHead(SListHead)

/*
 * USHORT NdisQueryDepthSList(
 *   IN PSLIST_HEADER SListHead);
 */
#define NdisQueryDepthSList(SListHead) ExQueryDepthSList(SListHead)

#define NdisInterlockedPushEntryList(ListHead, ListEntry, Lock) \
  ExInterlockedPushEntryList(ListHead, ListEntry, &(Lock)->SpinLock)

#define NdisInterlockedPopEntryList(ListHead, Lock) \
  ExInterlockedPopEntryList(ListHead, &(Lock)->SpinLock)

/* Non-paged lookaside lists */

#define NdisInitializeNPagedLookasideList(_L, _AR, _FR, _Fl, _S, _T, _D) \
  ExInitializeNPagedLookasideList(_L, _AR, _FR, _Fl, _S, _T, _D)
#define NdisDeleteNPagedLookasideList(_L) ExDeleteNPagedLookasideList(_L)
#define NdisAllocateFromNPagedLookasideList(_L) ExAllocateFromNPagedLookasideList(_L)
#define NdisFreeToNPagedLookasideList(_L, _E) ExFreeToNPagedLookasideList(_L, _E)

/* Interlocked routines */

/*
 * LONG
 * NdisInterlockedDecrement(
 *   IN PLONG  Addend);
 */
#define NdisInterlockedDecrement(Addend) InterlockedDecrement(Addend)

/*
 * LONG
 * NdisInterlockedIncrement(
 *   IN PLONG  Addend);
 */
#define NdisInterlockedIncrement(Addend) InterlockedIncrement(Addend)

/*
 * VOID
 * NdisInterlockedAddUlong(
 *   IN PULONG  Addend,
 *   IN ULONG  Increment,
 *   IN PNDIS_SPIN_LOCK  SpinLock);
 */
#define NdisInterlockedAddUlong(_Addend, _Increment, _SpinLock) \
  ExInterlockedAddUlong(_Addend, _Increment, &(_SpinLock)->SpinLock)

/* Miscellaneous routines */

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisCloseConfiguration(
  _In_ __drv_freesMem(mem) NDIS_HANDLE ConfigurationHandle);

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
NDISAPI
VOID
NTAPI
NdisReadConfiguration(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_CONFIGURATION_PARAMETER *ParameterValue,
  _In_ NDIS_HANDLE ConfigurationHandle,
  _In_ PNDIS_STRING Keyword,
  _In_ NDIS_PARAMETER_TYPE ParameterType);

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
NDISAPI
VOID
NTAPI
NdisWriteConfiguration(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE WrapperConfigurationContext,
  _In_ PNDIS_STRING Keyword,
  _In_ PNDIS_CONFIGURATION_PARAMETER ParameterValue);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
__cdecl
NdisWriteErrorLogEntry(
  _In_ NDIS_HANDLE NdisAdapterHandle,
  _In_ NDIS_ERROR_CODE ErrorCode,
  _In_ ULONG NumberOfErrorValues,
  ...);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisInitializeString(
  _Out_ _At_(Destination->Buffer, __drv_allocatesMem(Mem)) PNDIS_STRING Destination,
  _In_opt_z_ PUCHAR Source);

/*
 * VOID
 * NdisStallExecution(
 *   IN UINT MicrosecondsToStall)
 */
#define NdisStallExecution KeStallExecutionProcessor

/*
NDISAPI
VOID
NTAPI
NdisGetCurrentSystemTime(
  IN PLARGE_INTEGER  pSystemTime);
*/
#define NdisGetCurrentSystemTime KeQuerySystemTime

#if NDIS_SUPPORT_60_COMPATIBLE_API
_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
CCHAR
NTAPI
NdisSystemProcessorCount(VOID);
#endif

NDISAPI
VOID
NTAPI
NdisGetCurrentProcessorCpuUsage(
  OUT PULONG pCpuUsage);

/* NDIS helper macros */

/*
 * VOID
 * NDIS_INIT_FUNCTION(FunctionName)
 */
#define NDIS_INIT_FUNCTION(FunctionName)    \
  alloc_text(init, FunctionName)

/*
 * VOID
 * NDIS_PAGABLE_FUNCTION(FunctionName)
 */
#define NDIS_PAGEABLE_FUNCTION(FunctionName) \
  alloc_text(page, FunctionName)

#define NDIS_PAGABLE_FUNCTION NDIS_PAGEABLE_FUNCTION


/* NDIS 4.0 extensions */

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMFreeSharedMemory(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ ULONG Length,
  _In_ BOOLEAN Cached,
  _In_reads_bytes_(Length) PVOID VirtualAddress,
  _In_ NDIS_PHYSICAL_ADDRESS PhysicalAddress);

NDISAPI
VOID
NTAPI
NdisMWanIndicateReceive(
	OUT PNDIS_STATUS  Status,
	IN NDIS_HANDLE  MiniportAdapterHandle,
	IN NDIS_HANDLE  NdisLinkContext,
	IN PUCHAR  PacketBuffer,
	IN UINT  PacketSize);

NDISAPI
VOID
NTAPI
NdisMWanIndicateReceiveComplete(
  IN NDIS_HANDLE MiniportAdapterHandle,
  IN NDIS_HANDLE NdisLinkContext);

NDISAPI
VOID
NTAPI
NdisMWanSendComplete(
	IN NDIS_HANDLE  MiniportAdapterHandle,
	IN PNDIS_WAN_PACKET  Packet,
	IN NDIS_STATUS  Status);

NDISAPI
NDIS_STATUS
NTAPI
NdisPciAssignResources(
	IN NDIS_HANDLE  NdisMacHandle,
	IN NDIS_HANDLE  NdisWrapperHandle,
	IN NDIS_HANDLE  WrapperConfigurationContext,
	IN ULONG  SlotNumber,
	OUT PNDIS_RESOURCE_LIST  *AssignedResources);


/* NDIS 5.0 extensions */

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisAllocateMemoryWithTag(
  _At_(*VirtualAddress, __drv_allocatesMem(Mem)) _Outptr_result_bytebuffer_(Length)
    PVOID *VirtualAddress,
  _In_ UINT Length,
  _In_ ULONG Tag);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisGetCurrentProcessorCounts(
  _Out_ PULONG pIdleCount,
  _Out_ PULONG pKernelAndUser,
  _Out_ PULONG pIndex);

#if NDIS_LEGACY_DRIVER
NDISAPI
VOID
NTAPI
NdisGetSystemUpTime(
  OUT PULONG pSystemUpTime);
#endif

#if NDIS_SUPPORT_60_COMPATIBLE_API

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_raises_(DISPATCH_LEVEL)
_When_(fWrite, _Acquires_exclusive_lock_(*Lock))
_When_(!fWrite, _Acquires_shared_lock_(*Lock))
NDISAPI
VOID
NTAPI
NdisAcquireReadWriteLock(
  _Inout_ PNDIS_RW_LOCK Lock,
  _In_ BOOLEAN fWrite,
  _Out_ _IRQL_saves_ PLOCK_STATE LockState);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisInitializeReadWriteLock(
  _Out_ PNDIS_RW_LOCK Lock);

_IRQL_requires_(DISPATCH_LEVEL)
_Requires_lock_held_(*Lock)
_Releases_lock_(*Lock)
NDISAPI
VOID
NTAPI
NdisReleaseReadWriteLock(
  _Inout_ PNDIS_RW_LOCK Lock,
  _In_ _IRQL_restores_ PLOCK_STATE LockState);

#if NDIS_SUPPORT_NDIS6

NDISAPI
VOID
NTAPI
NdisDprAcquireReadWriteLock(
  IN PNDIS_RW_LOCK Lock,
  IN BOOLEAN fWrite,
  IN PLOCK_STATE LockState);

NDISAPI
VOID
NTAPI
NdisDprReleaseReadWriteLock(
  IN PNDIS_RW_LOCK Lock,
  IN PLOCK_STATE LockState);

#endif /* NDIS_SUPPORT_NDIS6 */

#endif /* NDIS_SUPPORT_60_COMPATIBLE_API */

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMDeregisterDevice(
  _In_ NDIS_HANDLE NdisDeviceHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMGetDeviceProperty(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _Inout_opt_ PDEVICE_OBJECT *PhysicalDeviceObject,
  _Inout_opt_ PDEVICE_OBJECT *FunctionalDeviceObject,
  _Inout_opt_ PDEVICE_OBJECT *NextDeviceObject,
  _Inout_opt_ PCM_RESOURCE_LIST *AllocatedResources,
  _Inout_opt_ PCM_RESOURCE_LIST *AllocatedResourcesTranslated);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMInitializeScatterGatherDma(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ BOOLEAN Dma64BitAddresses,
  _In_ ULONG MaximumPhysicalMapping);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMPromoteMiniport(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMQueryAdapterInstanceName(
  _Out_ PNDIS_STRING AdapterInstanceName,
  _In_ NDIS_HANDLE MiniportAdapterHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMRegisterDevice(
  _In_ NDIS_HANDLE NdisWrapperHandle,
  _In_ PNDIS_STRING DeviceName,
  _In_ PNDIS_STRING SymbolicName,
  _In_reads_(IRP_MJ_PNP) PDRIVER_DISPATCH *MajorFunctions,
  _Out_ PDEVICE_OBJECT *pDeviceObject,
  _Out_ NDIS_HANDLE *NdisDeviceHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMRegisterUnloadHandler(
  _In_ NDIS_HANDLE NdisWrapperHandle,
  _In_ PDRIVER_UNLOAD UnloadHandler);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMRemoveMiniport(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMSetMiniportSecondary(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_HANDLE PrimaryMiniportAdapterHandle);

_IRQL_requires_max_(APC_LEVEL)
_Success_(*Status >= 0)
NDISAPI
VOID
NTAPI
NdisOpenConfigurationKeyByIndex(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE ConfigurationHandle,
  _In_ ULONG Index,
  _Out_ PNDIS_STRING KeyName,
  _Out_ PNDIS_HANDLE KeyHandle);

_IRQL_requires_max_(APC_LEVEL)
_Success_(*Status >= 0)
NDISAPI
VOID
NTAPI
NdisOpenConfigurationKeyByName(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE ConfigurationHandle,
  _In_ PNDIS_STRING SubKeyName,
  _Out_ PNDIS_HANDLE SubKeyHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisQueryAdapterInstanceName(
  _Out_ PNDIS_STRING AdapterInstanceName,
  _In_ NDIS_HANDLE NdisBindingHandle);

_Must_inspect_result_
NDISAPI
NDIS_STATUS
NTAPI
NdisQueryBindInstanceName(
  _Out_ PNDIS_STRING pAdapterInstanceName,
  _In_ NDIS_HANDLE BindingContext);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisWriteEventLogEntry(
  _In_ _Points_to_data_ PVOID LogHandle,
  _In_ NDIS_STATUS EventCode,
  _In_ ULONG UniqueEventValue,
  _In_ USHORT NumStrings,
  _In_opt_ PVOID StringsList,
  _In_ ULONG DataSize,
  _In_reads_bytes_opt_(DataSize) PVOID Data);

/* Connectionless services */

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClAddParty(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ NDIS_HANDLE ProtocolPartyContext,
  _In_ PCO_CALL_PARAMETERS CallParameters,
  _Out_ PNDIS_HANDLE NdisPartyHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClCloseAddressFamily(
  _In_ NDIS_HANDLE NdisAfHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClCloseCall(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_opt_ NDIS_HANDLE NdisPartyHandle,
  _In_reads_bytes_opt_(Size) PVOID Buffer,
  _In_ UINT Size);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClDeregisterSap(
  _In_ NDIS_HANDLE NdisSapHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClDropParty(
  _In_ NDIS_HANDLE NdisPartyHandle,
  _In_reads_bytes_opt_(Size) PVOID Buffer,
  _In_opt_ UINT Size);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisClIncomingCallComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClMakeCall(
  _In_ NDIS_HANDLE NdisVcHandle,
  _Inout_ PCO_CALL_PARAMETERS CallParameters,
  _In_opt_ NDIS_HANDLE ProtocolPartyContext,
  _Out_opt_ PNDIS_HANDLE NdisPartyHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClModifyCallQoS(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClOpenAddressFamily(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ PCO_ADDRESS_FAMILY AddressFamily,
  _In_ NDIS_HANDLE ProtocolAfContext,
  _In_ PNDIS_CLIENT_CHARACTERISTICS ClCharacteristics,
  _In_ UINT SizeOfClCharacteristics,
  _Out_ PNDIS_HANDLE NdisAfHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisClRegisterSap(
  _In_ NDIS_HANDLE NdisAfHandle,
  _In_ NDIS_HANDLE ProtocolSapContext,
  _In_ PCO_SAP Sap,
  _Out_ PNDIS_HANDLE NdisSapHandle);


/* Call Manager services */

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCmActivateVc(
  _In_ NDIS_HANDLE NdisVcHandle,
  _Inout_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmAddPartyComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisPartyHandle,
  _In_opt_ NDIS_HANDLE CallMgrPartyContext,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmCloseAddressFamilyComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisAfHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmCloseCallComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_opt_ NDIS_HANDLE NdisPartyHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCmDeactivateVc(
  _In_ NDIS_HANDLE NdisVcHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmDeregisterSapComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisSapHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmDispatchCallConnected(
  _In_ NDIS_HANDLE NdisVcHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCmDispatchIncomingCall(
  _In_ NDIS_HANDLE NdisSapHandle,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmDispatchIncomingCallQoSChange(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmDispatchIncomingCloseCall(
  _In_ NDIS_STATUS CloseStatus,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_reads_bytes_opt_(Size) PVOID Buffer,
  _In_ UINT Size);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmDispatchIncomingDropParty(
  _In_ NDIS_STATUS DropStatus,
  _In_ NDIS_HANDLE NdisPartyHandle,
  _In_reads_bytes_opt_(Size) PVOID Buffer,
  _In_ UINT Size);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmDropPartyComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisPartyHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmMakeCallComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_opt_ NDIS_HANDLE NdisPartyHandle,
  _In_opt_ NDIS_HANDLE CallMgrPartyContext,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmModifyCallQoSComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmOpenAddressFamilyComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisAfHandle,
  _In_ NDIS_HANDLE CallMgrAfContext);

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCmRegisterAddressFamily(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ PCO_ADDRESS_FAMILY AddressFamily,
  _In_ PNDIS_CALL_MANAGER_CHARACTERISTICS CmCharacteristics,
  _In_ UINT SizeOfCmCharacteristics);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCmRegisterSapComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisSapHandle,
  _In_ NDIS_HANDLE CallMgrSapContext);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMCmActivateVc(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMCmCreateVc(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_HANDLE NdisAfHandle,
  _In_ NDIS_HANDLE MiniportVcContext,
  _Out_ PNDIS_HANDLE NdisVcHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMCmDeactivateVc(
  _In_ NDIS_HANDLE NdisVcHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMCmDeleteVc(
  _In_ NDIS_HANDLE NdisVcHandle);

NDISAPI
NDIS_STATUS
NTAPI
NdisMCmRegisterAddressFamily(
  IN NDIS_HANDLE  MiniportAdapterHandle,
  IN PCO_ADDRESS_FAMILY  AddressFamily,
  IN PNDIS_CALL_MANAGER_CHARACTERISTICS  CmCharacteristics,
  IN UINT  SizeOfCmCharacteristics);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMCmRequest(
  _In_ NDIS_HANDLE NdisAfHandle,
  _In_opt_ NDIS_HANDLE NdisVcHandle,
  _In_opt_ NDIS_HANDLE NdisPartyHandle,
  _Inout_ PNDIS_REQUEST NdisRequest);


/* Connection-oriented services */

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCoCreateVc(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_opt_ NDIS_HANDLE NdisAfHandle ,
  _In_ NDIS_HANDLE ProtocolVcContext,
  _Inout_ PNDIS_HANDLE NdisVcHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCoDeleteVc(
  _In_ NDIS_HANDLE NdisVcHandle);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisCoRequest(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_opt_ NDIS_HANDLE NdisAfHandle,
  _In_opt_ NDIS_HANDLE NdisVcHandle,
  _In_opt_ NDIS_HANDLE NdisPartyHandle,
  _Inout_ PNDIS_REQUEST NdisRequest);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCoRequestComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisAfHandle,
  _In_opt_ NDIS_HANDLE NdisVcHandle,
  _In_opt_ NDIS_HANDLE NdisPartyHandle,
  _In_ PNDIS_REQUEST NdisRequest);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCoSendPackets(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PPNDIS_PACKET PacketArray,
  _In_ UINT NumberOfPackets);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoActivateVcComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PCO_CALL_PARAMETERS CallParameters);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoDeactivateVcComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoIndicateReceivePacket(
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PPNDIS_PACKET PacketArray,
  _In_ UINT NumberOfPackets);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoIndicateStatus(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_opt_ NDIS_HANDLE NdisVcHandle,
  _In_ NDIS_STATUS GeneralStatus,
  _In_reads_bytes_opt_(StatusBufferSize) PVOID StatusBuffer,
  _In_ ULONG StatusBufferSize);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoReceiveComplete(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoRequestComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ PNDIS_REQUEST Request);

_IRQL_requires_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCoSendComplete(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisVcHandle,
  _In_ PNDIS_PACKET Packet);


/* NDIS 5.0 extensions for intermediate drivers */

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisIMAssociateMiniport(
  _In_ NDIS_HANDLE DriverHandle,
  _In_ NDIS_HANDLE ProtocolHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisIMCancelInitializeDeviceInstance(
  _In_ NDIS_HANDLE DriverHandle,
  _In_ PNDIS_STRING DeviceInstance);

NDISAPI
VOID
NTAPI
NdisIMCopySendCompletePerPacketInfo(
  IN PNDIS_PACKET  DstPacket,
  IN PNDIS_PACKET  SrcPacket);

NDISAPI
VOID
NTAPI
NdisIMCopySendPerPacketInfo(
  IN PNDIS_PACKET  DstPacket,
  IN PNDIS_PACKET  SrcPacket);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisIMDeregisterLayeredMiniport(
  _In_ NDIS_HANDLE DriverHandle);

_IRQL_requires_max_(APC_LEVEL)
NDISAPI
NDIS_HANDLE
NTAPI
NdisIMGetBindingContext(
  _In_ NDIS_HANDLE NdisBindingHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_HANDLE
NTAPI
NdisIMGetDeviceContext(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisIMInitializeDeviceInstanceEx(
  _In_ NDIS_HANDLE DriverHandle,
  _In_ PNDIS_STRING DriverInstance,
  _In_opt_ NDIS_HANDLE DeviceContext);

/*
NDISAPI
PSINGLE_LIST_ENTRY
NTAPI
NdisInterlockedPopEntrySList(
  IN PSLIST_HEADER ListHead,
  IN PKSPIN_LOCK Lock);
*/
#define NdisInterlockedPopEntrySList(SListHead, Lock) \
  ExInterlockedPopEntrySList(SListHead, &(Lock)->SpinLock)

/*
NDISAPI
PSINGLE_LIST_ENTRY
NTAPI
NdisInterlockedPushEntrySList(
  IN PSLIST_HEADER ListHead,
  IN PSINGLE_LIST_ENTRY ListEntry,
  IN PKSPIN_LOCK Lock);
*/
#define NdisInterlockedPushEntrySList(SListHead, SListEntry, Lock) \
  ExInterlockedPushEntrySList(SListHead, SListEntry, &(Lock)->SpinLock)

#define NdisInterlockedFlushSList(SListHead) ExInterlockedFlushSList(SListHead)

/*
NDISAPI
VOID
NTAPI
NdisQueryBufferSafe(
  IN PNDIS_BUFFER Buffer,
  OUT PVOID *VirtualAddress OPTIONAL,
  OUT PUINT Length,
  IN UINT Priority);
*/
#define NdisQueryBufferSafe(_Buffer, _VirtualAddress, _Length, _Priority) {         \
  if (ARGUMENT_PRESENT(_VirtualAddress)) {                                          \
    *(PVOID *)(_VirtualAddress) = MmGetSystemAddressForMdlSafe(_Buffer, _Priority); \
  }                                                                                 \
  *(_Length) = MmGetMdlByteCount(_Buffer);                                          \
}

/* Routines for NDIS miniport drivers */

#if NDIS_SUPPORT_NDIS6

NDISAPI
PNDIS_GENERIC_OBJECT
NTAPI
NdisAllocateGenericObject(
  PDRIVER_OBJECT DriverObject OPTIONAL,
  ULONG Tag,
  USHORT Size);

NDISAPI
VOID
NTAPI
NdisFreeGenericObject(
  IN PNDIS_GENERIC_OBJECT NdisObject);

#endif /* NDIS_SUPPORT_NDIS6 */

NDISAPI
VOID
NTAPI
NdisInitializeWrapper(
  OUT PNDIS_HANDLE  NdisWrapperHandle,
  IN PVOID  SystemSpecific1,
  IN PVOID  SystemSpecific2,
  IN PVOID  SystemSpecific3);

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
__drv_preferredFunction("NdisMInitializeScatterGatherDma", "See details in NdisMAllocateMapRegisters documentation")
NDISAPI
NDIS_STATUS
NTAPI
NdisMAllocateMapRegisters(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ UINT DmaChannel,
  _In_ NDIS_DMA_SIZE DmaSize,
  _In_ ULONG PhysicalMapRegistersNeeded,
  _In_ ULONG MaximumPhysicalMapping);

/*
 * VOID
 * NdisMArcIndicateReceive(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN PUCHAR  HeaderBuffer,
 *   IN PUCHAR  DataBuffer,
 *   IN UINT  Length);
 */
#define NdisMArcIndicateReceive(MiniportAdapterHandle, \
                                HeaderBuffer,          \
                                DataBuffer,            \
                                Length)                \
{                                                      \
    ArcFilterDprIndicateReceive(                       \
        (((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->ArcDB), \
        (HeaderBuffer), \
        (DataBuffer),   \
        (Length));      \
}

/*
 * VOID
 * NdisMArcIndicateReceiveComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle);
 */
#define NdisMArcIndicateReceiveComplete(MiniportAdapterHandle) \
{                                                              \
    if (((PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle)->EthDB)  \
	    {                                                        \
	        NdisMEthIndicateReceiveComplete(_H);                 \
	    }                                                        \
                                                               \
    ArcFilterDprIndicateReceiveComplete(                       \
      ((PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle)->ArcDB);   \
}

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCloseLog(
  _In_ NDIS_HANDLE LogHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMCreateLog(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ UINT Size,
  _Out_ PNDIS_HANDLE LogHandle);

NDISAPI
VOID
NTAPI
NdisMDeregisterAdapterShutdownHandler(
  _In_ NDIS_HANDLE MiniportHandle);

#if NDIS_LEGACY_MINIPORT

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMDeregisterInterrupt(
  _In_ PNDIS_MINIPORT_INTERRUPT Interrupt);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMRegisterAdapterShutdownHandler(
  _In_ NDIS_HANDLE MiniportHandle,
  _In_ PVOID ShutdownContext,
  _In_ ADAPTER_SHUTDOWN_HANDLER ShutdownHandler);

NDISAPI
NDIS_STATUS
NTAPI
NdisMRegisterInterrupt(
  OUT PNDIS_MINIPORT_INTERRUPT Interrupt,
  IN NDIS_HANDLE MiniportAdapterHandle,
  IN UINT InterruptVector,
  IN UINT InterruptLevel,
  IN BOOLEAN RequestIsr,
  IN BOOLEAN SharedInterrupt,
  IN NDIS_INTERRUPT_MODE InterruptMode);

NDISAPI
NDIS_STATUS
NTAPI
NdisMRegisterMiniport(
  IN NDIS_HANDLE NdisWrapperHandle,
  IN PNDIS_MINIPORT_CHARACTERISTICS MiniportCharacteristics,
  IN UINT CharacteristicsLength);

NDISAPI
BOOLEAN
NTAPI
NdisMSynchronizeWithInterrupt(
  IN PNDIS_MINIPORT_INTERRUPT Interrupt,
  IN PVOID SynchronizeFunction,
  IN PVOID SynchronizeContext);
#endif /* NDIS_LEGACY_MINIPORT */

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMDeregisterIoPortRange(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ UINT InitialPort,
  _In_ UINT NumberOfPorts,
  _In_ PVOID PortOffset);

/*
 * VOID
 * NdisMEthIndicateReceive(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN NDIS_HANDLE  MiniportReceiveContext,
 *   IN PVOID  HeaderBuffer,
 *   IN UINT  HeaderBufferSize,
 *   IN PVOID  LookaheadBuffer,
 *   IN UINT  LookaheadBufferSize,
 *   IN UINT  PacketSize);
 */
#define NdisMEthIndicateReceive(MiniportAdapterHandle,  \
                                MiniportReceiveContext, \
                                HeaderBuffer,           \
                                HeaderBufferSize,       \
                                LookaheadBuffer,        \
                                LookaheadBufferSize,    \
                                PacketSize)             \
{                                                       \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->EthRxIndicateHandler)( \
		((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->EthDB,  \
		(MiniportReceiveContext), \
		(HeaderBuffer),           \
		(HeaderBuffer),           \
		(HeaderBufferSize),       \
		(LookaheadBuffer),        \
		(LookaheadBufferSize),    \
		(PacketSize));            \
}

/*
 * VOID
 * NdisMEthIndicateReceiveComplete(
 *   IN NDIS_HANDLE MiniportAdapterHandle);
 */
#define NdisMEthIndicateReceiveComplete(MiniportAdapterHandle) \
{                                                              \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->EthRxCompleteHandler)( \
        ((PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle)->EthDB);    \
}

/*
 * VOID
 * NdisMFddiIndicateReceive(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN NDIS_HANDLE  MiniportReceiveContext,
 *   IN PVOID  HeaderBuffer,
 *   IN UINT  HeaderBufferSize,
 *   IN PVOID  LookaheadBuffer,
 *   IN UINT  LookaheadBufferSize,
 *   IN UINT  PacketSize);
 */
#define NdisMFddiIndicateReceive(MiniportAdapterHandle,  \
                                 MiniportReceiveContext, \
                                 HeaderBuffer,           \
                                 HeaderBufferSize,       \
                                 LookaheadBuffer,        \
                                 LookaheadBufferSize,    \
                                 PacketSize)             \
{                                                        \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->FddiRxIndicateHandler)( \
        (((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->FddiDB),   \
        (MiniportReceiveContext),              \
        (PUCHAR)(HeaderBuffer) + 1,            \
        (((*(PUCHAR*)(HeaderBuffer)) & 0x40) ? \
            FDDI_LENGTH_OF_LONG_ADDRESS :      \
		    FDDI_LENGTH_OF_SHORT_ADDRESS),     \
        (HeaderBuffer),                        \
        (HeaderBufferSize),                    \
        (LookaheadBuffer),                     \
        (LookaheadBufferSize),                 \
        (PacketSize));                         \
}



/*
 * VOID
 * NdisMFddiIndicateReceiveComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle);
 */
#define NdisMFddiIndicateReceiveComplete(MiniportAdapterHandle) \
{                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->FddiRxCompleteHandler)( \
        ((PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle)->FddiDB);      \
}

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMFlushLog(
  _In_ NDIS_HANDLE LogHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMFreeMapRegisters(
  _In_ NDIS_HANDLE MiniportAdapterHandle);

/*
 * VOID
 * EXPORT
 * NdisMIndicateReceivePacket(
 *  IN NDIS_HANDLE  MiniportAdapterHandle,
 *  IN PPNDIS_PACKET  ReceivePackets,
 *  IN UINT  NumberOfPackets);
 */
#define NdisMIndicateReceivePacket(MiniportAdapterHandle, \
  ReceivePackets, NumberOfPackets)                        \
  (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->PacketIndicateHandler)( \
  MiniportAdapterHandle, ReceivePackets, NumberOfPackets)

/*
 * VOID
 * NdisMIndicateStatus(
 *  IN NDIS_HANDLE  MiniportAdapterHandle,
 *  IN NDIS_STATUS  GeneralStatus,
 *  IN PVOID  StatusBuffer,
 *  IN UINT  StatusBufferSize);
 */

#define NdisMIndicateStatus(MiniportAdapterHandle,  \
   GeneralStatus, StatusBuffer, StatusBufferSize)   \
  (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->StatusHandler)(   \
  MiniportAdapterHandle, GeneralStatus, StatusBuffer, StatusBufferSize)

/*
 * VOID
 * NdisMIndicateStatusComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle);
 */
#define NdisMIndicateStatusComplete(MiniportAdapterHandle) \
  (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->StatusCompleteHandler)( \
    MiniportAdapterHandle)

/*
 * VOID
 * NdisMInitializeWrapper(
 *   OUT PNDIS_HANDLE  NdisWrapperHandle,
 *   IN PVOID  SystemSpecific1,
 *   IN PVOID  SystemSpecific2,
 *   IN PVOID  SystemSpecific3);
 */
#define NdisMInitializeWrapper(NdisWrapperHandle, \
                               SystemSpecific1,   \
                               SystemSpecific2,   \
                               SystemSpecific3)   \
    NdisInitializeWrapper((NdisWrapperHandle),    \
                          (SystemSpecific1),      \
                          (SystemSpecific2),      \
                          (SystemSpecific3))

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMMapIoSpace(
  _Outptr_result_bytebuffer_(Length) PVOID *VirtualAddress,
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ NDIS_PHYSICAL_ADDRESS PhysicalAddress,
  _In_ UINT Length);

/*
 * VOID
 * NdisMQueryInformationComplete(
 *  IN NDIS_HANDLE  MiniportAdapterHandle,
 *  IN NDIS_STATUS  Status);
 */
#define NdisMQueryInformationComplete(MiniportAdapterHandle, Status) \
  (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->QueryCompleteHandler)(MiniportAdapterHandle, Status)

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMRegisterIoPortRange(
  _Out_ PVOID *PortOffset,
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ UINT InitialPort,
  _In_ UINT NumberOfPorts);

NDISAPI
VOID
NTAPI
NdisMSetTimer(
  IN PNDIS_MINIPORT_TIMER  Timer,
  IN UINT  MillisecondsToDelay);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMInitializeTimer(
  _Inout_ PNDIS_MINIPORT_TIMER Timer,
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_ PNDIS_TIMER_FUNCTION TimerFunction,
  _In_ PVOID FunctionContext);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMSetPeriodicTimer(
  _In_ PNDIS_MINIPORT_TIMER Timer,
  _In_ UINT MillisecondPeriod);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisMCancelTimer(
  _In_ PNDIS_MINIPORT_TIMER Timer,
  _Out_ _At_(*TimerCancelled, _Must_inspect_result_) PBOOLEAN TimerCancelled);

#if !defined(NDIS_WRAPPER)

/*
 * VOID
 * NdisMResetComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN NDIS_STATUS  Status,
 *   IN BOOLEAN  AddressingReset);
 */
#define NdisMResetComplete(MiniportAdapterHandle, \
                           Status,                \
                           AddressingReset)       \
{                                                 \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->ResetCompleteHandler)( \
        MiniportAdapterHandle, Status, AddressingReset); \
}

/*
 * VOID
 * NdisMSendComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN PNDIS_PACKET  Packet,
 *   IN NDIS_STATUS  Status);
 */
#define	NdisMSendComplete(MiniportAdapterHandle, \
                          Packet,                \
                          Status)                \
{                                                \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->SendCompleteHandler)( \
        MiniportAdapterHandle, Packet, Status);  \
}

/*
 * VOID
 * NdisMSendResourcesAvailable(
 *   IN NDIS_HANDLE  MiniportAdapterHandle);
 */
#define	NdisMSendResourcesAvailable(MiniportAdapterHandle) \
{                                                \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->SendResourcesHandler)( \
        MiniportAdapterHandle); \
}

/*
 * VOID
 * NdisMTransferDataComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN PNDIS_PACKET  Packet,
 *   IN NDIS_STATUS  Status,
 *   IN UINT  BytesTransferred);
 */
#define	NdisMTransferDataComplete(MiniportAdapterHandle, \
                                  Packet,                \
                                  Status,                \
                                  BytesTransferred)      \
{                                                        \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->TDCompleteHandler)( \
        MiniportAdapterHandle, Packet, Status, BytesTransferred)           \
}

#endif /* !_NDIS_ */


/*
 * VOID
 * NdisMSetAttributes(
 *  IN NDIS_HANDLE  MiniportAdapterHandle,
 *  IN NDIS_HANDLE  MiniportAdapterContext,
 *  IN BOOLEAN  BusMaster,
 *  IN NDIS_INTERFACE_TYPE  AdapterType);
 */
#define NdisMSetAttributes(MiniportAdapterHandle,   \
                           MiniportAdapterContext,  \
                           BusMaster,               \
                           AdapterType)             \
  NdisMSetAttributesEx(MiniportAdapterHandle,       \
    MiniportAdapterContext,                         \
    0,                                              \
    (BusMaster) ? NDIS_ATTRIBUTE_BUS_MASTER : 0,    \
    AdapterType)

NDISAPI
VOID
NTAPI
NdisMSetAttributesEx(
  IN NDIS_HANDLE  MiniportAdapterHandle,
  IN NDIS_HANDLE  MiniportAdapterContext,
  IN UINT  CheckForHangTimeInSeconds   OPTIONAL,
  IN ULONG  AttributeFlags,
  IN NDIS_INTERFACE_TYPE AdapterType);

/*
 * VOID
 * NdisMSetInformationComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN NDIS_STATUS  Status);
 */
#define NdisMSetInformationComplete(MiniportAdapterHandle, \
                                    Status) \
  (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->SetCompleteHandler)( \
    MiniportAdapterHandle, Status)

_IRQL_requires_max_(APC_LEVEL)
NDISAPI
VOID
NTAPI
NdisMSleep(
  _In_ ULONG MicrosecondsToSleep);

/*
 * VOID
 * NdisMTrIndicateReceive(
 *   IN NDIS_HANDLE  MiniportAdapterHandle,
 *   IN NDIS_HANDLE  MiniportReceiveContext,
 *   IN PVOID  HeaderBuffer,
 *   IN UINT  HeaderBufferSize,
 *   IN PVOID  LookaheadBuffer,
 *   IN UINT  LookaheadBufferSize,
 *   IN UINT  PacketSize);
 */
#define NdisMTrIndicateReceive(MiniportAdapterHandle,  \
                               MiniportReceiveContext, \
                               HeaderBuffer,           \
                               HeaderBufferSize,       \
                               LookaheadBuffer,        \
                               LookaheadBufferSize,    \
                               PacketSize)             \
{                                                      \
    (*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->TrRxIndicateHandler)( \
      (((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->TrDB),     \
		(MiniportReceiveContext), \
		(HeaderBuffer),           \
		(HeaderBuffer),           \
		(HeaderBufferSize),       \
		(LookaheadBuffer),        \
		(LookaheadBufferSize),    \
		(PacketSize));            \
}

/*
 * VOID
 * NdisMTrIndicateReceiveComplete(
 *   IN NDIS_HANDLE  MiniportAdapterHandle);
 */
#define NdisMTrIndicateReceiveComplete(MiniportAdapterHandle) \
{                                                             \
	(*((PNDIS_MINIPORT_BLOCK)(MiniportAdapterHandle))->TrRxCompleteHandler)( \
    ((PNDIS_MINIPORT_BLOCK)MiniportAdapterHandle)->TrDB);    \
}

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisMWriteLogData(
  _In_ NDIS_HANDLE LogHandle,
  _In_reads_bytes_(LogBufferSize) PVOID LogBuffer,
  _In_ UINT LogBufferSize);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMQueryAdapterResources(
  _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE WrapperConfigurationContext,
  _Out_ PNDIS_RESOURCE_LIST ResourceList,
  _Inout_ PUINT BufferSize);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisTerminateWrapper(
  _In_ NDIS_HANDLE NdisWrapperHandle,
  _In_opt_ PVOID SystemSpecific);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisMUnmapIoSpace(
  _In_ NDIS_HANDLE MiniportAdapterHandle,
  _In_reads_bytes_(Length) PVOID VirtualAddress,
  _In_ UINT Length);

/* Event functions */

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisInitializeEvent(
  _Out_ PNDIS_EVENT Event);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisSetEvent(
  _In_ PNDIS_EVENT Event);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisResetEvent(
  _In_ PNDIS_EVENT Event);

_When_(MsToWait != 0, _Check_return_)
_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
BOOLEAN
NTAPI
NdisWaitEvent(
  _In_ PNDIS_EVENT Event,
  _In_ UINT MsToWait);

/* NDIS intermediate miniport structures */

typedef VOID (NTAPI *W_MINIPORT_CALLBACK)(
  IN NDIS_HANDLE  MiniportAdapterContext,
  IN PVOID  CallbackContext);

/* Routines for intermediate miniport drivers */

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisIMDeInitializeDeviceInstance(
  _In_ NDIS_HANDLE NdisMiniportHandle);

/*
 * NDIS_STATUS
 * NdisIMInitializeDeviceInstance(
 *   IN NDIS_HANDLE  DriverHandle,
 *   IN PNDIS_STRING  DeviceInstance);
 */
#define NdisIMInitializeDeviceInstance(DriverHandle, DeviceInstance) \
  NdisIMInitializeDeviceInstanceEx(DriverHandle, DeviceInstance, NULL)

/* Functions obsoleted by NDIS 5.0 */

NDISAPI
VOID
NTAPI
NdisFreeDmaChannel(
  IN PNDIS_HANDLE  NdisDmaHandle);

NDISAPI
VOID
NTAPI
NdisSetupDmaTransfer(
  OUT PNDIS_STATUS  Status,
  IN PNDIS_HANDLE  NdisDmaHandle,
  IN PNDIS_BUFFER  Buffer,
  IN ULONG  Offset,
  IN ULONG  Length,
  IN BOOLEAN  WriteToDevice);

/*
NDISAPI
NTSTATUS
NTAPI
NdisUpcaseUnicodeString(
  OUT PUNICODE_STRING DestinationString,
  IN PUNICODE_STRING SourceString);
*/
#define NdisUpcaseUnicodeString(_d, _s) RtlUpcaseUnicodeString(_d, _s, FALSE)


/* Routines for NDIS protocol drivers */

#if NDIS_LEGACY_PROTOCOL

NDISAPI
NDIS_STATUS
NTAPI
NdisIMRegisterLayeredMiniport(
  _In_ NDIS_HANDLE NdisWrapperHandle,
  _In_ PNDIS_MINIPORT_CHARACTERISTICS MiniportCharacteristics,
  _In_ UINT CharacteristicsLength,
  _Out_ PNDIS_HANDLE DriverHandle);

NDISAPI
VOID
NTAPI
NdisTransferData(
  OUT PNDIS_STATUS Status,
  IN NDIS_HANDLE NdisBindingHandle,
  IN NDIS_HANDLE MacReceiveContext,
  IN UINT ByteOffset,
  IN UINT BytesToTransfer,
  IN OUT PNDIS_PACKET Packet,
  OUT PUINT BytesTransferred);

NDISAPI
VOID
NTAPI
NdisSend(
  OUT PNDIS_STATUS Status,
  IN NDIS_HANDLE NdisBindingHandle,
  IN PNDIS_PACKET Packet);

NDISAPI
VOID
NTAPI
NdisSendPackets(
  IN NDIS_HANDLE NdisBindingHandle,
  IN PPNDIS_PACKET PacketArray,
  IN UINT NumberOfPackets);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisRequest(
  _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ PNDIS_REQUEST NdisRequest);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisReset(
  _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisBindingHandle);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisDeregisterProtocol(
  _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisProtocolHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisOpenAdapter(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_STATUS OpenErrorStatus,
  _Out_ PNDIS_HANDLE NdisBindingHandle,
  _Out_ PUINT SelectedMediumIndex,
  _In_reads_(MediumArraySize) PNDIS_MEDIUM MediumArray,
  _In_ UINT MediumArraySize,
  _In_ NDIS_HANDLE NdisProtocolHandle,
  _In_ NDIS_HANDLE ProtocolBindingContext,
  _In_ PNDIS_STRING AdapterName,
  _In_ UINT OpenOptions,
  _In_opt_ PSTRING AddressingInformation);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisCloseAdapter(
  _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisBindingHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisCompleteBindAdapter(
  _In_ NDIS_HANDLE BindAdapterContext,
  _In_ NDIS_STATUS Status,
  _In_ NDIS_STATUS OpenStatus);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisCompleteUnbindAdapter(
  _In_ NDIS_HANDLE UnbindAdapterContext,
  _In_ NDIS_STATUS Status);

NDISAPI
VOID
NTAPI
NdisSetProtocolFilter(
  _At_(*Status, _Must_inspect_result_) _Out_ PNDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ RECEIVE_HANDLER ReceiveHandler,
  _In_ RECEIVE_PACKET_HANDLER ReceivePacketHandler,
  _In_ NDIS_MEDIUM Medium,
  _In_ UINT Offset,
  _In_ UINT Size,
  _In_ PUCHAR Pattern);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisGetDriverHandle(
  _In_ PNDIS_HANDLE NdisBindingHandle,
  _Out_ PNDIS_HANDLE NdisDriverHandle);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisOpenProtocolConfiguration(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE ConfigurationHandle,
  _In_ PNDIS_STRING ProtocolSection);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisCompletePnPEvent(
  _In_ NDIS_STATUS Status,
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ PNET_PNP_EVENT NetPnPEvent);

/*
 * VOID
 * NdisSetSendFlags(
 *   IN PNDIS_PACKET Packet,
 *   IN UINT Flags);
 */
#define NdisSetSendFlags(_Packet,_Flags)(_Packet)->Private.Flags = (_Flags)

#define NdisQuerySendFlags(_Packet,_Flags) *(_Flags) = (_Packet)->Private.Flags

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisReturnPackets(
  _In_reads_(NumberOfPackets) PNDIS_PACKET *PacketsToReturn,
  _In_ UINT NumberOfPackets);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
PNDIS_PACKET
NTAPI
NdisGetReceivedPacket(
  _In_ PNDIS_HANDLE NdisBindingHandle,
  _In_ PNDIS_HANDLE MacContext);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
VOID
NTAPI
NdisCancelSendPackets(
  _In_ NDIS_HANDLE NdisBindingHandle,
  _In_ _Points_to_data_ PVOID CancelId);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
NDIS_STATUS
NTAPI
NdisQueryPendingIOCount(
  _In_ _Points_to_data_ PVOID NdisBindingHandle,
  _Out_ PULONG IoCount);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisRegisterProtocol(
  _Out_ PNDIS_STATUS Status,
  _Out_ PNDIS_HANDLE NdisProtocolHandle,
  _In_ PNDIS_PROTOCOL_CHARACTERISTICS ProtocolCharacteristics,
  _In_ UINT CharacteristicsLength);

#endif /* NDIS_LEGACY_PROTOCOL */

_IRQL_requires_max_(DISPATCH_LEVEL)
NDISAPI
UCHAR
NTAPI
NdisGeneratePartialCancelId(VOID);

_IRQL_requires_(PASSIVE_LEVEL)
NDISAPI
VOID
NTAPI
NdisReEnumerateProtocolBindings(
  _In_ NDIS_HANDLE NdisProtocolHandle);

NDISAPI
VOID
NTAPI
NdisRegisterTdiCallBack(
  _In_ TDI_REGISTER_CALLBACK RegisterCallback,
  _In_ TDI_PNP_HANDLER PnPHandler);

NDISAPI
VOID
NTAPI
NdisDeregisterTdiCallBack(VOID);

/* Obsoleted in Windows XP */

/* Prototypes for NDIS_MAC_CHARACTERISTICS */

typedef NDIS_STATUS (*OPEN_ADAPTER_HANDLER)(
  OUT PNDIS_STATUS  OpenErrorStatus,
  OUT NDIS_HANDLE  *MacBindingHandle,
  OUT PUINT  SelectedMediumIndex,
  IN PNDIS_MEDIUM  MediumArray,
  IN UINT  MediumArraySize,
  IN NDIS_HANDLE  NdisBindingContext,
  IN NDIS_HANDLE  MacAdapterContext,
  IN UINT  OpenOptions,
  IN PSTRING  AddressingInformation  OPTIONAL);

typedef NDIS_STATUS (NTAPI *CLOSE_ADAPTER_HANDLER)(
  IN NDIS_HANDLE  MacBindingHandle);

typedef NDIS_STATUS (NTAPI *WAN_TRANSFER_DATA_HANDLER)(
  VOID);

typedef NDIS_STATUS (NTAPI *QUERY_GLOBAL_STATISTICS_HANDLER)(
  IN NDIS_HANDLE  MacAdapterContext,
  IN PNDIS_REQUEST  NdisRequest);

typedef VOID (NTAPI *UNLOAD_MAC_HANDLER)(
  IN NDIS_HANDLE  MacMacContext);

typedef NDIS_STATUS (NTAPI *ADD_ADAPTER_HANDLER)(
  IN NDIS_HANDLE  MacMacContext,
  IN NDIS_HANDLE  WrapperConfigurationContext,
  IN PNDIS_STRING  AdapterName);

typedef VOID (*REMOVE_ADAPTER_HANDLER)(
  IN NDIS_HANDLE  MacAdapterContext);

typedef struct _NDIS_MAC_CHARACTERISTICS {
  UCHAR  MajorNdisVersion;
  UCHAR  MinorNdisVersion;
  USHORT  Filler;
  UINT  Reserved;
  OPEN_ADAPTER_HANDLER  OpenAdapterHandler;
  CLOSE_ADAPTER_HANDLER  CloseAdapterHandler;
  SEND_HANDLER  SendHandler;
  TRANSFER_DATA_HANDLER  TransferDataHandler;
  RESET_HANDLER  ResetHandler;
  REQUEST_HANDLER  RequestHandler;
  QUERY_GLOBAL_STATISTICS_HANDLER  QueryGlobalStatisticsHandler;
  UNLOAD_MAC_HANDLER  UnloadMacHandler;
  ADD_ADAPTER_HANDLER  AddAdapterHandler;
  REMOVE_ADAPTER_HANDLER  RemoveAdapterHandler;
  NDIS_STRING  Name;
} NDIS_MAC_CHARACTERISTICS, *PNDIS_MAC_CHARACTERISTICS;

typedef	NDIS_MAC_CHARACTERISTICS        NDIS_WAN_MAC_CHARACTERISTICS;
typedef	NDIS_WAN_MAC_CHARACTERISTICS    *PNDIS_WAN_MAC_CHARACTERISTICS;

#ifdef __cplusplus
}
#endif

#endif /* _NDIS_ */

/* EOF */
