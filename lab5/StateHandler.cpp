#include "StateHandler.h"

HWND this_parent;
HWND this_etTableNameHwnd;
HWND this_llTableFieldsHwnd;
HWND this_etSelectQueryHwnd;
HWND this_llSelectHwnd;
HWND this_btnOkHwnd;

HWND this_btnNextHwnd;
HWND this_etComparisonValueHwnd;
HWND this_llComparisonSignsHwnd;

HMENU this_hMenu;
HMENU this_hSubMenu;

const int UI_ELEMENTS_COUNT = 10;
const int UI_STATES_COUNT = 15;
const int UI_INPUT_SIGNALS_COUNT = 6;

void(*stateMatrix[6][15])();
UI_STATES transitionMatrix[6][15];
UI_CONTEXT_STATE contextMatrix[15][10];
UI_STATES currentState;

std::map<UI_ELEMENTS, HWND> elementsToHwnd;

char ErrorMSG[256] = "������";

char buffer[2048];
bool attrBeenSelected = false;

int selectedColumnsNumber = 0;
char **selectedColumns = nullptr;

char **tableColumns = nullptr;
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
) {
	
	this_parent = parent;
	elementsToHwnd[UI_ELEMENTS::TABLE_NAME_EDIT] = this_etTableNameHwnd = etTableName;
	elementsToHwnd[UI_ELEMENTS::TABLE_ATTRS_LIST] = this_llTableFieldsHwnd = llTableFields;
	elementsToHwnd[UI_ELEMENTS::SQL_STATEMENT_EDIT] = this_etSelectQueryHwnd = etSelectQuery;
	elementsToHwnd[UI_ELEMENTS::SELECT_RESULT_TABLE] = this_llSelectHwnd = llSelect;
	elementsToHwnd[UI_ELEMENTS::OK_BTN] = this_btnOkHwnd = btnOk;

	elementsToHwnd[UI_ELEMENTS::NEXT_BTN] = this_btnNextHwnd = btnNext;
	elementsToHwnd[UI_ELEMENTS::COMPARISON_EDIT] = this_etComparisonValueHwnd = etComparisonValue;
	elementsToHwnd[UI_ELEMENTS::COMPARISON_DROP_LIST] = this_llComparisonSignsHwnd = llComparisonSigns;
	
	this_hMenu = hMenu;
	this_hSubMenu = GetSubMenu(hMenu, 0);

	initStateMatrix();
	initTransitionMatrix();
	initContextMatrix();

}

