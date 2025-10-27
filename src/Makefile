SRC = src
BUILD = build
GEN = $(BUILD)/generated
INCLUDE = include

GRAMMAR = $(SRC)/grammar
F_IN = $(GRAMMAR)/lexer.l
B_IN = $(GRAMMAR)/parser.y

AST = $(SRC)/ast.cpp
RASCAL = $(SRC)/rascal.cpp

F_OUT = $(GEN)/lex.yy.cpp
B_OUT = $(GEN)/parser.tab.cpp
B_HEADER = $(GEN)/parser.tab.hpp

RASCAL = $(BUILD)/rascal

all: $(RASCAL)

$(B_OUT) $(B_HEADER): $(B_IN)
	bison -d $(B_IN) -o $(B_OUT)

$(F_OUT): $(F_IN) $(B_HEADER)
	flex -o $(F_OUT) $(F_IN)

$(CALC): $(B_OUT) $(F_OUT)
	g++ $(B_OUT) $(F_OUT) $(AST) -Wall -o $(RASCAL) -I$(SRC) -I$(GEN) -I$(INCLUDE)

clean:
	rm -f $(CALC) $(F_OUT) $(B_OUT) $(B_HEADER)

run: $(CALC)
	$(CALC) < $(file)
