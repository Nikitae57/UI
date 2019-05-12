#include <stdio.h>
#include <malloc.h>
#include "StateHandler.h"

HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";

HWND btnImg;
HWND btnStr;
HWND statusBar;
HWND parentHwnd;

const int ID_BTN_IMG = 1000;
const int ID_BTN_STR = 1001;
const int ID_STATUS_BAR = 1002;

LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void initUi(HWND hwnd) {
    btnImg = CreateWindow(
        "Button", "Image",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0,
        50, 30,
        hwnd, (HMENU)ID_BTN_IMG, hInstance, NULL
    );

    btnStr = CreateWindow(
        "Button", "String",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        60, 0,
        50, 30,
        hwnd, (HMENU)ID_BTN_STR, hInstance, NULL
    );

    statusBar = CreateStatusWindowA(
        WS_CHILD | WS_VISIBLE,
        "Status",
        hwnd,
        ID_STATUS_BAR
    );

    initStateHandler(&parentHwnd, &statusBar);
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

void handleWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (LOWORD(wParam)) {
        case ID_BTN_STR: {
            switchState(SIGNALS::CLICK_STR_BTN);
            return;
        }

        case ID_BTN_IMG: {
            switchState(SIGNALS::CLICK_IMG_BTN);
            return;
        }

        default: { return; }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
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
        "lab7",
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

    if (!parentHwnd) { return false; }

    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }

    return msg.wParam;
}