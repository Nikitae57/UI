#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <psapi.h>

#include "DbHandler.h"

const char *mainWindowClass = "WindowAppClass";
const char *etTableNameClass = "etTableNameClass";
const char *etSelectQueryClass = "etSelectQueryClass";
const char *llTableFieldsClass = "llTableFieldsClass";

WNDPROC etTableNameProc;

HWND etTableNameHwnd;
HWND etSelectQueryHwnd;
HWND llTableFieldsHwnd;
HWND llSelectHwnd;
HWND btnOkHwnd;
HMENU menuHmenu;

const int ID_TABLE_ATTRS_LISTBOX = 6785;

const UINT_PTR actionQueryMode = 1337;
const UINT_PTR actionTableMode = 2608;

char buffer[2048];

HINSTANCE hInstance;

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

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

HMENU initializeMenu(HWND parent) {
  HMENU menu = CreateMenu();
  HMENU dropDownMenuItem = CreatePopupMenu();

  AppendMenu(menu, MF_STRING | MF_POPUP, (UINT_PTR) dropDownMenuItem, TEXT("Mode"));
  AppendMenu(dropDownMenuItem, MF_STRING, actionTableMode, TEXT("Table mode"));
  AppendMenu(dropDownMenuItem, MF_STRING, actionQueryMode, TEXT("Query mode"));
  SetMenu(parent, menu);
  SetMenu(parent, dropDownMenuItem);

  return menu;
}

void switchToQueryMode() {
  // TODO implement
}

void switchToTableMode() {
  // TODO implement
}

void inflateListView(char** items, int nItems) {
  int count = SendMessage(
      llTableFieldsHwnd,
      LB_GETCOUNT, 0, 0L
  );

  while (count > 0) {
    count = SendMessage(
        llTableFieldsHwnd,
        LB_DELETESTRING,
        (WPARAM) 0,
        0L
    );
  }

  for (int i = 0; i < nItems; i++) {
    int pos = (int) SendMessage(
        llTableFieldsHwnd,
        LB_ADDSTRING,
        0,
        (LPARAM) items[i]
    );

    SendMessage(llTableFieldsHwnd, LB_SETITEMDATA, pos, (LPARAM) i);
  }

  SetFocus(llTableFieldsHwnd);
}

LRESULT CALLBACK etTableProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
  switch (msg) {
    case WM_KEYDOWN: {
      //std::cout << "keydown\n";
      if (wParam == VK_RETURN) {
        GetWindowText(hwnd, buffer, 2048);
        int colNumber;
        char** result = getTableColumns(buffer, &colNumber);
        inflateListView(result, colNumber);
      }

      return 0;
    }

    default: {
      return CallWindowProc(etTableNameProc, hwnd, msg, wParam, lParam);
    }
  }
}

HWND CreateTableAttrListView(HWND hWndParent, UINT uId) {
  HWND listbox = CreateWindow(
      TEXT("listbox"),
      TEXT("tableAttrs"),
      WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL | WS_BORDER
      | LBS_NOTIFY,
      220, 10,
      200, 150,
      hWndParent,
      (HMENU) ID_TABLE_ATTRS_LISTBOX,
      hInstance,
      NULL
  );

  return listbox;
}

HWND CreateSelectListView(HWND parent, UINT uId) {
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC  = ICC_LISTVIEW_CLASSES;
  InitCommonControlsEx(&icex);

  HWND hWndLV = CreateWindow(
      WC_LISTVIEW,
      "",
      WS_CHILD | LVS_REPORT | WS_VISIBLE,
      10, 162,
      600, 200,
      parent,
      (HMENU)uId,
      hInstance,
      NULL
  );

  // Чтобы определялись строка (item) и столбец (subitem) обязательно устанавливаем
  // расширенный стиль LVS_EX_FULLROWSELECT.
  ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

  return (hWndLV);
}

void initUi(HWND hwnd) {
  menuHmenu = initializeMenu(hwnd);
  etTableNameHwnd = CreateWindow(
      TEXT("EDIT"),
      NULL,
      WS_BORDER | WS_CHILD | WS_VISIBLE,
      10, 10,
      200, 32,
      hwnd,
      NULL,
      hInstance,
      NULL
  );

  etTableNameProc = (WNDPROC)SetWindowLongPtr(etTableNameHwnd, GWLP_WNDPROC, (LONG_PTR)etTableProc);

  etSelectQueryHwnd = CreateWindow(
      TEXT("EDIT"),
      NULL,
      WS_BORDER | WS_CHILD | WS_VISIBLE | ES_MULTILINE,
      10, 52,
      200, 100,
      hwnd,
      NULL,
      hInstance,
      NULL
  );

  llTableFieldsHwnd = CreateTableAttrListView(hwnd, 1);
  llSelectHwnd = CreateSelectListView(hwnd, 2);
}

void handleWmCommand(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
  switch (LOWORD(wParam)) {
    case actionQueryMode: {
      switchToQueryMode();
      return;
    }

    case ID_TABLE_ATTRS_LISTBOX: {
      switch (HIWORD(wParam)) {
        return;
      }
    }

    case actionTableMode: {
      switchToTableMode();
      return;
    }
  }
}

LRESULT CALLBACK mainWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {

  switch (msg) {
    case WM_CREATE: {
      initUi(hwnd);
      return 0;
    }

    case WM_COMMAND: {
      handleWmCommand(hwnd, msg, wParam, lParam);
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

  if (!RegClass(mainWindowProc, mainWindowClass, COLOR_WINDOW)) {
    return false;
  }

  hwnd = CreateWindow(
      mainWindowClass,
      "lab5",
      WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      800,
      600,
      0,
      0,
      hInstance,
      NULL
  );

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  if (!hwnd) { return false; }

  while (GetMessage(&msg, 0, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}