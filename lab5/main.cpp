#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <commctrl.h>
#include <psapi.h>
#include <unistd.h>
#include <windowsx.h>

#include "DbHandler.h"
#include "main.h"
#include "StateHandler.h"

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
	struct SELECT_TABLE_STATE_9 st9;
	switchState(st9);
	currentState = 9;
}

void switchToTableMode() {
	SELECT_TABLE_STATE_2 st;
	switchState(st);
	currentState = 2;
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
		  if (currentState == 2) {
			  CONNECT_TO_BD_STATE_3 st;
			  switchState(st);
			  currentState = 3;
			  GetWindowText(hwnd, buffer, 2048);
			  tableColumns = getTableColumns(buffer, &tableColumnsNumber);
			  if (tableColumns != NULL && tableColumnsNumber > 0) {
				  SELECT_ATTRIBUTES_5 st5;
				  switchState(st5);
				  currentState = 5;
				  inflateTableAttrsLb(tableColumns, tableColumnsNumber);
			  }
			  else {
				  FAILED_CONNECT_STATE_4 st4;
				  switchState(st4);
				  SELECT_TABLE_STATE_2 st2;
				  switchState(st2);
				  currentState = 2;
			  }
		  }
		  else if (currentState == 9) {
			  CONNECT_TO_BD_STATE_10 st10;
			  switchState(st10);
			  currentState = 10;
			  GetWindowText(hwnd, buffer, 2048);
			  tableColumns = getTableColumns(buffer, &tableColumnsNumber);
			  if (tableColumns != NULL && tableColumnsNumber > 0) {
				  SELECT_ATTRIBUTES_12 st12;
				  switchState(st12);
				  currentState = 12;
				  inflateTableAttrsLb(tableColumns, tableColumnsNumber);
			  }
			  else {
				  FAILED_CONNECT_STATE_11 st11;
				  switchState(st11);
				  SELECT_TABLE_STATE_9 st9;
				  switchState(st9);
				  currentState = 9;
			  }
		  }
      }

      return 0;
    }

    default: {
      return CallWindowProc(etTableNameProc, hwnd, msg, wParam, lParam);
    }
  }
}

