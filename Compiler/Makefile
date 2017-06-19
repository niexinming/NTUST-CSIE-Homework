all: jasm
	lex -o scanner.c go--.l
	yacc go--.y --output=parser.c --defines=parser.h
	gcc -g -std=c99 main.c parser.c symtab.c tokens.c ast.c codegen.c -ll -ly -o goc
	./goc testcase/test.go | tee prog.jasm
	@echo --- run assembler ---
	./jasm prog.jasm
	@echo --- run program ---
	java prog

jasm:
	(cd javaa; make)
	cp javaa/javaa jasm

clean:
	@rm scanner goc parser.c parser.h
