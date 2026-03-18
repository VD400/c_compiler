# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -g

# The final executable name
TARGET = compiler

# List of all C source files
SRCS = lex.yy.c parser.tab.c ast.c symbol_table.c semantic.c main.c

# Default rule when you just type 'make'
all: $(TARGET)

# Rule to link everything into the final executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Rule to run Bison
parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

# Rule to run Flex (depends on Bison's header)
lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

# Clean up generated files so you can start fresh
clean:
	rm -f $(TARGET) lex.yy.c parser.tab.c parser.tab.h tokens.txt ast.txt symbol_table.txt
