#include "mydll.h"
using namespace std;
extern "C" __declspec(dllexport) bool DrawLine(HDC hdc, int x0, int y0, int x, int y) {
	POINT pt;
	MoveToEx(hdc, x0, y0, &pt);
	return LineTo(hdc, x, y);
}

extern "C" __declspec(dllexport) POINT FindDelta(POINT start, POINT end) {
	POINT res;
	int delta = fmin(abs(end.x - start.x), abs(end.y - start.y));
	res.x = (end.x > start.x) ? delta : -delta;
	res.y = (end.y > start.y) ? delta : -delta;
	return res;
}
