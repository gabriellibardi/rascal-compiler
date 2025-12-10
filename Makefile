SRC = src
BUILD = build
GEN = $(BUILD)/generated
INCLUDE = include

GRAMMAR = $(SRC)/grammar
F_IN = $(GRAMMAR)/lexer.l
B_IN = $(GRAMMAR)/parser.y

CPP_SRC = \
    $(SRC)/ast.cpp \
    $(SRC)/rascal.cpp \
    $(SRC)/semantic/symbol_table.cpp \
    $(SRC)/semantic/check_visitor.cpp \

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

$(EXE): $(B_OUT) $(F_OUT) $(CPP_SRC) | $(BUILD)
	g++ $(B_OUT) $(F_OUT) $(CPP_SRC) -Wall -o $(EXE) \
		-I$(SRC) -I$(GEN) -I$(INCLUDE) -std=c++20

clean:
	rm -rf $(BUILD)

run: $(EXE)
	$(EXE) < $(file)
