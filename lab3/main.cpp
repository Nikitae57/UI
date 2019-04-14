#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>

char szClassName[] = "WindowAppClass";
char buffer[1024];
int textLength = 0;

HINSTANCE hInstance;
HWND child1Hwnd;
HWND child2Hwnd;

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void setText(char ch);
void setText(char* ch);

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

//  std::cout << msg << std::endl;
  switch (msg) {
    case WM_CHAR: {
      switch (wParam) {
        // enter
        case 0x0D: {
          setText("\r\n");
          textLength +=2;
          return 0;
        }

        default: {
          char ch = (TCHAR) wParam;
          setText(ch);

          textLength +=1;
          return 0;
        }
      }
    }

//    case WM_CTLCOLOREDIT: {
//      if ((HWND) lParam == child1Hwnd) {
//        std::cout << wParam << std::endl;
//        std::cout << lParam << std::endl;
//        std::cout << std::to_string(HIWORD(wParam))  << std::endl;
//        std::cout << std::to_string(LOWORD(wParam)) << std::endl;
//        std::cout << std::to_string(HIWORD(lParam)) << std::endl;
//        std::cout << std::to_string(LOWORD(lParam)) << std::endl;
//        GetWindowText(child1Hwnd, buffer, sizeof(buffer));
//        SendMessage(
//            child2Hwnd,
//            WM_SETTEXT,
//            NULL,
//            (LPARAM) buffer
//        );
//
//      } else if ((HWND) lParam == child2Hwnd) {
//        GetWindowText(child2Hwnd, buffer, sizeof(buffer));
//        SendMessage(
//            child1Hwnd,
//            WM_SETTEXT,
//            NULL,
//            (LPARAM) buffer
//        );
//      }
//      return 0;
//    }

    case WM_CREATE: {
      child1Hwnd = CreateWindow(
          TEXT("EDIT"),
          (LPSTR) "",
          WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|WS_EX_NOACTIVATE,
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
          WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|WS_EX_NOACTIVATE,
          400, 10,
          380, 540,
          hwnd,
          NULL,
          hInstance,
          NULL
      );

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

void setText(char ch) {
  char *str = (char*) malloc(2);
  *(str) = ch;
  *(str + 1) = '\0';

  SendMessage(
      child1Hwnd,
      EM_SETSEL,
      (WPARAM) textLength,
      (LPARAM) textLength
  );

  SendMessage(
      child1Hwnd,
      EM_REPLACESEL,
      (WPARAM)FALSE,
      (LPARAM) str
  );

  SendMessage(
      child2Hwnd,
      EM_SETSEL,
      (WPARAM) textLength,
      (LPARAM) textLength
  );

  SendMessage(
      child2Hwnd,
      EM_REPLACESEL,
      (WPARAM)FALSE,
      (LPARAM) str
  );

  free(str);
}

void setText(char* ch) {

  SendMessage(
      child1Hwnd,
      EM_SETSEL,
      (WPARAM) textLength,
      (LPARAM) textLength
  );

  SendMessage(
      child1Hwnd,
      EM_REPLACESEL,
      (WPARAM)FALSE,
      (LPARAM) ch
  );

  SendMessage(
      child2Hwnd,
      EM_SETSEL,
      (WPARAM) textLength,
      (LPARAM) textLength
  );

  SendMessage(
      child2Hwnd,
      EM_REPLACESEL,
      (WPARAM)FALSE,
      (LPARAM) ch
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

  while (GetMessage(&msg, 0, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}