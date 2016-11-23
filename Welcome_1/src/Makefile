objects = compile.o lex.o parser.o

compile : $(objects)
	gcc -o compile $(objects)
main.o : compile.c lex.h parser.h
	gcc -c compile.c
lex.o : lex.c lex.h
	gcc -c lex.c
parser.o : parser.c lex.h parser.h
	gcc -c parser.c
clean :
	rm compile $(objects)