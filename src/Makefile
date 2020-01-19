EXEC_FILE_NAME=$(BIN_DIR)/europa
CCOMPILER=cc
INCLUDE_FILES=main.c lexer.c parser.c ast.c core/*.c
#LINKER_FLAGS=-rdynamic -lfl -ldl
LINKER_FLAGS=
export INCLUDE_DIR=-I$(SRC_DIR)/include -I$(SRC_DIR)/core/include
export GCC_FLAGS=-O3
export DEBUG_GCC_FLAGS=-O0 -DDEBUG -ggdb

all:
	$(CCOMPILER) -o $(EXEC_FILE_NAME) $(INCLUDE_DIR) $(GCC_FLAGS) $(INCLUDE_FILES) $(LINKER_FLAGS)

debug: debug_parser scanner
	$(CCOMPILER) -o $(EXEC_FILE_NAME) $(INCLUDE_DIR) $(DEBUG_GCC_FLAGS) $(INCLUDE_FILES) $(LINKER_FLAGS)
	
clean: 
	rm -f $(EXEC_FILE_NAME) *.obj	
	
