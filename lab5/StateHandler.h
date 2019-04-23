#ifndef LAB5_STATEHANDLER_H
#define LAB5_STATEHANDLER_H

#include <windows.h>

struct STATE {
  void (*funPtr)();

  HWND visibles[8];
  HWND invisibles[8];
  int visibleCount;
  int invisibleCount;
};

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
);

void switchState(STATE state);

struct SELECT_MODE_STATE_1 : STATE {
 public:
	 SELECT_MODE_STATE_1();
};

struct SELECT_TABLE_STATE_2 : STATE {
public:
	SELECT_TABLE_STATE_2();
};

struct CONNECT_TO_BD_STATE_3 : STATE {
public:
	CONNECT_TO_BD_STATE_3();
};

struct FAILED_CONNECT_STATE_4 : STATE {
public:
	FAILED_CONNECT_STATE_4();
};

struct SELECT_ATTRIBUTES_5 : STATE {
public:
	SELECT_ATTRIBUTES_5();
};

struct CONNECT_TO_BD_STATE_6 : STATE {
public:
	CONNECT_TO_BD_STATE_6();
};

struct FAILED_CONNECT_STATE_7 : STATE {
public:
	FAILED_CONNECT_STATE_7();
};

struct OUTPUT_DATA_STATE_8 : STATE {
public:
	OUTPUT_DATA_STATE_8();
};

struct SELECT_TABLE_STATE_9 : STATE {
public:
	SELECT_TABLE_STATE_9();
};

struct CONNECT_TO_BD_STATE_10 : STATE {
public:
	CONNECT_TO_BD_STATE_10();
};

struct FAILED_CONNECT_STATE_11 : STATE {
public:
	FAILED_CONNECT_STATE_11();
};

struct SELECT_ATTRIBUTES_12 : STATE {
public:
	SELECT_ATTRIBUTES_12();
};

struct CONNECT_TO_BD_STATE_13 : STATE {
public:
	CONNECT_TO_BD_STATE_13();
};

struct FAILED_CONNECT_STATE_14 : STATE {
public:
	FAILED_CONNECT_STATE_14();
};

struct OUTPUT_DATA_STATE_15 : STATE {
public:
	OUTPUT_DATA_STATE_15();
};

struct SELECT_ATTRIBUTE_STATE_16 : STATE {
public:
	SELECT_ATTRIBUTE_STATE_16();
};

struct SELECT_COMPARISON_SIGN_STATE_17 : STATE {
public:
	SELECT_COMPARISON_SIGN_STATE_17();
};

struct SELECT_COMPARISON_VALUE_STATE_18 : STATE {
public:
	SELECT_COMPARISON_VALUE_STATE_18();
};

struct SELECT_LOGICAL_CONDITION_STATE_19 : STATE {
public:
	SELECT_LOGICAL_CONDITION_STATE_19();
};


#endif //LAB5_STATEHANDLER_H
