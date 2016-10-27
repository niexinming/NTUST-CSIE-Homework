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

FILE * imgproto_connect(const char *ip)
{
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(client_fd < 0) {
		error("Can not create socket");
	}

	struct sockaddr_in conn_addr;
	conn_addr.sin_family = AF_INET;
	conn_addr.sin_port = htons(9999);

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
		printf("- %s\n", buff);
		free(buff);
	}
}

void imgproto_get(FILE *stream, char *file)
{
	size_t sz = strlen(file);
	if(fwrite("DWNL", 4, 1, stream) != 1 ||
		fwrite(&sz, 4, 1, stream) != 1 ||
		fwrite(file, sz, 1, stream) != 1) {
		error("Can not write to socket");
	}

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

	if(argc > 1) {
		ip = argv[1];
	} else {
		puts("[*] Default connect to localhost");
	}

	FILE *stream = imgproto_connect(ip);

	if(stream == NULL) {
		error("Can not connect to host");
	}

	if(imgproto_handshake(stream) != 0) {
		error("Invalid protocol");
	}

	puts("Connected");

	imgproto_list(stream);

	printf("Which file do you want? ");
	char buff[64];
	fgets(buff, 63, stdin);
	buff[63] = '\0';
	char *p = buff + strlen(buff) - 1;
	if(*p == '\n') {
		*p = '\0';
	}

	imgproto_get(stream, buff);
	fwrite("EXIT", 4, 1, stream);
	fclose(stream);
}
