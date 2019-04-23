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
  for (int i = 0; i < state.visibleCount; i++) {
    ShowWindow(state.visibles[i], SW_SHOW);
  }

  for (int i = 0; i < state.invisibleCount; i++) {
    ShowWindow(state.invisibles[i], SW_HIDE);
  }

  if (state.funPtr != NULL) {
	  state.funPtr();
  }
}

SELECT_MODE_STATE_1::SELECT_MODE_STATE_1() : STATE() {
    invisibles[0] = this_etTableNameHwnd;
    invisibles[1] = this_llTableFieldsHwnd;
    invisibles[2] = this_etSelectQueryHwnd;
    invisibles[3] = this_llSelectHwnd;
    invisibles[4] = this_btnOkHwnd;
    invisibles[5] = this_btnNextHwnd;
    invisibles[6] = this_etComparisonValueHwnd;
    invisibles[7] = this_llComparisonSignsHwnd;

    invisibleCount = 8;
    visibleCount = 0;

	funPtr = []() {
		EnableMenuItem(this_hMenu, 0, MF_ENABLED | MF_BYPOSITION);
		DrawMenuBar(this_parent);
	};
}

SELECT_TABLE_STATE_2::SELECT_TABLE_STATE_2() : STATE() {
    invisibles[0] = this_llSelectHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;
    invisibles[3] = this_btnNextHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;

    invisibleCount = 4;
    visibleCount = 4;

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
    invisibles[0] = this_llSelectHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;
    invisibles[3] = this_btnNextHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;

    invisibleCount = 4;
    visibleCount = 4;

	funPtr = []() {};
}

FAILED_CONNECT_STATE_4::FAILED_CONNECT_STATE_4() : STATE() {
    invisibles[0] = this_llSelectHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;
    invisibles[3] = this_btnNextHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;

    invisibleCount = 4;
    visibleCount = 4;

	funPtr = []() {
		MessageBox(this_parent, "Таблица не найдена", NULL, MB_ICONERROR | MB_OK);
	};
}

SELECT_ATTRIBUTES_5::SELECT_ATTRIBUTES_5() : STATE() {
    invisibles[0] = this_llSelectHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;
    invisibles[3] = this_btnNextHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;

    invisibleCount = 4;
    visibleCount = 4;

	funPtr = []() {
		Button_Enable(this_btnOkHwnd, true);
		Edit_Enable(this_etTableNameHwnd, false);
	};
}

CONNECT_TO_BD_STATE_6::CONNECT_TO_BD_STATE_6() : STATE() {
    invisibles[0] = this_llSelectHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;
    invisibles[3] = this_btnNextHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;

    invisibleCount = 4;
    visibleCount = 4;

	funPtr = []() {};
}

FAILED_CONNECT_STATE_7::FAILED_CONNECT_STATE_7() : STATE() {
    invisibles[0] = this_llSelectHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;
    invisibles[3] = this_btnNextHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;

    invisibleCount = 4;
    visibleCount = 4;

	funPtr = []() {
		MessageBox(this_parent, "Ошибка при выполнении запроса", NULL, MB_ICONERROR | MB_OK);
	};
}

OUTPUT_DATA_STATE_8::OUTPUT_DATA_STATE_8() : STATE() {
    invisibles[0] = this_btnNextHwnd;
    invisibles[1] = this_etComparisonValueHwnd;
    invisibles[2] = this_llComparisonSignsHwnd;

    visibles[0] = this_btnOkHwnd;
    visibles[1] = this_etTableNameHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_llSelectHwnd;

    invisibleCount = 3;
    visibleCount = 5;

	funPtr = []() {};
}

SELECT_TABLE_STATE_9::SELECT_TABLE_STATE_9() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

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
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {};
}

FAILED_CONNECT_STATE_11::FAILED_CONNECT_STATE_11() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {
		MessageBox(this_parent, "Таблица не найдена", NULL, MB_ICONERROR | MB_OK);
	};
}

SELECT_ATTRIBUTES_12::SELECT_ATTRIBUTES_12() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {
		Button_Enable(this_btnOkHwnd, true);
		Button_Enable(this_btnNextHwnd, true);
		Edit_Enable(this_etTableNameHwnd, false);
		ListBox_Enable(this_llTableFieldsHwnd, true);
	};
}

CONNECT_TO_BD_STATE_13::CONNECT_TO_BD_STATE_13() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {
		Button_Enable(this_btnNextHwnd, true);
		Edit_Enable(this_etComparisonValueHwnd, false);
		ListBox_Enable(this_llTableFieldsHwnd, false);
		ComboBox_Enable(this_llComparisonSignsHwnd, false);
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
	};
}

FAILED_CONNECT_STATE_14::FAILED_CONNECT_STATE_14() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {
		MessageBox(this_parent, "Ошибка при выполнении запроса", NULL, MB_ICONERROR | MB_OK);
	};
}

OUTPUT_DATA_STATE_15::OUTPUT_DATA_STATE_15() : STATE() {
    visibles[0] = this_etTableNameHwnd;
    visibles[1] = this_llTableFieldsHwnd;
    visibles[2] = this_etSelectQueryHwnd;
    visibles[3] = this_llSelectHwnd;
    visibles[4] = this_btnOkHwnd;
    visibles[5] = this_btnNextHwnd;
    visibles[6] = this_etComparisonValueHwnd;
    visibles[7] = this_llComparisonSignsHwnd;

    invisibleCount = 0;
    visibleCount = 8;

	funPtr = []() {
		Button_Enable(this_btnNextHwnd, false);
	};
}

SELECT_ATTRIBUTE_STATE_16::SELECT_ATTRIBUTE_STATE_16() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {
		ListBox_Enable(this_llTableFieldsHwnd, true);
		Button_Enable(this_btnOkHwnd, false); 
		Button_Enable(this_btnNextHwnd, false);
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
	};
}

SELECT_COMPARISON_SIGN_STATE_17::SELECT_COMPARISON_SIGN_STATE_17() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

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
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

	funPtr = []() {
		ComboBox_Enable(this_llComparisonSignsHwnd, false);
		Edit_Enable(this_etComparisonValueHwnd, true);
		Button_Enable(this_btnNextHwnd, true);
		ComboBox_ResetContent(this_llComparisonSignsHwnd);
	};
}

SELECT_LOGICAL_CONDITION_STATE_19::SELECT_LOGICAL_CONDITION_STATE_19() : STATE() {
    invisibles[0] = this_llSelectHwnd;

    visibles[1] = this_etTableNameHwnd;
    visibles[0] = this_btnOkHwnd;
    visibles[2] = this_llTableFieldsHwnd;
    visibles[3] = this_etSelectQueryHwnd;
    visibles[4] = this_btnNextHwnd;
    visibles[5] = this_llComparisonSignsHwnd;
    visibles[6] = this_etComparisonValueHwnd;

    invisibleCount = 1;
    visibleCount = 7;

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







