-s: tree.o 
	gcc -o tree tree.o 

tree.o: tree.c
	gcc -g -Wall -Wextra -std=gnu11 -pedantic -c tree.c

debug: tree.c 
	gcc -Wall -Wextra -std=gnu11 -pedantic -g -o tree tree.c 

test: debug
	gcc -Wall -Wextra -std=gnu11 -pedantic -g tree.c
	valgrind --leak-check=full --show-leak-kinds=all ./tree testdir
	