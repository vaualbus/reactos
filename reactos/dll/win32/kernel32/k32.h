/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS System Libraries
 * FILE:            lib/kernel32/k32.h
 * PURPOSE:         Win32 Kernel Libary Header
 * PROGRAMMER:      Alex Ionescu (alex@relsoft.net)
 */

#ifndef __K32_H
#define __K32_H

/* INCLUDES ******************************************************************/

/* PSDK/NDK Headers */
#define WIN32_NO_STATUS
#include <stdarg.h>
#include <stdio.h>
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winreg.h>
#include <wincon.h>
#include <winuser.h>
#include <tlhelp32.h>

/* Redefine NTDDI_VERSION to 2K3 SP1 to get correct NDK definitions */
#undef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WS03SP1

#include <ndk/cmfuncs.h>
#include <ndk/dbgkfuncs.h>
#include <ndk/exfuncs.h>
#include <ndk/iofuncs.h>
#include <ndk/kdtypes.h>
#include <ndk/kefuncs.h>
#include <ndk/ldrfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/pofuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/setypes.h>
#include <ndk/umfuncs.h>

/* CSRSS Header */
#include <csr/csr.h>
#include <win/base.h>
#include <win/basemsg.h>
#include <win/console.h>
#include <win/conmsg.h>
#include <win/winmsg.h>

/* DDK Driver Headers */
#include <ntddbeep.h>
#include <mountmgr.h>

/* Internal Kernel32 Header */
#include "include/kernel32.h"

/* PSEH for SEH Support */
#include <pseh/pseh2.h>

/* Base Macros */
#include "include/base_x.h"

/* Console API Client Definitions */
#include "include/console.h"

/* Virtual DOS Machines (VDM) Support Definitions */
#include "include/vdm.h"

#endif
