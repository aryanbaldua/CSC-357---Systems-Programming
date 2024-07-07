/* Converts characters from spaces to tabs.
 * CSC 357, Assignment 1
 * Given code, Spring '24 */

#include <stdio.h>
#include "chars.h"

int main(void) {
    char next, str[9];

    while (scanf("%c", &next) != EOF) {
        /* TODO: Call "stot" here, then print the result. */
        stot(next, str, '\0');
        printf("%s", str);
    }

    return 0;
}
