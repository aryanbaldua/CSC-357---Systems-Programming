/* Tests functions for converting characters between tabs and spaces.
 * CSC 357, Assignment 1
 * Given tests, Spring '24. */

#include <stdio.h>
#include <assert.h>
#include "chars.h"

/* test01: Tests converting trivial tabs and spaces. */
void test01(void) {
    char str[9];

    /* NOTE: Tabs convert to enough spaces to reach the next tabstop, so one
     *       standalone tab can immediately be converted into eight spaces. */

    assert(ttos('\t', str, '?') == 8);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == ' ');
    assert(str[7] == ' ');
    assert(str[8] == '\0');

    /* NOTE: Spaces must reach the next tabstop to convert into a tab, so eight
     *       standalone spaces can eventually be converted into one tab. 
    */
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');
    
}

/* test02: Tests converting a tab into spaces. */
void test02(void) {
    char str[9];

    /* NOTE: One tab following one character converts into seven spaces. */

    assert(ttos('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(ttos('\t', str, '?') == 7);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == ' ');
    assert(str[7] == '\0');
}

/* test03: Tests converting multiple tabs into spaces. */
void test03(void) {
    char str[9];

    /* NOTE: One tab following two characters converts into six spaces. */

    assert(ttos('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(ttos('b', str, '?') == 1);
    assert(str[0] == 'b');
    assert(str[1] == '\0');
    assert(ttos('\t', str, '?') == 6);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == '\0');

    /* NOTE: One tab following nine characters converts into seven spaces. */

    assert(ttos('c', str, '?') == 1);
    assert(str[0] == 'c');
    assert(str[1] == '\0');
    assert(ttos('d', str, '?') == 1);
    assert(str[0] == 'd');
    assert(str[1] == '\0');
    assert(ttos('e', str, '?') == 1);
    assert(str[0] == 'e');
    assert(str[1] == '\0');
    assert(ttos('f', str, '?') == 1);
    assert(str[0] == 'f');
    assert(str[1] == '\0');
    assert(ttos('g', str, '?') == 1);
    assert(str[0] == 'g');
    assert(str[1] == '\0');
    assert(ttos('h', str, '?') == 1);
    assert(str[0] == 'h');
    assert(str[1] == '\0');
    assert(ttos('i', str, '?') == 1);
    assert(str[0] == 'i');
    assert(str[1] == '\0');
    assert(ttos('j', str, '?') == 1);
    assert(str[0] == 'j');
    assert(str[1] == '\0');
    assert(ttos('k', str, '?') == 1);
    assert(str[0] == 'k');
    assert(str[1] == '\0');
    assert(ttos('\t', str, '?') == 7);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == ' ');
    assert(str[7] == '\0');
}

/* test04: Tests converting tabs with whitespace into spaces. */
void test04(void) {
    char str[9];

    /* NOTE: Newlines reset the current line length. */

    assert(ttos('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(ttos('\n', str, '?') == 1);
    assert(str[0] == '\n');
    assert(str[1] == '\0');

    /* NOTE: Existing spaces are no different from ordinary characters. */

    assert(ttos(' ', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos(' ', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos('b', str, '?') == 1);
    assert(str[0] == 'b');
    assert(str[1] == '\0');

    /* NOTE: One tab following seven characters converts into one space. */

    assert(ttos(' ', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos(' ', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos(' ', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos(' ', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos('\t', str, '?') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');

    /* NOTE: One tab following a tabstop converts into eight spaces. */

    assert(ttos('\t', str, '?') == 8);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == ' ');
    assert(str[7] == ' ');
    assert(str[8] == '\0');
}

/* test05: Tests converting tabs with unprintables into spaces. */
void test05(void) {
    char str[9];

    /* NOTE: An unprintable that defaults to an ordinary character counts as an
     *       ordinary character; an unprintable that defaults to a newline
     *       resets the current line length. */

    assert(ttos('\0', str, '?') == 1);
    assert(str[0] == '?');
    assert(str[1] == '\0');
    assert(ttos('\0', str, '\n') == 1);
    assert(str[0] == '\n');
    assert(str[1] == '\0');

    /* NOTE: An unprintable that defaults to another unprintable does nothing
     *       and does not affect the current line length; an unprintable that
     *       defaults to a tab converts into spaces. 
    */
    assert(ttos('a', str, '\0') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(ttos('\0', str, ' ') == 1);
    assert(str[0] == ' ');
    assert(str[1] == '\0');
    assert(ttos('\0', str, '\0') == 0);
    assert(str[0] == '\0');
    assert(ttos('\0', str, '\t') == 6);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == '\0');
    
}

/* test06: Tests converting spaces into a tab. */
void test06(void) {
    char str[9];

    /* NOTE: Seven spaces following one character convert into one tab. */

    assert(stot('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');
}

/* test07: Tests converting spaces into multiple tabs. */
void test07(void) {
    char str[9];

    /* NOTE: Six spaces following two characters convert into one tab. */

    assert(stot('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(stot('b', str, '?') == 1);
    assert(str[0] == 'b');
    assert(str[1] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');

    /* NOTE: Seven spaces following nine characters convert into one tab. */

    assert(stot('c', str, '?') == 1);
    assert(str[0] == 'c');
    assert(str[1] == '\0');
    assert(stot('d', str, '?') == 1);
    assert(str[0] == 'd');
    assert(str[1] == '\0');
    assert(stot('e', str, '?') == 1);
    assert(str[0] == 'e');
    assert(str[1] == '\0');
    assert(stot('f', str, '?') == 1);
    assert(str[0] == 'f');
    assert(str[1] == '\0');
    assert(stot('g', str, '?') == 1);
    assert(str[0] == 'g');
    assert(str[1] == '\0');
    assert(stot('h', str, '?') == 1);
    assert(str[0] == 'h');
    assert(str[1] == '\0');
    assert(stot('i', str, '?') == 1);
    assert(str[0] == 'i');
    assert(str[1] == '\0');
    assert(stot('j', str, '?') == 1);
    assert(str[0] == 'j');
    assert(str[1] == '\0');
    assert(stot('k', str, '?') == 1);
    assert(str[0] == 'k');
    assert(str[1] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');
}

/* test08: Tests converting spaces with whitespace into tabs. */
void test08(void) {
    char str[9];

    /* NOTE: Newlines reset the current line length. */

    assert(stot('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(stot('\n', str, '?') == 1);
    assert(str[0] == '\n');
    assert(str[1] == '\0');

    /* NOTE: Spaces followed by an ordinary character cannot be converted. */

    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot('b', str, '?') == 3);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == 'b');
    assert(str[3] == '\0');

    /* NOTE: Existing tabs expand unconverted to the next tabstop. */

    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot('\t', str, '?') == 5);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == '\t');
    assert(str[5] == '\0');

    /* NOTE: Eight spaces following a tabstop convert into one tab. */

    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');
}

/* test09: Tests converting spaces with unprintables into tabs. */
void test09(void) {
    char str[9];

    /* NOTE: An unprintable that defaults to an ordinary character counts as an
     *       ordinary charcter; an unprintable that defaults to a newline
     *       resets the current line length. */

    assert(stot('\0', str, '?') == 1);
    assert(str[0] == '?');
    assert(str[1] == '\0');
    assert(stot('\0', str, '\n') == 1);
    assert(str[0] == '\n');
    assert(str[1] == '\0');

    /* NOTE: An unprintable that defaults to another unprintable does nothing
     *       and does not affect the current line length; an unprintable that
     *       defaults to a space potentially converts into a tab. */

    assert(stot('a', str, '\0') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');
    assert(stot('\0', str, ' ') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, '\0') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, ' ') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, ' ') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, ' ') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, ' ') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, ' ') == 0);
    assert(str[0] == '\0');
    assert(stot('\0', str, ' ') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');
}

/* test10: Tests converting tabs and spaces concurrently. */
void test10(void) {
    char str[9];

    /* NOTE: Converting tabs into spaces does not affect the current line
     *       length of converting spaces into tabs, and vice versa. */

    assert(stot('a', str, '?') == 1);
    assert(str[0] == 'a');
    assert(str[1] == '\0');

    assert(ttos('b', str, '?') == 1);
    assert(str[0] == 'b');
    assert(str[1] == '\0');

    assert(stot('c', str, '?') == 1);
    assert(str[0] == 'c');
    assert(str[1] == '\0');

    assert(ttos('\t', str, '?') == 7);
    assert(str[0] == ' ');
    assert(str[1] == ' ');
    assert(str[2] == ' ');
    assert(str[3] == ' ');
    assert(str[4] == ' ');
    assert(str[5] == ' ');
    assert(str[6] == ' ');
    assert(str[7] == '\0');

    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 0);
    assert(str[0] == '\0');
    assert(stot(' ', str, '?') == 1);
    assert(str[0] == '\t');
    assert(str[1] == '\0');
}


int main(void) {
    test01();
    test02();
    test03();
    test04();
    test05();
    test06();
    test07();
    test08();
    test09();
    test10();


    return 0;
}
