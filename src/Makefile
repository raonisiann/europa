EXEC_FILE_NAME=$(BIN_DIR)/europa
CCOMPILER=cc
INCLUDE_FILES=main.c lexer.c parser.c ast.c core/*.c debug/*.c io/*.c europa/*.c functions/*.c functions/builtin/*.c
#LINKER_FLAGS=-rdynamic -lfl -ldl
LINKER_FLAGS=
export INCLUDE_DIR=-I$(SRC_DIR)/include -I$(SRC_DIR)/core/include -I$(SRC_DIR)/debug/include -I$(SRC_DIR)/io/include -I$(SRC_DIR)/europa/include -I$(SRC_DIR)/functions/include
export GCC_FLAGS=-O3
export DEBUG_GCC_FLAGS=-O0 -DDEBUG -ggdb -fsanitize=address

all:
	$(CCOMPILER) -o $(EXEC_FILE_NAME) $(INCLUDE_DIR) $(GCC_FLAGS) $(INCLUDE_FILES) $(LINKER_FLAGS)

debug_enabled:
	$(CCOMPILER) -o $(EXEC_FILE_NAME) $(INCLUDE_DIR) $(DEBUG_GCC_FLAGS) $(INCLUDE_FILES) $(LINKER_FLAGS)
	
clean: 
	rm -f $(BIN_DIR)/*
	rm -f *.obj