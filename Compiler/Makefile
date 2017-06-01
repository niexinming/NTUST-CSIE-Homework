all:
	lex -o scanner.c go--.l
	yacc go--.y --output=parser.c --defines=parser.h
	gcc parser.c symtab.c tokens.c ast.c -ll -ly -o parser
	#gcc scanner.c symtab.c tokens.c -ll -o scanner
	#./scanner < testcase/test.go
	./parser < testcase/test.go

clean:
	@rm scanner parser parser.c parser.h
