#if !defined(_SIGMA_SIGMADEFINES_H_INCLUDED_)
#define _SIGMA_SIGMADEFINES_H_INCLUDED_
#include<windows.h>

#define IDM_FIRSTCHILD   50000 // used in structure when creating mdi client area for the main frame 

#define MEDIUM_CHAR_ARRAY_LEN	64 // max length for small to medium array lengths
#define SIGMA_SYSTEMINFO_MUTEX "SigmaSystemInformationMutex"
#define SIGMA_STATUSBAR_PARTS	3 // number of parts for status bar

#define SIGMA_SYSTRAY_ICON_ID	1000

#define WM_SIGMA_SYSTRAY_MSG		WM_USER+1

#define SIGMA_SYSINFO_WINDOW_MENU_POS	2

#endif
