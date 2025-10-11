all: rascal

syntactic.tab.c syntactic.tab.h: syntactic.y
	bison -d syntactic.y

lex.yy.c: lexer.l syntactic.tab.h
	flex lexer.l

rascal: rascal.cpp syntactic.tab.c lex.yy.c
	g++ rascal.cpp syntactic.tab.c lex.yy.c -o rascal

clean:
	rm syntactic.tab.* lex.yy.c rascal

recompile:
	clean rascal
