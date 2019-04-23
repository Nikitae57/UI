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

const int UI_ELEMENTS_COUNT = 10;
const int UI_STATES_COUNT = 15;
const int UI_INPUT_SIGNALS_COUNT = 6;

void (*stateMatrix[6][15])();
UI_STATES transitionMatrix[6][15];
char contextMatrix[15][11];

std::map<UI_ELEMENTS, HWND> elementsToHwnd;

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

    UI_ELEMENTS i = UI_ELEMENTS::SELECT_RESULT_TABLE;
    UI_ELEMENTS  lastElement = UI_ELEMENTS::COMPARISON_DROP_LIST;
	for (; i <= lastElement; i++) {

	}

	initStateMatrix();
	initTransitionMatrix();
	initContextMatrix()

}

void initStateMatrix() {
    stateMatrix[UI_INPUT_SIGNALS::CLICK_MENU_TABLE][UI_STATES::MODE_SELECTION_1] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_MENU_QUERY][UI_STATES::MODE_SELECTION_1] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::ENTER_KEY_PRESSED][UI_STATES::TABLE_SELECTION_2] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTRS_SELECTION_3] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::TABLE_ATTRS_SELECTION_3] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::ERROR_MSG_4] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::QUERY_RESULT_OUTPUT_5] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::ENTER_KEY_PRESSED][UI_STATES::TABLE_SELECTION_6] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTRS_SELECTION_7] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::TABLE_ATTRS_SELECTION_7] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::TABLE_ATTRS_SELECTION_7] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::ERROR_MSG_8] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::QUERY_RESULT_OUTPUT_9] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::DOUBLE_CLICK_ATTR][UI_STATES::TABLE_ATTR_SELECTION_10] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::COMPARISON_SIGN_SELECTION_11] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::COMPARISON_VALUE_INPUT_12] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_OK_BTN][UI_STATES::LOGICAL_OPERATION_SELECTION_13] = []() {

    };

    stateMatrix[UI_INPUT_SIGNALS::CLICK_NEXT_BTN][UI_STATES::LOGICAL_OPERATION_SELECTION_13] = []() {

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
    strcpy(contextMatrix[UI_STATES::START_0], "0000000000");
    strcpy(contextMatrix[UI_STATES::MODE_SELECTION_1], "2000000000");
    strcpy(contextMatrix[UI_STATES::TABLE_SELECTION_2], "1012110000");
    strcpy(contextMatrix[UI_STATES::TABLE_ATTRS_SELECTION_3], "1021120000");
    strcpy(contextMatrix[UI_STATES::ERROR_MSG_4], "1011110002");
    strcpy(contextMatrix[UI_STATES::QUERY_RESULT_OUTPUT_5], "1111120000");
    strcpy(contextMatrix[UI_STATES::TABLE_SELECTION_6], "1012111110");
    strcpy(contextMatrix[UI_STATES::TABLE_ATTRS_SELECTION_7], "1021122110");
    strcpy(contextMatrix[UI_STATES::ERROR_MSG_8], "1011111112");
    strcpy(contextMatrix[UI_STATES::QUERY_RESULT_OUTPUT_9], "1111121110");
    strcpy(contextMatrix[UI_STATES::TABLE_ATTR_SELECTION_10], "1021112110");
    strcpy(contextMatrix[UI_STATES::COMPARISON_SIGN_SELECTION_11], "1011112120");
    strcpy(contextMatrix[UI_STATES::COMPARISON_VALUE_INPUT_12], "1011112210");
    strcpy(contextMatrix[UI_STATES::LOGICAL_OPERATION_SELECTION_13], "1011122110");
    strcpy(contextMatrix[UI_STATES::FINISH_14], "0000000000");
}

void switchContext(UI_STATES st) {
    for (int i = 0; i < )
}