all: rascal.out

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

rascal.out: rascal.cpp parser.tab.c lex.yy.c
	g++ rascal.cpp parser.tab.c lex.yy.c -o rascal.out

clean:
	rm parser.tab.* lex.yy.c rascal

recompile:
	clean rascal.out
