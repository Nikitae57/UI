#if !defined(_SIGMA_MAIN_H_INCLUDED_)
#define _SIGMA_MAIN_H_INCLUDED_

// Mainframe window procedure declaration
LRESULT CALLBACK SigmaFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool CreateStatusBar();
void AddSysTrayIcon();
void RemoveSysTrayIcon();
bool SetupMailslot();
DWORD MonitorServiceThread(LPVOID lpParam);

#endif