CC     = gcc
CFLAGS = -Wall -g

SRCS = lex.yy.c parser.tab.c ast.c symbol_table.c semantic.c \
       tac.c optimizer.c codegen.c main.c

all: compiler

compiler: $(SRCS)
	$(CC) $(CFLAGS) -o compiler $(SRCS) -lfl

lex.yy.c: lexer.l parser.tab.h
	flex -o lex.yy.c lexer.l

parser.tab.c parser.tab.h: parser.y
	bison -d -o parser.tab.c parser.y

clean:
	rm -f compiler lex.yy.c parser.tab.c parser.tab.h \
	      tokens.txt ast.txt symbol_table.txt \
	      tac.txt optimized.txt output.asm output.s myprogram

run: compiler
	./compiler < tests/test_combined.vd

build: compiler
	./compiler < tests/test_combined.vd
	gcc -m32 output.s -o myprogram -no-pie
	
execute: build
	./myprogram


install: compiler
	@echo "Installing to /usr/local/bin..."
	sudo cp compiler /usr/local/bin/minivd-core
	sudo cp minivd /usr/local/bin/
	sudo chmod +x /usr/local/bin/minivd
	sudo chmod +x /usr/local/bin/minivd-core
	@echo "Installation successful! You can now use the 'minivd' command anywhere."

uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/minivd
	sudo rm -f /usr/local/bin/minivd-core
	@echo "Uninstalled."
