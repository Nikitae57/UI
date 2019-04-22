#include "StateHandler.h"
#include "windows.h"
#include <windowsx.h>
#include <commctrl.h>

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
	this_etTableNameHwnd = etTableName;
	this_llTableFieldsHwnd = llTableFields;
	this_etSelectQueryHwnd = etSelectQuery;
	this_llSelectHwnd = llSelect;
	this_btnOkHwnd = btnOk;

	this_btnNextHwnd = btnNext;
	this_etComparisonValueHwnd = etComparisonValue;
	this_llComparisonSignsHwnd = llComparisonSigns;
	this_hMenu = hMenu;
}

void switchState(STATE state) {
  if (state.tableNameVisible) {
    ShowWindow(this_etTableNameHwnd, SW_SHOW);
  } else {
    ShowWindow(this_etTableNameHwnd, SW_HIDE);
  }

  if (state.tableAttrsVisible) {
    ShowWindow(this_llTableFieldsHwnd, SW_SHOW);
  } else {
    ShowWindow(this_llTableFieldsHwnd, SW_HIDE);
  }

  if (state.okBtnVisible) {
    ShowWindow(this_btnOkHwnd, SW_SHOW);
  } else {
    ShowWindow(this_btnOkHwnd, SW_HIDE);
  }

  if (state.selectResultVisible) {
    ShowWindow(this_llSelectHwnd, SW_SHOW);
  } else {
    ShowWindow(this_llSelectHwnd, SW_HIDE);
  }

  if (state.sqlStatementVisible) {
    ShowWindow(this_etSelectQueryHwnd, SW_SHOW);
  } else {
    ShowWindow(this_etSelectQueryHwnd, SW_HIDE);
  }

  if (state.comparisonValueVisible) {
    ShowWindow(this_etComparisonValueHwnd, SW_SHOW);
  } else {
    ShowWindow(this_etComparisonValueHwnd, SW_HIDE);
  }

  if (state.comparisonSignsVisible) {
    ShowWindow(this_llComparisonSignsHwnd, SW_SHOW);
  } else {
    ShowWindow(this_llComparisonSignsHwnd, SW_HIDE);
  }

  if (state.nextBtnVisible) {
    ShowWindow(this_btnNextHwnd, SW_SHOW);
  } else {
    ShowWindow(this_btnNextHwnd, SW_HIDE);
  }

  if (state.funPtr != NULL) {
	  state.funPtr();
  }
}

SELECT_MODE_STATE_1::SELECT_MODE_STATE_1() : STATE() {
	tableNameVisible = false;
	tableAttrsVisible = false;
	okBtnVisible = false;
	selectResultVisible = false;
	sqlStatementVisible = false;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {
		EnableMenuItem(this_hMenu, 0, MF_ENABLED | MF_BYPOSITION);
		DrawMenuBar(this_parent);
	};
}

SELECT_TABLE_STATE_2::SELECT_TABLE_STATE_2() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {
		EnableMenuItem(this_hMenu, 0, MF_GRAYED | MF_BYPOSITION);
		DrawMenuBar(this_parent);
		Edit_SetText(this_etTableNameHwnd, "");
		Edit_SetText(this_etSelectQueryHwnd, "");
		Edit_SetText(this_etComparisonValueHwnd, "");
		Button_Enable(this_btnOkHwnd, false);
		Edit_Enable(this_etTableNameHwnd, true);
		ListBox_Enable(this_llTableFieldsHwnd, true);
		ListBox_ResetContent(this_llTableFieldsHwnd);
		ListView_DeleteAllItems(this_llSelectHwnd);
	};
}

CONNECT_TO_BD_STATE_3::CONNECT_TO_BD_STATE_3() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {};
}

FAILED_CONNECT_STATE_4::FAILED_CONNECT_STATE_4() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {
		MessageBox(this_parent, "Таблица не найдена", NULL, MB_ICONERROR | MB_OK);
	};
}

SELECT_ATTRIBUTES_5::SELECT_ATTRIBUTES_5() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {
		Button_Enable(this_btnOkHwnd, true);
		Edit_Enable(this_etTableNameHwnd, false);
	};
}

CONNECT_TO_BD_STATE_6::CONNECT_TO_BD_STATE_6() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {};
}

FAILED_CONNECT_STATE_7::FAILED_CONNECT_STATE_7() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {
		MessageBox(this_parent, "Ошибка при выполнении запроса", NULL, MB_ICONERROR | MB_OK);
	};
}

OUTPUT_DATA_STATE_8::OUTPUT_DATA_STATE_8() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = true;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {};
}

