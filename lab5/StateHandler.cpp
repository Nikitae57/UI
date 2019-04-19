#include "StateHandler.h"
#include "windows.h"

HWND this_etTableNameHwnd;
HWND this_llTableFieldsHwnd;
HWND this_etSelectQueryHwnd;
HWND this_llSelectHwnd;
HWND this_btnOkHwnd;

HWND this_btnNextHwnd;
HWND this_etComparisonValueHwnd;
HWND this_llComparisonSignsHwnd;

void initStateHandler(
    HWND etTableName,
    HWND llTableFields,
    HWND etSelectQuery,
    HWND llSelect,
    HWND btnOk,

    HWND btnNext,
    HWND etComparisonValue,
    HWND llComparisonSigns
) {
  this_etTableNameHwnd = etTableName;
  this_llTableFieldsHwnd = llTableFields;
  this_etSelectQueryHwnd = etSelectQuery;
  this_llSelectHwnd = llSelect;
  this_btnOkHwnd = btnOk;

  this_btnNextHwnd = btnNext;
  this_etComparisonValueHwnd = etComparisonValue;
  this_llComparisonSignsHwnd = llComparisonSigns;
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

  state.funPtr();
}





