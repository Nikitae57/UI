#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define MAX_LOADSTRING 100
#define MAX_TEXT 1024

HINSTANCE hInst;
HWND hMainWnd, hMDIClientWnd;
//TCHAR szTitle[MAX_LOADSTRING];
//TCHAR szMainWndClass[MAX_LOADSTRING];
//TCHAR szChildWndClass[MAX_LOADSTRING];

ATOM MainWndRegisterClass(HINSTANCE hInstance);
ATOM ChildWndRegisterClass(HINSTANCE hInstance);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);