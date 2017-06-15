all:
	lex -o scanner.c go--.l
	yacc go--.y --output=parser.c --defines=parser.h
	gcc -g parser.c symtab.c tokens.c ast.c codegen.c -ll -ly -o parser
	./parser testcase/test.go | tee prog.jasm
	@echo --- run assembler ---
	./javaa/javaa prog.jasm
	@echo --- run program ---
	java prog

clean:
	@rm scanner parser parser.c parser.h