void initStateMatrix() {
    stateMatrix[UI_INPUT_SIGNALS::CLICK_MENU_TABLE][UI_STATES::MODE_SELECTION_1] = []() {
        ResetContext();
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_MENU_QUERY][UI_STATES::MODE_SELECTION_1] = []() {
        ResetContext();
    };

    stateMatrix[UI_INPUT_SIGNALS::ENTER_KEY_PRESSED][UI_STATES::TABLE_SELECTION_2] = []() {
        GetWindowText(this_etTableNameHwnd, buffer, 2048);
        tableColumns = getTableColumns(buffer, &tableColumnsNumber);
        if (tableColumns != NULL && tableColumnsNumber > 0) {
            inflateTableAttrsLb(tableColumns, tableColumnsNumber);
        }
        else {
			strcpy(ErrorMSG, "������� �� �������");
			PostMessage(this_parent, WM_ERRMSG, (WPARAM)0, (LPARAM)UI_STATES::ERROR_MSG_4);
        }
    };

    stateMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTRS_SELECTION_3] = []() {
        tableAttrSelected();
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::TABLE_ATTRS_SELECTION_3] = []() {
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
        GetWindowText(this_etSelectQueryHwnd, selectStatement, 500);

        int rowCount;
		char ***selectResult = makeSelectQuery(selectStatement, &rowCount);
        free(selectStatement);
        if (selectResult != NULL) {
            inflateSelectLvBody(selectResult, rowCount, columnsToInflate);

            for (int i = 0; i < rowCount; i++) {
                for (int j = 0; j < columnsToInflate; j++) {
                    free(selectResult[i][j]);
                }
                free(selectResult[i]);
            }
            free(selectResult);
        }
        else {
			strcpy(ErrorMSG, "������ ��� ���������� �������");
			PostMessage(this_parent, WM_ERRMSG, (WPARAM)0, (LPARAM)UI_STATES::ERROR_MSG_4);
        }
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::ERROR_MSG_4] = []() {
        ResetContext();
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::QUERY_RESULT_OUTPUT_5] = []() {
        ResetContext();
    };

    stateMatrix[UI_INPUT_SIGNALS::ENTER_KEY_PRESSED][UI_STATES::TABLE_SELECTION_6] = []() {
		GetWindowText(this_etTableNameHwnd, buffer, 2048);
		tableColumns = getTableColumns(buffer, &tableColumnsNumber);
		if (tableColumns != NULL && tableColumnsNumber > 0) {
			inflateTableAttrsLb(tableColumns, tableColumnsNumber);
		}
		else {
			strcpy(ErrorMSG, "������� �� �������");
			PostMessage(this_parent, WM_ERRMSG, (WPARAM)0, (LPARAM)UI_STATES::ERROR_MSG_8);
		}
    };

    stateMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTRS_SELECTION_7] = []() {
		tableAttrSelected();
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::TABLE_ATTRS_SELECTION_7] = []() {
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
		GetWindowText(this_etSelectQueryHwnd, selectStatement, 500);

		int rowCount;
		char ***selectResult = makeSelectQuery(selectStatement, &rowCount);
		free(selectStatement);
		if (selectResult != NULL) {
			inflateSelectLvBody(selectResult, rowCount, columnsToInflate);
            for (int i = 0; i < rowCount; i++) {
                for (int j = 0; j < columnsToInflate; j++) {
                    free(selectResult[i][j]);
                }
                free(selectResult[i]);
            }
            free(selectResult);
		}
		else {
			strcpy(ErrorMSG, "������ ��� ���������� �������");
			PostMessage(this_parent, WM_ERRMSG, (WPARAM)0, (LPARAM)UI_STATES::ERROR_MSG_8);
		}
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::TABLE_ATTRS_SELECTION_7] = []() {
		addSectionFromAndWhere();
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::ERROR_MSG_8] = []() {
		ResetContext();
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::QUERY_RESULT_OUTPUT_9] = []() {
		ResetContext();
    };

    stateMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTR_SELECTION_10] = []() {
		addAttributeToQuery();
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT(">"));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT(">="));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("<"));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("<="));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("="));
		ComboBox_SetCurSel(this_llComparisonSignsHwnd, 0);
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::COMPARISON_SIGN_SELECTION_11] = []() {
		addComboBoxItem();
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::COMPARISON_VALUE_INPUT_12] = []() {
		addComparisonValue();
		Edit_SetText(this_etComparisonValueHwnd, "");
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("AND"));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("OR"));
		ComboBox_SetCurSel(this_llComparisonSignsHwnd, 0);
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::LOGICAL_OPERATION_SELECTION_13] = []() {
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
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
		GetWindowText(this_etSelectQueryHwnd, selectStatement, 500);

		int rowCount;
		char ***selectResult = makeSelectQuery(selectStatement, &rowCount);
		free(selectStatement);
		if (selectResult != NULL) {
			inflateSelectLvBody(selectResult, rowCount, columnsToInflate);
            for (int i = 0; i < rowCount; i++) {
                for (int j = 0; j < columnsToInflate; j++) {
                    free(selectResult[i][j]);
                }
                free(selectResult[i]);
            }
            free(selectResult);
		}
		else {
			strcpy(ErrorMSG, "������ ��� ���������� �������");
			PostMessage(this_parent, WM_ERRMSG, (WPARAM)0, (LPARAM)UI_STATES::ERROR_MSG_8);
		}
    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::LOGICAL_OPERATION_SELECTION_13] = []() {
		addComboBoxItem();
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
    };
}

