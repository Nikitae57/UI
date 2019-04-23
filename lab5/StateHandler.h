#ifndef LAB5_STATEHANDLER_H
#define LAB5_STATEHANDLER_H

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <psapi.h>
#include <unistd.h>
#include <windowsx.h>
#include "DbHandler.h"
#include "enums.h"

char buffer[2048];
bool attrBeenSelected = false;

int selectedColumnsNumber = 0;
char **selectedColumns;

char **tableColumns;
int tableColumnsNumber = 0;

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

void switchState(STATE state);
void initStateMatrix();
void initTransitionMatrix();
void initContextMatrix();

#endif //LAB5_STATEHANDLER_H
