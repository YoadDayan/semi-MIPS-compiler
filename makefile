assembler: main.o pre_assembler.o assembler.o assembler_pass_1.o assembler_pass_2.o linked_list.o utils.o errors.o
	gcc -ansi -Wall -pedantic main.o pre_assembler.o assembler.o assembler_pass_1.o assembler_pass_2.o linked_list.o utils.o errors.o -o assembler -lm

main.o: main.c header.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

pre_assembler.o: pre_assembler.c header.h
	gcc -c -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o

assembler.o: assembler.c header.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

assembler_pass_1.o: assembler_pass_1.c header.h
	gcc -c -ansi -Wall -pedantic assembler_pass_1.c -o assembler_pass_1.o

assembler_pass_2.o: assembler_pass_2.c header.h
	gcc -c -ansi -Wall -pedantic assembler_pass_2.c -o assembler_pass_2.o

linked_list.o: linked_list.c header.h
	gcc -c -ansi -Wall -pedantic linked_list.c -o linked_list.o

utils.o: utils.c header.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o

errors.o: errors.c header.h
	gcc -c -ansi -Wall -pedantic errors.c -o errors.o
