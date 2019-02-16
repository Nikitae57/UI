#include <windows.h>
#include <stdio.h>

HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";
char szChildClassName[] = "ChildClassName";
long x;
long y;

LRESULT CALLBACK WndProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
);

LRESULT CALLBACK ChildWndProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
);

int GetTextSize (LPSTR a0)
{
    for (int iLoopCounter = 0; ;iLoopCounter++)
    {
        if (a0 [iLoopCounter] == '\0')
            return iLoopCounter;
    }
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

LRESULT CALLBACK WndProc(HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
        ) {

    static short cxClient, cyClient,
        left, top, Width, Height;
    static HWND childHwnd;

    switch (msg) {
        case WM_MOVE: {
            left = LOWORD(lParam);
            top = HIWORD(lParam);

            if (IsWindow(childHwnd)) {
                MoveWindow(
                        childHwnd,
                        left + cxClient - Width,
                        top + cyClient - Height,
                        Width,
                        Height,
                        true
                );
            }

            return 0;
        }

        case WM_SIZE: {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            Width = cxClient / 10;
            Height = cyClient / 10;

            if (IsWindow(childHwnd)) {
                MoveWindow(
                        childHwnd,
                        left + cxClient - Width,
                        top + cyClient - Height,
                        Width,
                        Height,
                        true
                );
            }

            return 0;
        }

        case WM_LBUTTONDOWN: {
            x = LOWORD(lParam);
            y = HIWORD(lParam);

            DestroyWindow(childHwnd);
            childHwnd = CreateWindow(
                szChildClassName,
               "Child window",
               WS_VISIBLE | WS_CHILD,
               x,
               y,
               Width,
               Height,
               hwnd,
               0,
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

LRESULT CALLBACK ChildWndProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
) {
    HDC hDC;
    PAINTSTRUCT PaintStruct;
    RECT Rect;
    char text[100];
    sprintf_s(text, "x=%ld y=%ld", x, y);

    switch (msg) {
        case WM_PAINT: {
            hDC = BeginPaint(hwnd, &PaintStruct);
            GetClientRect(hwnd, &Rect);

            DrawText(hDC, text, -1, &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(hwnd, &PaintStruct);
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

    if (!RegClass(WndProc, szClassName, COLOR_WINDOW)) {
        return false;
    }

    if (!RegClass(ChildWndProc, szChildClassName, COLOR_BTNFACE)) {
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

    if (!hwnd) { return  false; }

    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }

    return msg.wParam;
}