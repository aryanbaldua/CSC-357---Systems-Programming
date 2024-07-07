-s: lzw.o dict.o
	gcc -o lzw lzw.o dict.o

lzw.o: lzw.c
	gcc -g -Wall -Wextra -std=gnu11 -pedantic -c lzw.c

dict.o: dict.c
	gcc -g -Wall -Wextra -std=gnu11 -pedantic -c dict.c

debug: lzw.c dict.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -g -o lzw lzw.c dict.c

test: debug
	gcc -Wall -Wextra -std=gnu11 -pedantic -g lzw.c dict.c
	valgrind --leak-check=full --show-leak-kinds=all ./lzw -x out4.bin tmp.bin