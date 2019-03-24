#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

char szClassName[] = "WindowAppClass";
char childClassname[] = "childClass";
char buffer[1024];

int mainWindowWidth;
int mainWindowHeight;
const int idChild1 = 1001;
const int idChild2 = 1002;

HINSTANCE hInstance;
HWND child1Hwnd;
HWND child2Hwnd;

LRESULT CALLBACK childWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
  switch (msg) {
    case WM_CREATE: {
      CreateWindow(
          TEXT("EDIT"),
          (LPSTR) "",
          WS_BORDER|WS_CHILD|WS_VISIBLE,
          10, 10,
          370, 530,
          hwnd,
          NULL,
          NULL,
          NULL
      );
      std::cout << hwnd << std::endl;

      return  0;
    }

    default: {
      DefWindowProc(hwnd, msg, wParam, lParam);
    }
  }
}

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

bool RegClass(WNDPROC proc, LPCSTR szName, UINT color) {
  WNDCLASS windowClass;
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.lpfnWndProc = proc;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadCursor(NULL, IDI_APPLICATION);
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH) (color + 1);
  windowClass.lpszMenuName = (LPCTSTR) NULL;
  windowClass.lpszClassName = szName;

  return RegisterClass(&windowClass);
}

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
  switch (msg) {
    case WM_CREATE: {
      child1Hwnd = CreateMDI

      child1Hwnd = CreateWindow(
          TEXT("EDIT"),
          (LPSTR) "",
          WS_BORDER|WS_CHILD|WS_VISIBLE |ES_MULTILINE,
          10, 10,
          380, 540,
          hwnd,
          NULL,
          hInstance,
          NULL
      );

      child2Hwnd = CreateWindow(
          TEXT("EDIT"),
          (LPCSTR) "",
          WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE,
          400, 10,
          380, 540,
          hwnd,
          NULL,
          hInstance,
          NULL
      );

      return 0;
    }

    case WM_COMMAND: {
      if (HIWORD(wParam) == EN_CHANGE) {

        if ((HWND) lParam == child1Hwnd) {
          GetWindowText(child1Hwnd, buffer, sizeof(buffer));
          SendMessage(
              child2Hwnd,
              WM_SETTEXT,
              NULL,
              (LPARAM) buffer
          );

        } else if ((HWND) lParam == child2Hwnd) {
          GetWindowText(child2Hwnd, buffer, sizeof(buffer));
          SendMessage(
              child1Hwnd,
              WM_SETTEXT,
              NULL,
              (LPARAM) buffer
          );
        }
      }
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }

    default: {
      return DefWindowProc(hwnd, msg, wParam, lParam);
    }
  }
}

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
) {

  MSG msg;
  HWND hwnd;
  hInstance = hInst;
  mainWindowHeight = GetSystemMetrics(SM_CYSCREEN);
  mainWindowWidth = GetSystemMetrics(SM_CXSCREEN);

  if (!RegClass(mainWindowProc, szClassName, COLOR_WINDOW)) {
    return false;
  }

  hwnd = CreateWindow(
      szClassName,
      "lab3",
      WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      800,
      600,
      0,
      0,
      hInstance,
      NULL
  );

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  if (!hwnd) { return false; }

  child1Hwnd = GetDlgItem(hwnd, idChild1);
  HACCEL hAccel = 0;

  while (GetMessage(&msg, 0, 0, 0) > 0) {
    if (!TranslateMDISysAccel(child1Hwnd, &msg)
        && !TranslateAccelerator(hwnd, hAccel, &msg)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
  }

  return msg.wParam;
}