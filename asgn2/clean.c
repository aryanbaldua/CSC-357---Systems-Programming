#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clean.h"




int countHeaderCells(char *line) {
    char *comma;
    char *position;
    int cellCount = 0;
    int i;

    comma = strchr(line, ',');
    position = line;
    while (comma) {
        cellCount++;
        i = 0;
        while (position < comma && i < MAX_LINE_LENGTH) {
            position++;
            i++;
        }

        position++;
        comma = strchr(position, ',');
    }

    cellCount++;
    while (*position != '\0' && i < MAX_LINE_LENGTH) {
        position++;
        i++;
    }

    return cellCount;
}

/*
int countHeaderCells() {
    char line[MAX_LINE_LENGTH];
    int headerCellCount = 0;

    if (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
        char *token = strtok(line, ",");
        while (token != NULL) {
            headerCellCount++;
            token = strtok(NULL, ",");
        }
    }

    return headerCellCount;
}
*/

int containsQuotationMarks(char *line) {
    if (strchr(line, '\'') != NULL || strchr(line, '\"') != NULL) {
        return 1;
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 2048

int isDigit(char c) {
    return ((c >= '0' && c <= '9' )|| c == '-');
}

int sumRow(char *line, int headerCellCount) {
    char *comma;
    char *position;
    char number[MAX_LINE_LENGTH + 1];
    int cellCount = 0;
    int sum = 0;
    int i;

    comma = strchr(line, ',');
    position = line;
    while (comma) {
        cellCount++;
        i = 0;
        while (position < comma && i < MAX_LINE_LENGTH) {
            if (isDigit(*position)) { 
                number[i] = *position;
                i++;
            }
            position++;
        }

        number[i] = '\0';
        sum += atoi(number);

        position++;
        comma = strchr(position, ',');
    }

    cellCount++;
    i = 0;
    while (*position != '\0' && i < MAX_LINE_LENGTH) {
        if (isDigit(*position)) { 
            number[i] = *position;
            i++;
        }
        position++;
    }
    number[i] = '\0';
    sum += atoi(number);

    if (cellCount != headerCellCount) {
        printf("./hist: Mismatched cells\n");
        return -1;
    }

    return sum;
}
