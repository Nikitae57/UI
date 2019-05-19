#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <string>
#include <commctrl.h>
#include <iostream>

HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";

HWND parentHwnd;
HWND cell00, cell01, cell02,
    cell10, cell11, cell12,
    cell20, cell21, cell22;
HWND cells[9];

int ID_CELL = 1111;
char buffer[2];

#define FIRST_PLAYER_TURN 0
#define SECOND_PLAYER_TURN 1
int turn = 0;

LRESULT CALLBACK
WndProc(
    HWND
    hwnd,
    UINT msg,
    WPARAM
    wParam,
    LPARAM lParam
);

int winCombinations[8][9] = {
    {1, 1, 1,
     0, 0, 0,
     0, 0, 0},

    {0, 0, 0,
     1, 1, 1,
     0, 0, 0},

    {0, 0, 0,
     0, 0, 0,
     1, 1, 1},

    {1, 0, 0,
     1, 0, 0,
     1, 0, 0},

    {0, 1, 0,
     0, 1, 0,
     0, 1, 0},

    {0, 0, 1,
     0, 0, 1,
     0, 0, 1},

    {1, 0, 0,
     0, 1, 0,
     0, 0, 1},

    {0, 0, 1,
     0, 1, 0,
     1, 0, 0},
};

void initUi(HWND hwnd) {
    cell00 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell01 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 10,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell02 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        70, 10,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell10 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 40,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell11 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 40,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell12 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        70, 40,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell20 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 70,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell21 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 70,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cell22 = CreateWindow(
        "Button", "",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        70, 70,
        20, 20,
        hwnd, (HMENU) ID_CELL, hInstance, NULL
    );

    cells[0] = cell00;
    cells[1] = cell01;
    cells[2] = cell02;
    cells[3] = cell10;
    cells[4] = cell11;
    cells[5] = cell12;
    cells[6] = cell20;
    cells[7] = cell21;
    cells[8] = cell22;
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

void winAchieved() {
    MessageBox(
        parentHwnd,
        "Win!",
        NULL,
        MB_ICONEXCLAMATION | MB_OK
    );
}

void winNotAchieved() {
    switch (turn) {
        case FIRST_PLAYER_TURN: {
            turn = SECOND_PLAYER_TURN;
            return;
        }

        case SECOND_PLAYER_TURN: {
            turn = FIRST_PLAYER_TURN;
            return;
        }
    }
}

bool checkCombo(int combo[]) {
    bool isWin = false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 9; j++) {
            if (winCombinations[i][j] != combo[j]) {
                isWin = false;
                break;
            } else {
                isWin = true;
            }
        }

        if (isWin) {
            break;
        }
    }

    return isWin;
}

void winCheck() {
    char symbol;
    if (turn == FIRST_PLAYER_TURN) {
        symbol = 'O';
    } else {
        symbol = 'X';
    }

    int combination[9];
    for (int i = 0; i < 9; i++) {
        GetWindowText(cells[i], buffer, 2);
        if (buffer[0] == symbol) {
            combination[i] = 1;
        } else {
            combination[i] = 0;
        }

        printf("%d", combination[i]);
        if ((i + 1) % 3 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    bool isWinCombo = checkCombo(combination);
    if (isWinCombo) {
        winAchieved();
    } else {
        winNotAchieved();
    }
}

void cellClicked(LPARAM lparam) {
    GetWindowText((HWND) lparam, buffer, 2);
    bool isCellFree = strlen(buffer) == 0;

    if (isCellFree) {
        switch (turn) {
            case FIRST_PLAYER_TURN: {
                SetWindowText((HWND) lparam, "O");
                winCheck();
                return;
            }

            case SECOND_PLAYER_TURN: {
                SetWindowText((HWND) lparam, "X");
                winCheck();
                return;
            }
        }
    }
}

void handleWmCommand(HWND
                     hwnd,
                     UINT msg, WPARAM
                     wParam,
                     LPARAM lParam
) {
    if (LOWORD(wParam) == ID_CELL) {
        cellClicked(lParam);
    }
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