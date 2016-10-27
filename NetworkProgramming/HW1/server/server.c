#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>

struct remote_peer {
	struct   sockaddr_in addr;
	char     ip[16];
	uint16_t port;
	int      fd;
};

int writeall(int fd, const void *data, size_t len)
{
	size_t written = 0;
	while(written < len) {
		ssize_t writed = write(fd, written + (const char *)data, len - written);
		if(writed < 0) {
			return written;
		}
		written += writed;
	}
	return written;
}

int writestr(int fd, const char *str)
{
	return writeall(fd, str, strlen(str));
}

int sendstr(int fd, const char *str)
{
	size_t s = strlen(str);
	// printf("Send str \"%s\" with length %zu\n", str, s);
	size_t l = write(fd, &s, 4);
	l += writeall(fd, str, s);
	return l;
}

int readall(int fd, void *data, size_t len)
{
	size_t readed = 0;
	while(readed < len) {
		ssize_t s = read(fd, readed + (char*)data, len - readed);
		if(s <= 0) {
			return readed;
		}
		readed += s;
	}
	return readed;
}

char *recvstr(int fd)
{
	size_t s = 0;

	if(readall(fd, &s, 4) != 4) {
		return NULL;
	}

	char *buff = malloc(s + 1);
	if(readall(fd, buff, s) != s) {
		free(buff);
		return NULL;
	}
	buff[s] = '\0';
	return buff;
}

void handle_list(int fd)
{
	DIR *dir = opendir(".");
	struct dirent *curr;
	int i = 0;
	while ((curr = readdir(dir)) != NULL) {
		if(curr->d_name[0] == '.') {
			continue;
		}
		sendstr(fd, curr->d_name);
	}
	sendstr(fd, "");
	closedir(dir);
}

int handle_download(int fd)
{
	char *file = recvstr(fd);
	printf("[*] Download: %s\n", file);

	struct stat fs;
	if(stat(file, &fs) != 0) {
		printf("[-] Can not stat file\n");
		writestr(fd, "");
	}

	printf("[*] File size = %u\n", (uint32_t)fs.st_size);

	int fd_f = open(file, O_RDONLY);
	free(file);

	if(fd_f < 0) {
		printf("[-] Can not open file\n");
		writestr(fd, "");
	}

	write(fd, &fs.st_size, 4);
	char buff[4096];
	size_t sended = 0;
	while(sended < fs.st_size) {
		ssize_t r = read(fd_f, buff, sizeof(buff));
		if(r <= 0) {
			printf("[-] Can not read file");
			break;
		}
		write(fd, buff, r);
		sended += r;
	}
	close(fd_f);

	return sended != fs.st_size;
}

void * serve(void *arg)
{
	struct remote_peer peer = *(struct remote_peer *)arg;
	free(arg);
	printf("[*] Connection from %s:%d at fd %d\n", peer.ip, peer.port, peer.fd);

	char buff[64];
	read(peer.fd, buff, 8);
	if(memcmp(buff, "IMGPROTO", 8) != 0) {
		goto byebye;
	}
	if(writeall(peer.fd, "IMGPROTO", 8) != 8) {
		goto byebye;
	}

	printf("[*] fd#%d have correct protocol\n", peer.fd);

	while(1) {
		if(readall(peer.fd, buff, 4) != 4) {
			printf("[-] can not read command from fd#%d\n", peer.fd);
			break;
		}
		buff[4] = '\0';
		printf("[*] fd#%d got command %s\n", peer.fd, buff);
		if(memcmp(buff, "LIST", 4) == 0) {
			handle_list(peer.fd);
		} else if (memcmp(buff, "DWNL", 4) == 0) {
			if(handle_download(peer.fd) != 0) {
				break;
			}
		} else if (memcmp(buff, "EXIT", 4) == 0) {
			writestr(peer.fd, "BYEE");
			break;
		}
	}

byebye:
	shutdown(peer.fd, SHUT_RDWR);
	close(peer.fd);

	return NULL;
}

int main(int argc, char *argv[])
{
	int serv_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(serv_fd < 0) {
		fprintf(stderr, "Can not create socket\n");
		return 1;
	}

	if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR,
					&(int){ 1 }, sizeof(int)) < 0) {
		fprintf(stderr, "Can not set SO_REUSEADDR\n");
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof serv_addr);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(9999);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);

	if(bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
		fprintf(stderr, "Can not bind address\n");
		return 1;
	}

	if(listen(serv_fd, 5) != 0) {
		fprintf(stderr, "Can not listen\n");
		return 1;
	}

	while(1) {
		struct remote_peer *remote = malloc(sizeof(struct remote_peer));
		socklen_t s_len = sizeof(remote->addr);
		int remote_fd = accept(serv_fd, (struct sockaddr *)&remote->addr, &s_len);

		inet_ntop(AF_INET, &(remote->addr.sin_addr), remote->ip, sizeof remote->ip);
		remote->port = ntohs(remote->addr.sin_port);
		remote->fd = remote_fd;

		if(remote_fd < 0) {
			fprintf(stderr, "Can not accept connection\n");
			return 2;
		}

		pthread_t thread;
		if(pthread_create(&thread, /* attr */ NULL, serve, remote) != 0) {
			fprintf(stderr, "Can not create thread\n");
			return 3;
		}
	}
}
