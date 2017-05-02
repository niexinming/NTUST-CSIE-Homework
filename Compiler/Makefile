all:
	flex -o scanner.c go--.l
	gcc scanner.c symtab.c tokens.c -ll -o scanner
	./scanner < testcase/test.go