SELECT_TABLE_STATE_9::SELECT_TABLE_STATE_9() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		EnableMenuItem(this_hMenu, 0, MF_GRAYED | MF_BYPOSITION);
		DrawMenuBar(this_parent);
		Edit_SetText(this_etTableNameHwnd, "");
		Edit_SetText(this_etSelectQueryHwnd, "");
		Edit_SetText(this_etComparisonValueHwnd, "");
		Button_Enable(this_btnOkHwnd, false); 
		Button_Enable(this_btnNextHwnd, false);
		Edit_Enable(this_etTableNameHwnd, true);
		Edit_Enable(this_etComparisonValueHwnd, false);
		ComboBox_Enable(this_llComparisonSignsHwnd, false);
		ListBox_Enable(this_llTableFieldsHwnd, true);
		ListBox_ResetContent(this_llTableFieldsHwnd);
		ListView_DeleteAllItems(this_llSelectHwnd);
	};
}

CONNECT_TO_BD_STATE_10::CONNECT_TO_BD_STATE_10() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {};
}

FAILED_CONNECT_STATE_11::FAILED_CONNECT_STATE_11() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		MessageBox(this_parent, "Таблица не найдена", NULL, MB_ICONERROR | MB_OK);
	};
}

SELECT_ATTRIBUTES_12::SELECT_ATTRIBUTES_12() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		Button_Enable(this_btnOkHwnd, true);
		Button_Enable(this_btnNextHwnd, true);
		Edit_Enable(this_etTableNameHwnd, false);
		ListBox_Enable(this_llTableFieldsHwnd, true);
	};
}

CONNECT_TO_BD_STATE_13::CONNECT_TO_BD_STATE_13() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		Button_Enable(this_btnNextHwnd, true);
		Edit_Enable(this_etComparisonValueHwnd, false);
		ListBox_Enable(this_llTableFieldsHwnd, false);
		ComboBox_Enable(this_llComparisonSignsHwnd, false);
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
	};
}

FAILED_CONNECT_STATE_14::FAILED_CONNECT_STATE_14() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = false;
	comparisonSignsVisible = false;
	nextBtnVisible = false;

	funPtr = []() {
		MessageBox(this_parent, "Ошибка при выполнении запроса", NULL, MB_ICONERROR | MB_OK);
	};
}

OUTPUT_DATA_STATE_15::OUTPUT_DATA_STATE_15() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = true;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		Button_Enable(this_btnNextHwnd, false);
	};
}

SELECT_ATTRIBUTE_STATE_16::SELECT_ATTRIBUTE_STATE_16() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		ListBox_Enable(this_llTableFieldsHwnd, true);
		Button_Enable(this_btnOkHwnd, false); 
		Button_Enable(this_btnNextHwnd, false);
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
	};
}

SELECT_COMPARISON_SIGN_STATE_17::SELECT_COMPARISON_SIGN_STATE_17() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		ListBox_Enable(this_llTableFieldsHwnd, false);
		ComboBox_Enable(this_llComparisonSignsHwnd, true);
		Button_Enable(this_btnNextHwnd, true);

		ComboBox_ResetContent(this_llComparisonSignsHwnd);
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT(">"));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT(">="));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("<"));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("<="));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("="));
		ComboBox_SetCurSel(this_llComparisonSignsHwnd, 0);
	};
}

SELECT_COMPARISON_VALUE_STATE_18::SELECT_COMPARISON_VALUE_STATE_18() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		ComboBox_Enable(this_llComparisonSignsHwnd, false);
		Edit_Enable(this_etComparisonValueHwnd, true);
		Button_Enable(this_btnNextHwnd, true);
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
	};
}

SELECT_LOGICAL_CONDITION_STATE_19::SELECT_LOGICAL_CONDITION_STATE_19() : STATE() {
	tableNameVisible = true;
	tableAttrsVisible = true;
	okBtnVisible = true;
	selectResultVisible = false;
	sqlStatementVisible = true;

	comparisonValueVisible = true;
	comparisonSignsVisible = true;
	nextBtnVisible = true;

	funPtr = []() {
		ComboBox_Enable(this_llComparisonSignsHwnd, true);
		Edit_Enable(this_etComparisonValueHwnd, false);
		Button_Enable(this_btnNextHwnd, true);
		Button_Enable(this_btnOkHwnd, true);

		Edit_SetText(this_etComparisonValueHwnd, "");
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("AND"));
		ComboBox_AddString(this_llComparisonSignsHwnd, TEXT("OR"));
		ComboBox_SetCurSel(this_llComparisonSignsHwnd, 0);
	};
}