void initTransitionMatrix() {
    transitionMatrix[UI_INPUT_SIGNALS::CLICK_MENU_TABLE][UI_STATES::MODE_SELECTION_1] = UI_STATES::TABLE_SELECTION_2;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_MENU_QUERY][UI_STATES::MODE_SELECTION_1] = UI_STATES::TABLE_SELECTION_6;

    transitionMatrix[UI_INPUT_SIGNALS::ENTER_KEY_PRESSED][UI_STATES::TABLE_SELECTION_2] = UI_STATES::TABLE_ATTRS_SELECTION_3;

    transitionMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTRS_SELECTION_3] = UI_STATES::TABLE_ATTRS_SELECTION_3;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::TABLE_ATTRS_SELECTION_3] = UI_STATES::QUERY_RESULT_OUTPUT_5;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::ERROR_MSG_4] = UI_STATES::TABLE_SELECTION_2;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::QUERY_RESULT_OUTPUT_5] = UI_STATES::MODE_SELECTION_1;

    transitionMatrix[UI_INPUT_SIGNALS::ENTER_KEY_PRESSED][UI_STATES::TABLE_SELECTION_6] = UI_STATES::TABLE_ATTRS_SELECTION_7;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::TABLE_ATTRS_SELECTION_7] = UI_STATES::QUERY_RESULT_OUTPUT_9;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::TABLE_ATTRS_SELECTION_7] = UI_STATES::TABLE_ATTR_SELECTION_10;

    transitionMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTRS_SELECTION_7] = UI_STATES::TABLE_ATTRS_SELECTION_7;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::ERROR_MSG_8] = UI_STATES::TABLE_SELECTION_6;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::QUERY_RESULT_OUTPUT_9] = UI_STATES::MODE_SELECTION_1;

    transitionMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTR_SELECTION_10] = UI_STATES::COMPARISON_SIGN_SELECTION_11;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::COMPARISON_SIGN_SELECTION_11] = UI_STATES::COMPARISON_VALUE_INPUT_12;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::COMPARISON_VALUE_INPUT_12] = UI_STATES::LOGICAL_OPERATION_SELECTION_13;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::LOGICAL_OPERATION_SELECTION_13] = UI_STATES::TABLE_ATTR_SELECTION_10;

    transitionMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::LOGICAL_OPERATION_SELECTION_13] = UI_STATES::QUERY_RESULT_OUTPUT_9;
}

void initContextMatrix() {
	std::string tempMatrix[] = {
		"0000000000",
		"2000000000",
		"0012110000",
		"0021120000",
		"0011110002",
		"0211120000",
		"0012111110",
		"0021122110",
		"0011111112",
		"0211121110",
		"0021112110",
		"0011112120",
		"0011112210",
		"0011122120",
		"0000000000"
	};

	for (int i = 0; i < UI_STATES_COUNT; i++) {
		for (int j = 0; j < UI_ELEMENTS_COUNT; j++) {
			contextMatrix[i][j] = (UI_CONTEXT_STATE)(tempMatrix[i][j] - '0');
		}
	}

}

