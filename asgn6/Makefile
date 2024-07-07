-s: batch.o
	gcc -o batch batch.o

batch.o: batch.c
	gcc -g -Wall -Wextra -std=gnu11 -pedantic -c batch.c

testprog.o: testprog.c
	gcc -g -Wall -Wextra -std=gnu11 -pedantic -c testprog.c

debug: batch.c 
	gcc -Wall -Wextra -std=gnu11 -pedantic -g -o batch batch.c 

test: debug
	gcc -Wall -Wextra -std=gnu11 -pedantic -g batch.c
	valgrind --leak-check=full --show-leak-kinds=all ./batch 


