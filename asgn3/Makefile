-s: freq.o dict.o
	gcc -o freq freq.o dict.o

freq.o: freq.c
	gcc -g -Wall -Wextra -Werror -Werror=unused-but-set-variable -ansi -pedantic -c freq.c

dict.o: dict.c
	gcc -g -Wall -Wextra -Werror -Werror=unused-but-set-variable -ansi -pedantic -c dict.c

debug: freq.c dict.c
	gcc -Wall -Wextra -ansi -pedantic -g -o freq freq.c dict.c

test: debug
	gcc -Wall -Wextra -Werror -ansi -pedantic -g dicttests.c dict.c
	valgrind --leak-check=full --show-leak-kinds=all ./a.out