void switchContext(UI_STATES st) {
	switch (contextMatrix[st][UI_ELEMENTS::MENU])
	{

        case UI_CONTEXT_STATE::INVISIBLE: {
            RemoveMenu(this_hMenu,(UINT_PTR) this_hSubMenu, MF_BYCOMMAND);
            break;
        }

        case UI_CONTEXT_STATE::DISABLED: {
            RemoveMenu(this_hMenu, (UINT_PTR)this_hSubMenu, MF_BYCOMMAND);
            InsertMenu(this_hMenu, 0, MF_STRING | MF_POPUP | MF_BYPOSITION, (UINT_PTR)this_hSubMenu, TEXT("Mode"));
            EnableMenuItem(this_hMenu, (UINT_PTR)this_hSubMenu, MF_GRAYED | MF_BYCOMMAND);
            break;
        }

        case UI_CONTEXT_STATE::ENABLED: {
            RemoveMenu(this_hMenu, (UINT_PTR)this_hSubMenu, MF_BYCOMMAND);
            InsertMenu(this_hMenu, 0, MF_STRING | MF_POPUP | MF_BYPOSITION, (UINT_PTR)this_hSubMenu, TEXT("Mode"));
            EnableMenuItem(this_hMenu, (UINT_PTR)this_hSubMenu, MF_ENABLED | MF_BYCOMMAND);
            break;
        }

        default:
            break;
	}
	DrawMenuBar(this_parent);

	for (int i = 1; i < UI_ELEMENTS_COUNT - 1; i++) {
		switch (contextMatrix[st][i])
		{
		case UI_CONTEXT_STATE::INVISIBLE: {
			ShowWindow(elementsToHwnd[(UI_ELEMENTS)i], SW_HIDE);
			break;
		}
										  
		case UI_CONTEXT_STATE::DISABLED: {
			ShowWindow(elementsToHwnd[(UI_ELEMENTS)i], SW_SHOW);
			EnableWindow(elementsToHwnd[(UI_ELEMENTS)i], false);
			break;
		}

		case UI_CONTEXT_STATE::ENABLED: {
			ShowWindow(elementsToHwnd[(UI_ELEMENTS)i], SW_SHOW);
			EnableWindow(elementsToHwnd[(UI_ELEMENTS)i], true);
			break;
		}
		default:
			break;
		}
	}

	if (contextMatrix[st][UI_ELEMENTS::ERROR_MESSAGE] == UI_CONTEXT_STATE::ENABLED) {
		if (MessageBox(this_parent, ErrorMSG, 0, MB_ICONERROR | MB_OK) == IDOK) {
			switchState(UI_INPUT_SIGNALS::CLICK_OK_BTN);
		}
	}
}

void switchState(UI_INPUT_SIGNALS signal) {
    stateMatrix[signal][currentState]();
    currentState = transitionMatrix[signal][currentState];
    switchContext(currentState);
}

void inflateTableAttrsLb(char **items, int nItems) {
    SendMessage(
        this_llTableFieldsHwnd,
        LB_RESETCONTENT,
        0, 0L
    );

    for (int i = 0; i < nItems; i++) {
        int pos = (int) SendMessage(
            this_llTableFieldsHwnd,
            LB_ADDSTRING,
            0,
            (LPARAM) items[i]
        );

        SendMessage(this_llTableFieldsHwnd, LB_SETITEMDATA, pos, (LPARAM) i);
    }

    SetFocus(this_llTableFieldsHwnd);
}

void ResetContext() {
	attrBeenSelected = false;
	Edit_SetText(this_etTableNameHwnd, "");
	ListBox_ResetContent(this_llTableFieldsHwnd);
	Edit_SetText(this_etSelectQueryHwnd, "");
	ListView_DeleteAllItems(this_llSelectHwnd);

	if (selectedColumnsNumber > 0) {
		for (int i = 0; i < selectedColumnsNumber; i++) {
			ListView_DeleteColumn(this_llSelectHwnd, 0);
		}
	}
	else {
		for (int i = 0; i < tableColumnsNumber; i++) {
			ListView_DeleteColumn(this_llSelectHwnd, 0);
		}
	}
	tableColumnsNumber = 0;
	selectedColumnsNumber = 0;
}

void inflateLvHeader(char** titles, int columnsNumber) {
	char tmp[256];
    for (int i = 0; i < columnsNumber; i++) {
		strcpy(tmp, titles[i]);
        LV_COLUMN col;
        col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        col.cx = 100;
        col.pszText = tmp;
		col.cchTextMax = 256;
        col.iSubItem = i;
        ListView_InsertColumn(this_llSelectHwnd, i, &col);
    }
}

