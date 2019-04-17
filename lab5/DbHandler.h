#ifndef LAB5_DBHANDLER_H
#define LAB5_DBHANDLER_H

char** getTableColumns(const char* tableName, int* colNumber);
void makeSelectQueryWithoutCondition(
    char *tableName,
    char **attrs,
    char *result,
    int attrsCount
);

char ***makeSelectQuery(
    char *selectStatement,
    int *rowNumber
);
#endif //LAB5_DBHANDLER_H
