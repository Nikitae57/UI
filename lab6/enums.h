enum UI_ELEMENTS {
    MENU, SELECT_RESULT_TABLE, TABLE_ATTRS_LIST,
    TABLE_NAME_EDIT, SQL_STATEMENT_EDIT, OK_BTN,
    NEXT_BTN, COMPARISON_EDIT, COMPARISON_DROP_LIST,

    ARCHIVE_LISTBOX, BTN_ARCHIVE, BTN_CANCEL, BTN_UP, BTN_DOWN,

    ERROR_MESSAGE
};

enum UI_STATES {
    START_0,

    MODE_SELECTION_1, TABLE_SELECTION_2, TABLE_ATTRS_SELECTION_3,
    ERROR_MSG_4, QUERY_RESULT_OUTPUT_5,

    TABLE_SELECTION_6, TABLE_ATTRS_SELECTION_7, ERROR_MSG_8,
    QUERY_RESULT_OUTPUT_9, TABLE_ATTR_SELECTION_10,
    COMPARISON_SIGN_SELECTION_11, COMPARISON_VALUE_INPUT_12,
    LOGICAL_OPERATION_SELECTION_13,

    ARCHIVE_OPENED_14, ARCHIVE_ITEM_SELECTED_15, ARCHIVE_SELECT_RESULT_OUTPUT_16,

    FINISH_17
};

enum UI_INPUT_SIGNALS {
    CLICK_MENU_TABLE, CLICK_MENU_QUERY, ENTER_KEY_PRESSED,
    DOUBLE_CLICK_ATTR, CLICK_OK_BTN, CLICK_NEXT_BTN,

    CLICK_ARCHIVE_BTN, CLICK_UP_BTN, CLICK_DOWN_BTN, CLICK_CANCEL_BTN
};

enum UI_CONTEXT_STATE {
    INVISIBLE, DISABLED, ENABLED
};