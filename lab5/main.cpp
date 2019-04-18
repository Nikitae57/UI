#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <psapi.h>
#include <unistd.h>

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
const int ID_SELECT_TABLE = 5623;
const int ID_OK_BTN = 2908;

const UINT_PTR actionQueryMode = 1337;
const UINT_PTR actionTableMode = 2608;

char buffer[2048];
bool attrBeenSelected = false;

int selectedColumnsNumber = 0;
char **selectedColumns;

char **tableColumns;
int tableColumnsNumber;

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

void inflateTableAttrsLb(char **items, int nItems) {
  SendMessage(
      llTableFieldsHwnd,
      LB_RESETCONTENT,
      0, 0L
  );

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
      if (wParam == VK_RETURN) {
        GetWindowText(hwnd, buffer, 2048);
        tableColumns = getTableColumns(buffer, &tableColumnsNumber);
        inflateTableAttrsLb(tableColumns, tableColumnsNumber);
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

void inflateLvHeader(char** titles, int columnsNumber) {
  for (int i = 0; i < columnsNumber; i++) {
    LV_COLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.cx = 100;
    col.pszText = titles[i];
    col.iSubItem = i;
    ListView_InsertColumn(llSelectHwnd, i, &col);
  }
}

HWND CreateSelectListView(HWND parent, UINT uId) {
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC  = ICC_LISTVIEW_CLASSES;
  InitCommonControlsEx(&icex);

  HWND hWndLV = CreateWindowEx(
      0L,
      WC_LISTVIEW,
      "",
      WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT |
      LVS_EDITLABELS | LVS_NOSORTHEADER,
      10, 162,
      600, 200,
      parent,
      (HMENU) ID_SELECT_TABLE,
      hInstance,
      NULL
  );

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
  btnOkHwnd = CreateWindow(
      TEXT("BUTTON"),
      TEXT("OK"),
      WS_CHILD | WS_VISIBLE,
      430, 10,
      50, 30,
      hwnd,
      (HMENU) ID_OK_BTN,
      hInstance,
      NULL
  );
}

void tableAttrSelected() {
  // selected item index
  int lbItem = (int) SendMessage(
      llTableFieldsHwnd,
      LB_GETCURSEL, 0, 0
  );

  char attrName[256];
  SendMessage(
      llTableFieldsHwnd,
      LB_GETTEXT,
      (WPARAM) lbItem,
      (LPARAM) attrName
  );
  std::cout << "Selected table attr: " << attrName << std::endl;

  if (selectedColumnsNumber <= 0) {
    selectedColumns = (char **) malloc(sizeof(char *) * 100);
  }

  selectedColumns[selectedColumnsNumber] = (char*)
    malloc(sizeof(char) * strlen(attrName));
  strcpy(selectedColumns[selectedColumnsNumber], attrName);
  selectedColumnsNumber++;

  GetWindowText(etSelectQueryHwnd, buffer, 2048);
  char tmp[2048];
  if (strlen(buffer) == 0) {
    sprintf(tmp, "SELECT %s ", attrName);
  } else {
    sprintf(tmp, "%s, %s", buffer, attrName);
  }
  SetWindowText(etSelectQueryHwnd, tmp);
  attrBeenSelected = true;
}

void finishSelectQuery() {
  char tmp[2048];
  char tableName[256];
  GetWindowText(etTableNameHwnd, tableName, 256);

  if (attrBeenSelected) {
    GetWindowText(etSelectQueryHwnd, buffer, 2048);
    sprintf(tmp, "%s FROM %s", buffer, tableName);
  } else {
    sprintf(tmp, "SELECT * FROM %s", tableName);
  }
  SetWindowText(etSelectQueryHwnd, tmp);
}

void inflateSelectLvBody(
    char*** selectResult,
    int rowCount,
    int colCount
) {

  LV_ITEM item;
  memset(&item, 0, sizeof(LV_ITEM));
  item.mask = LVIF_TEXT;
  for (int i = 0; i < rowCount; i++) {
    item.iItem = i;
    for (int j = 0; j < colCount; j++) {
      item.iSubItem = j;
      item.pszText = selectResult[i][j];
      item.cchTextMax = 256;
      ListView_InsertItem(llSelectHwnd, &item);
      ListView_SetItem(llSelectHwnd, &item);
//      std::cout << selectResult[i][j] << " ";
    }
//    std::cout << std::endl;
  }
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
        case LBN_DBLCLK: {
          tableAttrSelected();
        }
      }
      return;
    }

    case ID_OK_BTN: {
      finishSelectQuery();
      int columnsToInflate;
      if (selectedColumnsNumber > 0) {
        inflateLvHeader(selectedColumns, selectedColumnsNumber);
        columnsToInflate = selectedColumnsNumber;
      } else {
        inflateLvHeader(tableColumns, tableColumnsNumber);
        columnsToInflate = tableColumnsNumber;
      }

      char* selectStatement = (char*) malloc(sizeof(char) * 500);
      GetWindowText(etSelectQueryHwnd, selectStatement, 500);

      int rowCount;
      char*** selectResult = makeSelectQuery(selectStatement, &rowCount);
      inflateSelectLvBody(selectResult, rowCount, columnsToInflate);

      return;
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