/* Converts characters from tabs to spaces.
 * CSC 357, Assignment 1
 * Given code, Spring '24 */

#include <stdio.h>
#include "chars.h"

int main(void) {
    char next, str[9];

    while (scanf("%c", &next) != EOF) {
        /* TODO: Call "ttos" here, then print the result. */
        ttos(next, str, '\0');
        printf("%s", str);
    }

    return 0;
}
