//
// Created by Nikita on 12.05.2019.
//

#include <iostream>
#include "StateHandler.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

STATES statesMatrix[STATES_COUNT] = {STATES::INACTIVE, STATES::INACTIVE};
ACTIONS actionsMatrix[SIGNALS_COUNT][STATES_COUNT];

HANDLE imgHandle;
HANDLE strHandle;

HWND *statusBarPtr;
HWND *parentPtr;

char *imgPaths[] = {
    "D:\\projects\\ui\\lab7\\img\\0001.bmp",
    "D:\\projects\\ui\\lab7\\img\\0002.bmp",
    "D:\\projects\\ui\\lab7\\img\\0003.bmp",
    "D:\\projects\\ui\\lab7\\img\\0004.bmp",
    "D:\\projects\\ui\\lab7\\img\\0005.bmp",
    "D:\\projects\\ui\\lab7\\img\\0006.bmp",
    "D:\\projects\\ui\\lab7\\img\\0007.bmp",
    "D:\\projects\\ui\\lab7\\img\\0008.bmp",
    "D:\\projects\\ui\\lab7\\img\\0009.bmp",
    "D:\\projects\\ui\\lab7\\img\\0010.bmp",
    "D:\\projects\\ui\\lab7\\img\\0011.bmp",
    "D:\\projects\\ui\\lab7\\img\\0012.bmp",
    "D:\\projects\\ui\\lab7\\img\\0013.bmp",
    "D:\\projects\\ui\\lab7\\img\\0014.bmp",
    "D:\\projects\\ui\\lab7\\img\\0015.bmp",
    "D:\\projects\\ui\\lab7\\img\\0016.bmp",
    "D:\\projects\\ui\\lab7\\img\\0017.bmp",
    "D:\\projects\\ui\\lab7\\img\\0018.bmp",
    "D:\\projects\\ui\\lab7\\img\\0019.bmp",
    "D:\\projects\\ui\\lab7\\img\\0020.bmp",
    "D:\\projects\\ui\\lab7\\img\\0021.bmp",
    "D:\\projects\\ui\\lab7\\img\\0022.bmp",
    "D:\\projects\\ui\\lab7\\img\\0023.bmp",
    "D:\\projects\\ui\\lab7\\img\\0024.bmp",
    "D:\\projects\\ui\\lab7\\img\\0025.bmp",
    "D:\\projects\\ui\\lab7\\img\\0026.bmp",
    "D:\\projects\\ui\\lab7\\img\\0027.bmp",
    "D:\\projects\\ui\\lab7\\img\\0028.bmp",
    "D:\\projects\\ui\\lab7\\img\\0029.bmp",
    "D:\\projects\\ui\\lab7\\img\\0030.bmp",
};

int currentImage = 0;
DWORD runImg() {
    while (currentImage < 30) {
        HDC dc;
        HDC dcCompat;
        HANDLE hBitmap;
        HANDLE hOldBitmap;
        RECT rect;
        BITMAP bitmap;

        dc = GetDC(*parentPtr);
        hBitmap = LoadImage(
            NULL,
            imgPaths[currentImage],
            IMAGE_BITMAP,
            0, 0,
            LR_LOADFROMFILE
        );

        if (hBitmap == NULL) {
            std::cout << "Image not found" << std::endl;
            return -1;
        }

        GetObject(hBitmap, sizeof(BITMAP), &bitmap);
        dcCompat = CreateCompatibleDC(dc);
        hOldBitmap = SelectObject(dcCompat, hBitmap);
        GetWindowRect(*parentPtr, &rect);
        StretchBlt(
            dc,
            0, 50,
            256, 256,
            dcCompat,
            0, 0,
            256, 256,
            SRCCOPY
        );
        SelectObject(dcCompat, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(dcCompat);
        ReleaseDC(*parentPtr, dc);
        currentImage++;

        Sleep((DWORD) 33);

        if (currentImage == 29) {
            currentImage = 0;
        }
    }
}

std::string statusBarText = "Status";
DWORD WINAPI runStr() {
    while (true) {
        if (statusBarText.size() > 300) {
            statusBarText = "Status";
        }
        statusBarText.insert(0, " ");

        SendMessage(
            *statusBarPtr,
            SB_SETTEXT,
            0,
            (LPARAM) statusBarText.c_str()
        );
        Sleep((DWORD) 500);
    }
}

void switchState(SIGNALS signal) {
    STATES state = statesMatrix[signal];
    ACTIONS action = actionsMatrix[signal][state];

    switch (action) {
        case ACTIONS::RUN_IMG: {
            imgHandle = CreateThread(
                NULL,
                0,
                (LPTHREAD_START_ROUTINE)runImg,
                NULL,
                0,
                NULL
            );
            statesMatrix[signal] = STATES::ACTIVE;

            return;
        }

        case ACTIONS::STOP_IMG: {
            TerminateThread(imgHandle, (DWORD) 0);
            statesMatrix[signal] = STATES::INACTIVE;

            return;
        }

        case ACTIONS::RUN_STR: {
            strHandle = CreateThread(
                NULL,
                0,
                (LPTHREAD_START_ROUTINE)runStr,
                NULL,
                0,
                NULL
            );
            statesMatrix[signal] = STATES::ACTIVE;

            return;
        }

        case ACTIONS::STOP_STR: {
            TerminateThread(strHandle, (DWORD) 0);
            statesMatrix[signal] = STATES::INACTIVE;

            return;
        }

        default: {}
    }
}

void initActionsMatrix() {
    actionsMatrix[SIGNALS::CLICK_IMG_BTN][STATES::INACTIVE] = ACTIONS::RUN_IMG;
    actionsMatrix[SIGNALS::CLICK_IMG_BTN][STATES::ACTIVE] = ACTIONS::STOP_IMG;
    actionsMatrix[SIGNALS::CLICK_STR_BTN][STATES::INACTIVE] = ACTIONS::RUN_STR;
    actionsMatrix[SIGNALS::CLICK_STR_BTN][STATES::ACTIVE] = ACTIONS::STOP_STR;
}

void initStateHandler(
    HWND *parentPointer,
    HWND *statusBarPointer
) {

    initActionsMatrix();

    parentPtr = parentPointer;
    statusBarPtr = statusBarPointer;
}
#pragma clang diagnostic pop