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
	      tac.txt optimized.txt output.asm

run: compiler
	./compiler < test.c

build: compiler
	./compiler < test.c
	cp output.asm output.s
	gcc -m32 output.s -o myprogram -no-pie

execute: build
	./myprogram
