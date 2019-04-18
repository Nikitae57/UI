#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>

char** columnNames = nullptr;
int currentCol = 0;

char*** selectResult = nullptr;
int colNumber = 0;
int currentRow = 0;

static int selectCallback(void *NotUsed, int argc, char **argv, char **azColName) {
  if (selectResult == nullptr) {
    selectResult = (char***) malloc(sizeof(char**) * 1000);
    for (int i = 0; i < 1000; i++) {
      selectResult[i] = (char**) malloc(sizeof(char*) * argc);
      for (int j = 0; j < argc; j++) {
        selectResult[i][j] = (char*) malloc(sizeof(char) * 200);
      }
    }
    colNumber = argc;
  }

  for (int i = 0; i < argc; i++) {
    strcpy(selectResult[currentRow][i], argv[i]);
  }

  currentRow++;

  return 0;
}

static int getTableColumnsCallback(void *NotUsed, int argc, char **argv, char **azColName) {

  if (columnNames == nullptr) {
    columnNames = (char**) malloc(50 * sizeof(char*));

    for (int i = 0; i < 50; i++) {
      columnNames[i] = (char*) malloc(200 * sizeof(char));
    }
  }

  strcpy(columnNames[currentCol], argv[1]);
  currentCol += 1;

  return 0;
}

void makeSelectQueryWithoutCondition(
    char *tableName,
    char **attrs,
    char *result,
    int attrsCount
) {
  if (attrsCount == 0) {
    sprintf(result, "SELECT * FROM %s", tableName);
    return;
  }

  std::string tmp = "SELECT ";
  for (int i = 0; i < attrsCount; i++) {
    tmp += attrs[i];
    if (i == attrsCount - 1) {
      tmp += " ";
    } else {
      tmp += ", ";
    }
  }
  tmp += "FROM ";
  tmp += tableName;

  strcpy(result, tmp.c_str());
}

char ***makeSelectQuery(
    char *selectStatement,
    int *rowNumber
) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  rc = sqlite3_exec(
      db,
      selectStatement,
      selectCallback,
      0, &zErrMsg
  );
  sqlite3_close(db);

  char*** result = (char***) malloc(sizeof(char**) * (currentRow + 1));
  for (int i = 0; i < currentRow; i++) {
    result[i] = (char**) malloc(sizeof(char*) * (colNumber + 1));
    for (int j = 0; j < colNumber; j++) {
      result[i][j] = (char*)
          malloc(sizeof(char) * strlen(selectResult[i][j]) + 1);
      strcpy(result[i][j], selectResult[i][j]);
    }
  }
  *rowNumber = currentRow;

  if (selectResult != nullptr) {
    for (int i = 0; i < 1000; i++) {
      for (int j = 0; j < colNumber; j++) {
        free(selectResult[i][j]);
      }
      free(selectResult[i]);
    }
    free(selectResult);
  }
  selectResult = nullptr;
  currentRow = 0;
  colNumber = 0;

  return result;
}

char** getTableColumns(const char* tableName, int* columnNumber) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  char* query = (char*) malloc(
      sizeof(char) * (strlen("PRAGMA table_info()")
      + strlen(tableName)) + 2
  );
  sprintf(query, "PRAGMA table_info(%s)", tableName);

  rc = sqlite3_exec(db, query, getTableColumnsCallback, 0, &zErrMsg);
  sqlite3_close(db);

  std::cout << std::endl << "Table name: " << tableName << std::endl;
  std::cout << "Table attrs:" << std::endl;
  for (int i = 0; i < currentCol; i++) {
    printf("%s\n", columnNames[i]);
  }

  char** result = (char**) (malloc(sizeof(char*) * (currentCol + 1)));
  for (int i = 0; i < currentCol; i++) {
    result[i] = (char*) malloc(sizeof(char) * strlen(columnNames[i]) + 1);
    strcpy(result[i], columnNames[i]);
  }

  for (int i = 0; i < currentCol; i++) {
    free(columnNames[i]);
  }

  free(columnNames);
  columnNames = nullptr;
  *columnNumber = currentCol;
  currentCol = 0;

  return result;
}