void ResetContext() {
	attrBeenSelected = false;
	if (selectedColumnsNumber > 0) {
		for (int i = 0; i < selectedColumnsNumber; i++) {
			ListView_DeleteColumn(llSelectHwnd, 0);
		}
	}
	else {
		for (int i = 0; i < tableColumnsNumber; i++) {
			ListView_DeleteColumn(llSelectHwnd, 0);
		}
	}
	if (tableColumns != NULL) {
		for (int i = 0; i < tableColumnsNumber; i++) {
			free(tableColumns[i]);
		}
		free(tableColumns);
	}
	tableColumnsNumber = 0;
	selectedColumnsNumber = 0;
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

  btnNextHwnd = CreateWindow(
      TEXT("BUTTON"),
      TEXT(">>"),
      WS_CHILD | WS_VISIBLE,
      430, 50,
      50, 30,
      hwnd,
      (HMENU) ID_NEXT_BTN,
      hInstance,
      NULL
  );

  etComparisonValueHwnd = CreateWindow(
      TEXT("EDIT"),
      NULL,
      WS_BORDER | WS_CHILD | WS_VISIBLE,
      430, 90,
      50, 30,
      hwnd,
      NULL,
      hInstance,
      NULL
  );

  llComparisonSignsHwnd = CreateWindow(
      TEXT("COMBOBOX"),
      TEXT("Attr operation"),
      WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
      490, 10,
      50, 200,
      hwnd,
      NULL,
      hInstance,
      NULL
  );

  initStateHandler(
	  hwnd,
      etTableNameHwnd,
      llTableFieldsHwnd,
      etSelectQueryHwnd,
      llSelectHwnd,
      btnOkHwnd,
      btnNextHwnd,
      etComparisonValueHwnd,
      llComparisonSignsHwnd,
	  menuHmenu
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

void addAttributeToQuery() {
	int lbItem = (int)SendMessage(
		llTableFieldsHwnd,
		LB_GETCURSEL, 0, 0
	);

	char attrName[256];
	SendMessage(
		llTableFieldsHwnd,
		LB_GETTEXT,
		(WPARAM)lbItem,
		(LPARAM)attrName
	);

	GetWindowText(etSelectQueryHwnd, buffer, 2048);
	char tmp[2048];
	sprintf(tmp, "%s %s", buffer, attrName);
	SetWindowText(etSelectQueryHwnd, tmp);
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

void addSectionFromAndWhere() {
	char tmp[2048];
	char tableName[256];
	GetWindowText(etTableNameHwnd, tableName, 256);

	if (attrBeenSelected) {
		GetWindowText(etSelectQueryHwnd, buffer, 2048);
		sprintf(tmp, "%s FROM %s WHERE", buffer, tableName);
	}
	else {
		sprintf(tmp, "SELECT * FROM %s WHERE", tableName);
	}
	SetWindowText(etSelectQueryHwnd, tmp);
}

void addComboBoxItem() {
	char tmp[2048];
	char sign[10];
	GetWindowText(etSelectQueryHwnd, buffer, 2048);
	ComboBox_GetText(llComparisonSignsHwnd, sign, 10);
	sprintf(tmp, "%s %s", buffer, sign);
	SetWindowText(etSelectQueryHwnd, tmp);
}

bool addComparisonValue() {
	char value[256];
	Edit_GetText(etComparisonValueHwnd, value, 256);
	if (strcmp(value, "") == 0) {
		return false;
	}
	else {
		char tmp[2048];
		GetWindowText(etSelectQueryHwnd, buffer, 2048);
		sprintf(tmp, "%s %s", buffer, value);
		SetWindowText(etSelectQueryHwnd, tmp);
		return true;
	}
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
    }
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
			if (currentState == 5 || currentState == 12) {
				tableAttrSelected();
			}
			else if (currentState == 16) {
				addAttributeToQuery();
				struct SELECT_COMPARISON_SIGN_STATE_17 st17;
				switchState(st17);
				currentState = 17;
			}
          
        }
      }
      return;
    }

    case ID_OK_BTN: {
		if (currentState == 5) {
			struct CONNECT_TO_BD_STATE_6 st;
			switchState(st);
			currentState = 6;
			finishSelectQuery();
			int columnsToInflate;
			if (selectedColumnsNumber > 0) {
				inflateLvHeader(selectedColumns, selectedColumnsNumber);
				columnsToInflate = selectedColumnsNumber;
			}
			else {
				inflateLvHeader(tableColumns, tableColumnsNumber);
				columnsToInflate = tableColumnsNumber;
			}

			char* selectStatement = (char*)malloc(sizeof(char) * 500);
			GetWindowText(etSelectQueryHwnd, selectStatement, 500);

			int rowCount;
			char*** selectResult = makeSelectQuery(selectStatement, &rowCount);
			free(selectStatement);
			if (selectResult != NULL) {
				OUTPUT_DATA_STATE_8 st8;
				switchState(st8);
				currentState = 8;
				inflateSelectLvBody(selectResult, rowCount, columnsToInflate);
				for (int i = 0; i < rowCount; i++) {
					for (int j = 0; j < selectedColumnsNumber; j++) {
						free(selectResult[i][j]);
					}
					free(selectResult[i]);
				}
				free(selectResult);
			}
			else {
				FAILED_CONNECT_STATE_7 st7;
				switchState(st7);
				SELECT_TABLE_STATE_2 st2;
				switchState(st2);
				currentState = 2;
				ResetContext();
			}
		}
		else if (currentState == 8 || currentState == 15) {
			SELECT_MODE_STATE_1 st;
			switchState(st);
			currentState = 1;
			ResetContext();
		}
		else if (currentState == 12 || currentState == 19) {
			struct CONNECT_TO_BD_STATE_13 st13;
			switchState(st13);
			if (currentState != 19) {
				finishSelectQuery();
			}
			currentState = 13;
			int columnsToInflate;
			if (selectedColumnsNumber > 0) {
				inflateLvHeader(selectedColumns, selectedColumnsNumber);
				columnsToInflate = selectedColumnsNumber;
			}
			else {
				inflateLvHeader(tableColumns, tableColumnsNumber);
				columnsToInflate = tableColumnsNumber;
			}

			char* selectStatement = (char*)malloc(sizeof(char) * 500);
			GetWindowText(etSelectQueryHwnd, selectStatement, 500);

			int rowCount;
			char*** selectResult = makeSelectQuery(selectStatement, &rowCount);
			free(selectStatement);
			if (selectResult != NULL) {
				OUTPUT_DATA_STATE_15 st15;
				switchState(st15);
				currentState = 15;
				inflateSelectLvBody(selectResult, rowCount, columnsToInflate);
				for (int i = 0; i < rowCount; i++) {
					for (int j = 0; j < selectedColumnsNumber; j++) {
						free(selectResult[i][j]);
					}
					free(selectResult[i]);
				}
				free(selectResult);
			}
			else {
				FAILED_CONNECT_STATE_14 st14;
				switchState(st14);
				SELECT_TABLE_STATE_9 st9;
				switchState(st9);
				currentState = 9;
				ResetContext();
			}
		}

      return;
    }

    case ID_NEXT_BTN: {
		if (currentState == 12) {
			struct SELECT_ATTRIBUTE_STATE_16 st16;
			switchState(st16);
			currentState = 16;
			addSectionFromAndWhere();

		}
		else if (currentState == 17) {
			struct SELECT_COMPARISON_VALUE_STATE_18 st18;
			addComboBoxItem();
			switchState(st18);
			currentState = 18;
		}
		else if (currentState == 18) {
			if (addComparisonValue()) {
				struct SELECT_LOGICAL_CONDITION_STATE_19 st19;
				switchState(st19);
				currentState = 19;
			}
		}
		else if (currentState == 19) {
			struct SELECT_ATTRIBUTE_STATE_16 st16;
			addComboBoxItem();
			switchState(st16);
			currentState = 16;
		}
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
	  struct SELECT_MODE_STATE_1 st;
	  switchState(st);
	  currentState = 1;
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