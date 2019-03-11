#include <iostream>
#include <Windows.h>
#include <compressapi.h>

HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";

LRESULT CALLBACK WndProc(
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

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
) {

  MSG msg;
  HWND hwnd;
  hInstance = hInst;
  COMPRESSOR_HANDLE Compressor    = NULL;
  PBYTE CompressedBuffer          = NULL;
  PBYTE InputBuffer               = NULL;
  HANDLE InputFile                = INVALID_HANDLE_VALUE;
  HANDLE CompressedFile           = INVALID_HANDLE_VALUE;
  BOOL DeleteTargetFile           = TRUE;
  BOOL Success;
  SIZE_T CompressedDataSize, CompressedBufferSize;
  DWORD InputFileSize, ByteRead, ByteWritten;
  LARGE_INTEGER FileSize;
  ULONGLONG StartTime, EndTime;
  double TimeDuration;

  if (!RegClass(WndProc, szClassName, COLOR_WINDOW)) {
    return false;
  }

  hwnd = CreateWindow(
      szClassName,
      "Compressor",
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