void tableAttrSelected() {
    // selected item index
    int lbItem = (int) SendMessage(
        this_llTableFieldsHwnd,
        LB_GETCURSEL, 0, 0
    );

    char attrName[256];
    SendMessage(
        this_llTableFieldsHwnd,
        LB_GETTEXT,
        (WPARAM) lbItem,
        (LPARAM) attrName
    );
    std::cout << "Selected table attr: " << attrName << std::endl;

    if (selectedColumnsNumber <= 0) {
        selectedColumns = (char **) malloc(sizeof(char *) * 100);
    }

    selectedColumns[selectedColumnsNumber] = (char*)
        malloc(sizeof(char) * strlen(attrName) + 1);
    strcpy(selectedColumns[selectedColumnsNumber], attrName);
    selectedColumnsNumber++;

    GetWindowText(this_etSelectQueryHwnd, buffer, 2048);
    char tmp[2048];
    if (strlen(buffer) == 0) {
        sprintf(tmp, "SELECT %s ", attrName);
    } else {
        sprintf(tmp, "%s, %s", buffer, attrName);
    }
    SetWindowText(this_etSelectQueryHwnd, tmp);
    attrBeenSelected = true;
}

void addAttributeToQuery() {
	int lbItem = (int)SendMessage(
        this_llTableFieldsHwnd,
		LB_GETCURSEL, 0, 0
	);

	char attrName[256];
	SendMessage(
        this_llTableFieldsHwnd,
		LB_GETTEXT,
		(WPARAM)lbItem,
		(LPARAM)attrName
	);

	GetWindowText(this_etSelectQueryHwnd, buffer, 2048);
	char tmp[2048];
	sprintf(tmp, "%s %s", buffer, attrName);
	SetWindowText(this_etSelectQueryHwnd, tmp);
}

void finishSelectQuery() {
  char tmp[2048];
  char tableName[256];
  GetWindowText(this_etTableNameHwnd, tableName, 256);

  if (attrBeenSelected) {
    GetWindowText(this_etSelectQueryHwnd, buffer, 2048);
    sprintf(tmp, "%s FROM %s", buffer, tableName);
  } else {
    sprintf(tmp, "SELECT * FROM %s", tableName);
  }
  SetWindowText(this_etSelectQueryHwnd, tmp);
}

void addSectionFromAndWhere() {
	char tmp[2048];
	char tableName[256];
	GetWindowText(this_etTableNameHwnd, tableName, 256);

	if (attrBeenSelected) {
		GetWindowText(this_etSelectQueryHwnd, buffer, 2048);
		sprintf(tmp, "%s FROM %s WHERE", buffer, tableName);
	}
	else {
		sprintf(tmp, "SELECT * FROM %s WHERE", tableName);
	}
	SetWindowText(this_etSelectQueryHwnd, tmp);
}

void addComboBoxItem() {
	char tmp[2048];
	char sign[10];
	GetWindowText(this_etSelectQueryHwnd, buffer, 2048);
	ComboBox_GetText(this_llComparisonSignsHwnd, sign, 10);
	sprintf(tmp, "%s %s", buffer, sign);
	SetWindowText(this_etSelectQueryHwnd, tmp);
}

bool addComparisonValue() {
	char value[256];
	char tmp[2048];
	Edit_GetText(this_etComparisonValueHwnd, value, 256);
	GetWindowText(this_etSelectQueryHwnd, buffer, 2048);
	if (strcmp(value, "") == 0) {
		sprintf(tmp, "%s %d", buffer, 0);
		SetWindowText(this_etSelectQueryHwnd, tmp);
		return false;
	}
	else {
		
		sprintf(tmp, "%s %s", buffer, value);
		SetWindowText(this_etSelectQueryHwnd, tmp);
		return true;
	}
}

void inflateSelectLvBody(
    char*** selectResult,
    int rowCount,
    int colCount
) {

	char tmp[256];
    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;
    for (int i = 0; i < rowCount; i++) {
        item.iItem = i;
        for (int j = 0; j < colCount; j++) {
            item.iSubItem = j;
			strcpy(tmp, selectResult[i][j]);
			item.pszText = tmp;
			item.cchTextMax = 256;
            ListView_InsertItem(this_llSelectHwnd, &item);
            ListView_SetItem(this_llSelectHwnd, &item);
        }
    }
}