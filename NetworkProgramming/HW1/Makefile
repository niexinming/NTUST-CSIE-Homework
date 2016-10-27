all:
	gcc server/server.c -o server/server
	gcc client/client.c -o client/client

clean:
	rm server/server client/client

test:
	bash -c '(cd server; ./server &); ./client/client; kill %1'
