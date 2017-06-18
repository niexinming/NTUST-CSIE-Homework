all: jasm
	lex -o scanner.c go--.l
	yacc go--.y --output=parser.c --defines=parser.h
	gcc -g -std=c99 parser.c symtab.c tokens.c ast.c codegen.c -ll -ly -o parser
	./parser testcase/test.go | tee prog.jasm
	@echo --- run assembler ---
	./jasm prog.jasm
	@echo --- run program ---
	java prog

jasm:
	(cd javaa; make)
	cp javaa/javaa jasm

clean:
	@rm scanner parser parser.c parser.h
