#pragma once

#define IDC_STATIC -1

#define IDI_START 101

#define IDS_START             101
#define IDS_PROPERTIES        102
#define IDS_OPEN_ALL_USERS    103
#define IDS_EXPLORE_ALL_USERS 104
#define IDS_STARTUP_ERROR     105

#define IDC_STARTBTN 304

#define IDM_TRAYWND   101
#define IDM_STARTMENU 102

#define IDB_STARTMENU 101

#define IDA_TASKBAR 251

#define IDMA_START 305
#define IDMA_CYCLE_FOCUS 41008
#define IDMA_SEARCH 41093
#define IDMA_RESTORE_OPEN 416
#define IDMA_MINIMIZE_ALL 419

#define ID_SHELL_CMD_FIRST              0xF
#define ID_SHELL_CMD_LAST               0x7FEF
#define ID_SHELL_CMD_PROPERTIES         (ID_SHELL_CMD_LAST + 1)
#define ID_SHELL_CMD_OPEN_ALL_USERS     (ID_SHELL_CMD_LAST + 2)
#define ID_SHELL_CMD_EXPLORE_ALL_USERS  (ID_SHELL_CMD_LAST + 3)

#define ID_LOCKTASKBAR (ID_SHELL_CMD_LAST + 4)

#define ID_SHELL_CMD_OPEN_TASKMGR (ID_SHELL_CMD_LAST + 5)
#define ID_SHELL_CMD_UNDO_ACTION  (ID_SHELL_CMD_LAST + 6)
#define ID_SHELL_CMD_SHOW_DESKTOP (ID_SHELL_CMD_LAST + 7)
#define ID_SHELL_CMD_TILE_WND_V   (ID_SHELL_CMD_LAST + 8)
#define ID_SHELL_CMD_TILE_WND_H   (ID_SHELL_CMD_LAST + 9)
#define ID_SHELL_CMD_CASCADE_WND  (ID_SHELL_CMD_LAST + 10)
#define ID_SHELL_CMD_CUST_NOTIF   (ID_SHELL_CMD_LAST + 11)
#define ID_SHELL_CMD_ADJUST_DAT   (ID_SHELL_CMD_LAST + 12)

/* NOTE: The following constants may *NOT* be changed because
         they're hardcoded and need to be the exact values
         in order to get the start menu to work! */
#define IDM_PROGRAMS                504
#define IDM_FAVORITES               507
#define IDM_DOCUMENTS               501
#define IDM_SETTINGS                508
#define IDM_CONTROLPANEL            505
#define IDM_SECURITY                5001
#define IDM_NETWORKCONNECTIONS      557
#define IDM_PRINTERSANDFAXES        510
#define IDM_TASKBARANDSTARTMENU     413
#define IDM_SEARCH                  520
#define IDM_HELPANDSUPPORT          503
#define IDM_RUN                     401
#define IDM_SYNCHRONIZE             553
#define IDM_LOGOFF                  402
#define IDM_DISCONNECT              5000
#define IDM_UNDOCKCOMPUTER          410
#define IDM_SHUTDOWN                506
#define IDM_LASTSTARTMENU_SEPARATOR 450

/* Taskbar resources */
#define IDD_TASKBARPAGE      2000
#define IDC_TASKBAR_BITMAP   2001
#define IDD_STARTMENUPAGE    2001
#define IDD_NOTIFICATIONPAGE 2002
#define IDD_TOOLBARSPAGE     2003

/* Taskbar properties, taskbar */
#define IDD_TASKBARPROP_TASKBAR       2000
#define IDC_TASKBARPROP_TASKBARBITMAP 2001
#define IDC_TASKBARPROP_LOCK          2002
#define IDC_TASKBARPROP_HIDE          2003
#define IDC_TASKBARPROP_ONTOP         2004
#define IDC_TASKBARPROP_GROUP         2005
#define IDC_TASKBARPROP_SHOWQL        2006
#define IDC_TASKBARPROP_WNDPREV       2007

#define IDB_TASKBARPROP_AUTOHIDE            2050
#define IDB_TASKBARPROP_LOCK_GROUP_NOQL     2051
#define IDB_TASKBARPROP_LOCK_GROUP_QL       2052
#define IDB_TASKBARPROP_LOCK_NOGROUP_NOQL   2053
#define IDB_TASKBARPROP_LOCK_NOGROUP_QL     2054
#define IDB_TASKBARPROP_NOLOCK_GROUP_NOQL   2055
#define IDB_TASKBARPROP_NOLOCK_NOGROUP_NOQL 2056
#define IDB_TASKBARPROP_NOLOCK_NOGROUP_QL   2057
#define IDB_TASKBARPROP_NOLOCK_GROUP_QL     2058
#define IDC_TASKBARPROP_SECONDS             2059

/* Taskbar properties, startmenu*/
#define IDD_TASKBARPROP_STARTMENU            2100
#define IDC_TASKBARPROP_STARTMENU            2101
#define IDC_TASKBARPROP_STARTMENUCUST        2102
#define IDC_TASKBARPROP_STARTMENUCLASSIC     2103
#define IDC_TASKBARPROP_STARTMENUCLASSICCUST 2104
#define IDC_TASKBARPROP_RECENTFILES          2105
#define IDC_TASKBARPROP_RECENTFOLDERS        2106

/* Taskbar properties, notification */
#define IDD_TASKBARPROP_NOTIFICATION       2200
#define IDC_TASKBARPROP_NOTIFICATIONBITMAP 2201
#define IDC_TASKBARPROP_HIDEICONS          2202
#define IDC_TASKBARPROP_ICONCUST           2203
#define IDC_TASKBARPROP_CLOCK              2204
#define IDC_TASKBARPROP_VOLUME             2205
#define IDC_TASKBARPROP_NETWORK            2206
#define IDC_TASKBARPROP_POWER              2207
#define IDD_TASKBARPROP_ADVANCED           2208

/* Taskbar properties, toolbars */
#define IDD_TASKBARPROP_TOOLBARS     2300
#define IDC_TASKBARPROP_DESKBANDLIST 2301

#define IDS_TASKBAR_STARTMENU_PROP_CAPTION 2200
#define IDS_HELP_COMMAND 732
