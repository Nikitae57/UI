#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <string>
#include <commctrl.h>

HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";

HWND parentHwnd;
HWND btn00, btn01, btn02,
    btn10, btn11, btn12,
    btn20, btn21, btn22;

LRESULT CALLBACK
WndProc(
    HWND
    hwnd,
    UINT msg,
    WPARAM
    wParam,
    LPARAM lParam
);

void initUi(HWND hwnd) {
    btn00 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn01 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 10,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn02 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        70, 10,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn10 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 40,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn11 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 40,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn12 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        70, 40,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn20 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 70,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn21 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 70,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );

    btn00 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        70, 70,
        20, 20,
        hwnd, NULL, hInstance, NULL
    );
}

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

void handleWmCommand(HWND
                     hwnd,
                     UINT msg, WPARAM
                     wParam,
                     LPARAM lParam
) {

}

LRESULT CALLBACK
WndProc(HWND
        hwnd,
        UINT msg, WPARAM
        wParam,
        LPARAM lParam
) {
    switch (msg) {
        case WM_CREATE: {
            initUi(hwnd);
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        case WM_COMMAND: {
            handleWmCommand(hwnd, msg, wParam, lParam);
            return 0;
        }

        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
}

int WINAPI
WinMain(
    HINSTANCE
    hInst,
    HINSTANCE hPrevInstance,
    LPSTR
    lpszCmdLine,
    int nCmdShow
) {

    MSG msg;
    parentHwnd;
    hInstance = hInst;

    if (!RegClass(WndProc, szClassName, COLOR_WINDOW)) {
        return false;
    }

    parentHwnd = CreateWindow(
        szClassName,
        "lab8",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        250,
        150,
        0,
        0,
        hInstance,
        NULL
    );

    if (!parentHwnd) {
        return false;
    }

    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }

    return msg.wParam;
}