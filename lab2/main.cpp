#include <windows.h>
#include <stdio.h>
#include <malloc.h>

HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";

LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void showErrMsg(HWND hwnd);

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_LBUTTONDOWN: {
      RegClass(NULL, "ID", COLOR_WINDOW);
      bool errNotOccurred = RegClass(NULL, "ID", COLOR_WINDOW);

      if (errNotOccurred) { return 0; }
      showErrMsg(hwnd);

      return 0;
    }

    case WM_RBUTTONDOWN: {
      CreateWindow(
          NULL,
          NULL,
          NULL,
          NULL,
          NULL, NULL, NULL, NULL,
          NULL,
          NULL,
          NULL
      );
      showErrMsg(hwnd);

      return 0;
    }

    case WM_KEYDOWN: {
      HWND hwnd2;
      MessageBox(hwnd2, NULL, NULL, NULL);
      showErrMsg(hwnd);

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

void showErrMsg(HWND hwnd) {
  DWORD error = GetLastError();
  LPVOID msgTextRus;
  LPVOID msgTextEng;
  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR) &msgTextRus,
      0, NULL);

  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error,
      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
      (LPTSTR) &msgTextEng,
      0, NULL);

  char finalMsg[500];
  sprintf_s(finalMsg, "%d\n%s%s", error, msgTextRus, msgTextEng);
  int msgBoxId = MessageBox(
      hwnd,
      finalMsg,
      NULL,
      MB_ICONEXCLAMATION | MB_OK
  );
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

  if (!RegClass(WndProc, szClassName, COLOR_WINDOW)) {
    return false;
  }

  hwnd = CreateWindow(
      szClassName,
      "lab1",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      0,
      0,
      hInstance,
      NULL
  );

  if (!hwnd) { return false; }

  while (GetMessage(&msg, 0, 0, 0)) {
    DispatchMessage(&msg);
  }

  return msg.wParam;
}