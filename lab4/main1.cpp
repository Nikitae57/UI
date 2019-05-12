#include <stdio.h>
#include "MyDll.h"

#define ID_BTNLINE 1000
#define ID_BTNBOX 1001
#define ID_BTNCIRCLE 1002
#define ID_BTNCLRRED 2000
#define ID_BTNCLRGREEN 2001
#define ID_BTNCLRBLUE 2002

enum DrawMode
{
	Line,
	Box,
	Circle
};

HMODULE hLib;
HINSTANCE hInstance;
char szClassName[] = "WindowAppClass";
DrawMode dMode = DrawMode::Line;
bool isDrawing = false;
POINT pStart, pEnd;
COLORREF clr = RGB(0, 255, 0);

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

bool RegClass(WNDPROC proc, LPCSTR szName, UINT color) {
	WNDCLASS windowClass;
	windowClass.style = 0;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.lpfnWndProc = proc;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadCursor(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(color + 1);
	windowClass.lpszMenuName = (LPCTSTR)NULL;
	windowClass.lpszClassName = szName;

	return RegisterClass(&windowClass);
}

/*bool(*DrawLine)(HDC, int, int, int, int);

POINT(*FindDelta)(POINT, POINT);*/

LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {
	static HWND hBtnLine, hBtnBox, hBtnCircle;
	static HWND hBtnClrRed, hBtnClrGreen, hBtnClrBlue;

	switch (msg) {

	case WM_CREATE: {

		CreateWindow(
			"button", "Фигура",
			WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
			20, 20,
			140, 155,
			hwnd, (HMENU)0, hInstance, NULL
		);

		hBtnLine = CreateWindow(
			"button", "Линия",
			WS_CHILD | WS_VISIBLE,
			30, 45,
			120, 30,
			hwnd, (HMENU) ID_BTNLINE, hInstance, NULL
		);

		hBtnBox = CreateWindow(
			"button", "Прямоугольник",
			WS_CHILD | WS_VISIBLE,
			30, 85,
			120, 30,
			hwnd, (HMENU)ID_BTNBOX, hInstance, NULL
		);

		hBtnCircle = CreateWindow(
			"button", "Круг",
			WS_CHILD | WS_VISIBLE,
			30, 125,
			120, 30,
			hwnd, (HMENU)ID_BTNCIRCLE, hInstance, NULL
		);

		CreateWindow(
			"button", "Цвет",
			WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
			20, 190,
			140, 155,
			hwnd, (HMENU)0, hInstance, NULL
		);

		hBtnClrRed = CreateWindow(
			"button", "Красный",
			WS_CHILD | WS_VISIBLE,
			30, 215,
			120, 30,
			hwnd, (HMENU)ID_BTNCLRRED, hInstance, NULL
		);

		hBtnClrGreen = CreateWindow(
			"button", "Зелёный",
			WS_CHILD | WS_VISIBLE,
			30, 255,
			120, 30,
			hwnd, (HMENU)ID_BTNCLRGREEN, hInstance, NULL
		);

		hBtnClrBlue = CreateWindow(
			"button", "Синий",
			WS_CHILD | WS_VISIBLE,
			30, 295,
			120, 30,
			hwnd, (HMENU)ID_BTNCLRBLUE, hInstance, NULL
		);
		
		return 0;
	}

	case WM_COMMAND: {

		switch (LOWORD(wParam))
		{
		case ID_BTNLINE: {
			dMode = DrawMode::Line;
			break;
		}

		case ID_BTNBOX: {
			dMode = DrawMode::Box;
			break;
		}

		case ID_BTNCIRCLE: {
			dMode = DrawMode::Circle;
			break;
		}

		case ID_BTNCLRRED: {
			clr = RGB(255, 0, 0);
			break;
		}

		case ID_BTNCLRGREEN: {
			clr = RGB(0, 255, 0);
			break;
		}

		case ID_BTNCLRBLUE: {
			clr = RGB(0, 0, 255);
			break;
		}

		default:
			break;
		}
		isDrawing = false;
		return 0;
	}

	case WM_LBUTTONDOWN: {
		
		if (!isDrawing) {
			pStart.x = LOWORD(lParam);
			pStart.y = HIWORD(lParam);
			isDrawing = true;
		}
		else {
			pEnd.x = LOWORD(lParam);
			pEnd.y = HIWORD(lParam);
			HDC hdc = GetDC(hwnd);
			HBRUSH hOldBrush, hNewBrush;
			HPEN hOldPen, hNewPen;
			hNewBrush = CreateSolidBrush(clr);
			hNewPen = CreatePen(PS_SOLID, 1, clr);
			hOldBrush = (HBRUSH) SelectObject(hdc, hNewBrush);
			hOldPen = (HPEN)SelectObject(hdc, hNewPen);
			switch (dMode)
			{
			case DrawMode::Line: {
				(*DrawLine)(hdc, pStart.x, pStart.y, pEnd.x, pEnd.y);
				break;
			}

			case DrawMode::Box: {
				Rectangle(hdc, pStart.x, pStart.y, pEnd.x, pEnd.y);
				break;
			}

			case DrawMode::Circle: {
				POINT pDelta = (*FindDelta)(pStart, pEnd);
				Ellipse(hdc, pStart.x, pStart.y, pStart.x + pDelta.x, pStart.y + pDelta.y);
				break;
			}

			default:
				break;
			}
			SelectObject(hdc, hOldBrush);
			SelectObject(hdc, hOldPen);
			DeleteObject(hNewBrush);
			DeleteObject(hNewPen);
			ReleaseDC(hwnd, hdc);
			isDrawing = false;
		}
		
		
	
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

	hwnd = CreateWindow(
		szClassName,
		"lab4",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		0,
		0,
		hInstance,
		NULL
	);

	/*hLib = LoadLibrary("MyDll.dll");
	DrawLine = (bool(*)(HDC, int, int, int, int)) GetProcAddress(hLib, "DrawLine");
	FindDelta = (POINT(*)(POINT, POINT)) GetProcAddress(hLib, "FindDelta");*/

	if (!hwnd) { return  false; }

	while (GetMessage(&msg, 0, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}

	return msg.wParam;
}