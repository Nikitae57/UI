#ifndef LAB5_STATEHANDLER_H
#define LAB5_STATEHANDLER_H

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <windowsx.h>
#include <map>
#include <psapi.h>
#include <unistd.h>
#include "DbHandler.h"
#include "enums.h"

void initStateHandler(
	HWND parent,
    HWND etTableName,
    HWND llTableFields,
    HWND etSelectQuery,
    HWND llSelect,
    HWND btnOk,

    HWND btnNext,
    HWND etComparisonValue,
    HWND llComparisonSigns,
	HMENU hMenu
);

//void switchState(UI_S state);
void initStateMatrix();
void initTransitionMatrix();
void initContextMatrix();
void switchContext(UI_STATES st);

#endif //LAB5_STATEHANDLER_H
