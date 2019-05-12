//
// Created by Nikita on 12.05.2019.
//

#ifndef LAB7_STATEHANDLER_H
#define LAB7_STATEHANDLER_H

#include "Enums.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <commctrl.h>

#define IMG_COUNT = 30

void switchState(SIGNALS signal);
void initStateHandler(
    HWND *parentPointer,
    HWND *statusBarPointer
);

#endif //LAB7_STATEHANDLER_H
