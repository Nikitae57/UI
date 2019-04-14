#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>

char** columnNames = nullptr;
int currentCol = 0;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");

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

char** getTableColumns(const char* tableName, int* colNumber) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  char* query = (char*) malloc(sizeof(char) * (strlen("PRAGMA table_info()") + strlen(tableName)));
  sprintf(query, "PRAGMA table_info(%s)", tableName);

  rc = sqlite3_exec(db, query, getTableColumnsCallback, 0, &zErrMsg);
  sqlite3_close(db);

  for (int i = 0; i < currentCol; i++) {
    printf("%s\n", columnNames[i]);
  }

  char** result = (char**) (malloc(sizeof(char*) * currentCol));
  for (int i = 0; i < currentCol; i++) {
    result[i] = (char*) malloc(sizeof(char) * strlen(columnNames[i]));
    strcpy(result[i], columnNames[i]);
  }

  for (int i = 0; i < currentCol; i++) {
    free(columnNames[i]);
  }
  free(columnNames);
  columnNames = nullptr;
  *colNumber = currentCol;
  currentCol = 0;

  return result;
}

