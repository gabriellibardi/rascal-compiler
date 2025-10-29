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

EXE = $(BUILD)/rascal

all: $(EXE)

$(BUILD):
	mkdir -p $(BUILD)

$(GEN): | $(BUILD)
	mkdir -p $(GEN)

$(B_OUT) $(B_HEADER): $(B_IN) | $(GEN)
	bison -d $(B_IN) -o $(B_OUT)

$(F_OUT): $(F_IN) $(B_HEADER) | $(GEN)
	flex -o $(F_OUT) $(F_IN)

$(EXE): $(B_OUT) $(F_OUT) $(AST) $(RASCAL) | $(BUILD)
	g++ $(B_OUT) $(F_OUT) $(AST) $(RASCAL) -Wall -o $(EXE) -I$(SRC) -I$(GEN) -I$(INCLUDE)

clean:
	rm -rf $(BUILD)

run: $(EXE)
	$(EXE) < $(file)
