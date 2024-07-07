/* Defines functions for converting characters between tabs and spaces.
 * CSC 357, Assignment 1
 * Given code, Spring '24. */
#include <stdio.h>
#include "chars.h"

/* istext: Checks whether or not a character is plain text.
 * NOTE: Do not alter this function. It returns 1 if "c" has an ASCII code in
 *       the ranges [0x08, 0x0D] or [0x20, 0x7E], and 0 otherwise. */
int istext(char c) {
    return (c >= 0x08 && c <= 0x0D) || (c >= 0x20 && c <= 0x7E);
}

/* ttos: Converts a character from tabs to spaces.
 * TODO: Implement this function. It should return the number of characters
 *       placed into "str": 1 to 8 spaces if "next" is a tab (depending on the
 *       current line length), and "next" otherwise. Use "dflt" instead if
 *       "next" is not plain text, and do nothing if neither is plain text.
 *       See also the given unit tests. */

int ttos(char next, char str[], char dflt) {
    static int tabCount = 0;
    static int spaceCount = 0;
    int i;

    /* check if next is tab or if dflt is tab when next is not a printable character */
    if (next == '\t' || (!istext(next) && dflt == '\t')) {
        spaceCount = 8 - (tabCount % 8);
        for (i = 0; i < spaceCount; i++) {
            str[i] = ' ';
        }
        str[i] = '\0';
        tabCount += spaceCount;
        return i;
    }
    /* check if next is a printable character*/
    if (istext(next)){
        str[0] = next;
        str[1] = '\0';
        if (next == '\n') {
            tabCount = 0;
        }
        else{
            tabCount = tabCount + 1;
        }
        return 1;
    }
    /* check if dflt is a printable character when next is not  a printable character */
    if (!istext(next) && istext(dflt)) {
        str[0] = dflt;
        str[1] = '\0';
        if (dflt == '\n') {
            tabCount = 0;
        }
        else{
            tabCount = tabCount + 1;
        }
        return 1;
    }
    /* when both next and dflt are not printable characters */
    str[0] = '\0';
    return 0;
}


/* stot: Converts a character from spaces to tabs.
 * TODO: Implement this function. It should return the number of characters
 *       placed into "str": a tab or nothing if "next" is a space (depending on
 *       the current line length), and 0 to 7 spaces followed by "next"
 *       otherwise. Use "dflt" instead if "next" is not plain text, and do
 *       nothing if neither is plain text. See also the given unit tests. */

int stot(char next, char str[], char dflt) {
    static int tabCount = 0;
    static int spaceCount = 0;
    int i = 0;

    /*
    printf("Next is: %c, Defualt is: %c, Space count is: %d, Column is: %d \n", next, dflt, spaceCount, tabCount);
    */

    /* when both next and dflt are not printable characters */
    if (!istext(next) && !istext(dflt)){
        str[i] = '\0';
        return 0;
    }
    /* check if next is a space or if dflt is space when next is not a printable character */
    if (next == ' ' || (!istext(next) && dflt == ' ')) {
        spaceCount++;
        tabCount++;
    
        if ((spaceCount == 8) || ((tabCount % 8) == 0)) {
            str[i] = '\t';
            i++;
            spaceCount = 0;
            tabCount = 0;
        }
        str[i] = '\0';
        return i;
    } 
    /* whenever either next or dflt is a printable character */
    if (istext(next) || istext(dflt)) {
        while (spaceCount > 0 && spaceCount < 8) {
            str[i] = ' ';
            i++;
            spaceCount--;
        }
    }
    /* check if next is a tab or if dflt is tab when next is not a printable character */
    if (next == '\t' || (!istext(next) && dflt == '\t')) {
        str[i] = '\t';
        tabCount = 0;
    } 
    else {
        if (!istext(next)){
            str[i] = dflt;
        }
        else{
            str[i] = next;
        }
        if (next == '\n' || (!istext(next) && dflt == '\n')){
            tabCount = 0;
        }
        else{
            tabCount++;
        }
    }
    spaceCount = 0;
    str[i + 1] = '\0';
    return i + 1;
       
}


/*
int main(void) {
    int z;

    z = istext('\0');
    printf("this is %d", z);
    
    
    return 0;
}
*/
