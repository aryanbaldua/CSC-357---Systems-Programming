# Builds targets for converting characters between tabs and spaces.
# CSC 357, Assignment 1
# Given code, Spring '24
# NOTE: Do not alter this file.

all: detab entab

detab: detab.o chars.o
	gcc -o detab detab.o chars.o

entab: entab.o chars.o
	gcc -o entab entab.o chars.o

detab.o: detab.c
	gcc -Wall -Wextra -Werror -ansi -pedantic -c detab.c

entab.o: entab.c
	gcc -Wall -Wextra -Werror -ansi -pedantic -c entab.c

chars.o: chars.c chars.h
	gcc -Wall -Wextra -Werror -ansi -pedantic -c chars.c

test:
	gcc -Wall -Wextra -Werror -ansi -pedantic chars.c chartests.c
	./a.out
