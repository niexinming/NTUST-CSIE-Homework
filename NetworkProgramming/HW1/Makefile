all:
	gcc server/server.c -o server/server
	gcc client/client.c -o client/client

test:
	bash -c '(cd server; ./server &); ./client/client'
