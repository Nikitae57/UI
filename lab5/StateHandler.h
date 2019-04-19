#ifndef LAB5_STATEHANDLER_H
#define LAB5_STATEHANDLER_H

#include <windows.h>

struct STATE {
  bool tableNameVisible;
  bool tableAttrsVisible;
  bool okBtnVisible;
  bool selectResultVisible;
  bool sqlStatementVisible;

  bool comparisonValueVisible;
  bool comparisonSignsVisible;
  bool nextBtnVisible;

  void funPtr() {};
};

void initStateHandler(
    HWND etTableName,
    HWND llTableFields,
    HWND etSelectQuery,
    HWND llSelect,
    HWND btnOk,

    HWND btnNext,
    HWND etComparisonValue,
    HWND llComparisonSigns
);

void switchState(STATE state);

struct FIRST_STATE : STATE {
 public:
  FIRST_STATE() : STATE() {
    tableNameVisible = false;
    tableAttrsVisible = false;
    okBtnVisible = false;
    selectResultVisible = false;
    sqlStatementVisible = false;

    comparisonValueVisible = false;
    comparisonSignsVisible = false;
    nextBtnVisible = false;
  }

  void funPtr() {}
};

#endif //LAB5_STATEHANDLER_H
