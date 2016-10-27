#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void error(const char *errmsg)
{
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}

FILE * imgproto_connect(const char *ip, int port)
{
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(client_fd < 0) {
		error("Can not create socket");
	}

	struct sockaddr_in conn_addr;
	conn_addr.sin_family = AF_INET;
	conn_addr.sin_port = htons(port);

	if(inet_pton(AF_INET, ip, &conn_addr.sin_addr) <= 0) {
		error("Can not convert ip");
	}

	if(connect(client_fd, (struct sockaddr *)&conn_addr, sizeof(conn_addr)) < 0) {
		error("Can not connect");
	}

	return fdopen(client_fd, "r+b");
}

int imgproto_handshake(FILE *stream)
{
	if(fwrite("IMGPROTO", 8, 1, stream) != 1) {
		error("Can not write to socket");
	}

	char buff_protoid[8];
	if(fread(buff_protoid, 8, 1, stream) != 1) {
		error("Can not read from socket");
	}

	if(memcmp(buff_protoid, "IMGPROTO", 8) != 0) {
		return 1;
	}

	return 0;
}

void imgproto_list(FILE *stream)
{
	if(fwrite("LIST", 4, 1, stream) != 1) {
		error("Can not write to socket");
	}

	puts("Available files:");
	int idx = 0;
	while(1) {
		size_t sz = 0;
		if(fread(&sz, 4, 1, stream) != 1) {
			error("Can not read from socket");
		}
		if(sz == 0) {
			break;
		}
		char *buff = malloc(sz + 1);
		fgets(buff, sz + 1, stream);
		buff[sz] = 0;
		printf("%2d: %s\n", idx++, buff);
		free(buff);
	}
}

void imgproto_get(FILE *stream, int which, const char *file)
{
	if(fwrite("DWNL", 4, 1, stream) != 1 ||
		fwrite(&which, 4, 1, stream) != 1) {
		error("Can not write to socket");
	}

	size_t sz = 0;
	if(fread(&sz, 4, 1, stream) != 1) {
		error("Can not read file size from socket");
	}
	printf("size = %zu\n", sz);
	size_t readed = 0;
	char buff[16384];
	FILE *fp = fopen(file, "wb");
	while(readed < sz) {
		size_t chunk = sz - readed;
		if(chunk > sizeof(buff)) chunk = sizeof(buff);
		if(fread(buff, chunk, 1, stream) != 1) {
			error("Can not read file body from socket");
		}
		if(fwrite(buff, chunk, 1, fp) != 1) {
			error("Can not write file");
		}
		readed += chunk;
	}
	fclose(fp);

	puts("File downloaded!");
}

int main(int argc, char *argv[])
{
	const char *ip = "127.0.0.1";
	int port = 9999;
	int choosen = 0;
	const char *filename = "out.dat";

	switch(argc) {
		default:
			puts("Too many arguments");
		case 5:
			filename = argv[4];
		case 4:
			choosen = atoi(argv[3]);
		case 3:
			port = atoi(argv[2]);
		case 2:
			ip = argv[1];
			break;
		case 1:
			printf("Usage: %s ip port choosen filename\n", argv[0]);
	}

	FILE *stream = imgproto_connect(ip, port);

	if(stream == NULL) {
		error("Can not connect to host");
	}

	if(imgproto_handshake(stream) != 0) {
		error("Invalid protocol");
	}

	puts("Connected");
	imgproto_list(stream);

	puts("Download");
	imgproto_get(stream, choosen, filename);
	fwrite("EXIT", 4, 1, stream);
	fclose(stream);
}
