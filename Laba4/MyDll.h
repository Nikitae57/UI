#include <windows.h>
#include <stdio.h>
#include <math.h>

extern "C" __declspec(dllexport) bool DrawLine(HDC hdc, int x0, int y0, int x, int y);
extern "C" __declspec(dllexport) POINT FindDelta(POINT start, POINT end);