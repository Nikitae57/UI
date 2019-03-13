#include "main.h"

static LPCSTR szTitle = "Title";
static LPCSTR szMainWndClass = "MainClass";
static LPCSTR szChildWndClass = "ChildClass";

static const long long ID_NEWCHILD = 1;
static const long long ID_EXIT = 2;


int APIENTRY
WinMain(HINSTANCE
        hInstance,
        HINSTANCE hPrevInstance,
        LPSTR
        lpCmdLine,
        int nCmdShow
) {
//LoadString(hInstance, IDS_TITLE, szTitle, MAX_LOADSTRING
//);
//LoadString(hInstance, IDS_MAINWNDCLASS, szMainWndClass, MAX_LOADSTRING
//);
//LoadString(hInstance, IDS_CHILDWNDCLASS, szChildWndClass, MAX_LOADSTRING
//);

  if (!MainWndRegisterClass(hInstance)
      || !ChildWndRegisterClass(hInstance))
    return false;

  if (!
      InitInstance(hInstance, nCmdShow
      ))
    return false;

  MSG msg;

  while (
      GetMessage(&msg, NULL,
                 0, 0)) {
    if (!
        TranslateMDISysAccel(hMDIClientWnd, &msg
        )) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int) msg.
      wParam;
}

//------------------------------------------------------------------------------

ATOM MainWndRegisterClass(HINSTANCE
                          hInstance) {
  WNDCLASSEX wcex;

  memset(&wcex,
         0, sizeof(WNDCLASSEX));

  wcex.
      cbSize = sizeof(WNDCLASSEX);
  wcex.
      style = CS_HREDRAW | CS_VREDRAW;
  wcex.
      lpfnWndProc = (WNDPROC) MainWndProc;
  wcex.
      hInstance = hInstance;
  wcex.
      hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.
      hbrBackground = (HBRUSH) (COLOR_WINDOW);
  wcex.
      lpszMenuName = (LPCTSTR) "Menu";
  wcex.
      lpszClassName = (LPCTSTR) szMainWndClass;

  return
      RegisterClassEx(&wcex);
}

//------------------------------------------------------------------------------

ATOM ChildWndRegisterClass(HINSTANCE
                           hInstance) {
  WNDCLASSEX wcex;

  memset(&wcex,
         0, sizeof(WNDCLASSEX));

  wcex.
      cbSize = sizeof(WNDCLASSEX);
  wcex.
      lpfnWndProc = (WNDPROC) ChildWndProc;
  wcex.
      hInstance = hInstance;
  wcex.
      hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.
      hbrBackground = (HBRUSH) (COLOR_WINDOW);
  wcex.
      lpszClassName = (LPCTSTR) szChildWndClass;

  return
      RegisterClassEx(&wcex);
}

//------------------------------------------------------------------------------

BOOL InitInstance(HINSTANCE
                  hInstance,
                  int nCmdShow
) {
  hInst = hInstance;

  hMainWnd = CreateWindow(szMainWndClass, szTitle, WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL);

  if (!hMainWnd)
    return false;

  ShowWindow(hMainWnd, nCmdShow
  );
  UpdateWindow(hMainWnd);

  return true;
}

//------------------------------------------------------------------------------

LRESULT CALLBACK
MainWndProc(HWND
            hWnd,
            UINT message, WPARAM
            wParam,
            LPARAM lParam
) {
  switch (message) {
    case WM_CREATE: {
      CLIENTCREATESTRUCT ccs;
      memset(&ccs,
             0, sizeof(CLIENTCREATESTRUCT));

//////////////////////////////////////////////////
// если нет меню следующие 2 строки можно пропустить //
      ccs.
          hWindowMenu = GetSubMenu(GetMenu(hWnd), 1);
      ccs.
          idFirstChild = 100;
//////////////////////////////////////////////////

      hMDIClientWnd = CreateWindow("MDICLIENT", (LPCTSTR) NULL,
                                   WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
                                   0, 0, 0, 0, hWnd, NULL, hInst, (LPSTR) &ccs);

      if (!hMDIClientWnd)
        return false;

      ShowWindow(hMDIClientWnd, SW_SHOW
      );
    }
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_NEWCHILD: {
          HWND hChild = CreateMDIWindow(
              szChildWndClass,
              (LPCTSTR) "Child Window",
              MDIS_ALLCHILDSTYLES,
              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
              hMDIClientWnd,
              hInst,
              NULL
          );

          if (!hChild)
            return 0;

          ShowWindow(hChild, SW_SHOW);
        }
        break;

        case ID_EXIT: SendMessage(hWnd, WM_CLOSE,0, 0);
          break;

        default:
          return
              DefFrameProc(hWnd, hMDIClientWnd, message, wParam, lParam
              );
      }
      break;

    case WM_DESTROY:PostQuitMessage(0);
      break;

    default:
      return
          DefFrameProc(hWnd, hMDIClientWnd, message, wParam, lParam
          );
  }

  return 0;
}

//------------------------------------------------------------------------------

LRESULT CALLBACK
ChildWndProc(HWND
             hWnd,
             UINT message, WPARAM
             wParam,
             LPARAM lParam
) {
  switch (message) {
    default:
      return
          DefMDIChildProc(hWnd, message, wParam, lParam
          );
  }

  return 0;
}