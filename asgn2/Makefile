# To combine all object files into a single executable "program":
-s: assemble.o clean.o draw.o
	gcc -o hist assemble.o clean.o draw.o

# Dependencies for each object file
assemble.o: assemble.c
	gcc -g -Wall -Wextra -Werror -Werror=unused-but-set-variable -ansi -pedantic -c assemble.c

clean.o: clean.c
	gcc -g -Wall -Wextra -Werror -Werror=unused-but-set-variable -ansi -pedantic -c clean.c

draw.o: draw.c
	gcc -g -Wall -Wextra -Werror -Werror=unused-but-set-variable -ansi -pedantic -c draw.c
