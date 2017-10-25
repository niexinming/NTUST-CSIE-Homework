#include <unistd.h>
#include <syscall.h>
#include <fcntl.h>

size_t strlen(const char *s)
{
	size_t len = 0;
	while (s[len]) len++;
	return len;
}

void println(const char *msg)
{
	syscall(SYS_write, 1, msg, strlen(msg));
	syscall(SYS_write, 1, "\n", 1);
}

int main()
{
	const char const * file = "/tmp/file_owner_uid_should_be_1337";

	println("We are demo usage of syscall: setuid (syscall number 105 in x64 Linux)");

	println("setuid to 1337");
	if(syscall(SYS_setuid, 1337) != 0) {
		println("Can not setuid, please run as root!");
		syscall(SYS_exit, 1);
	}
	syscall(SYS_unlink, file);
	int fd = syscall(SYS_open, file, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0) {
		syscall(SYS_write, 2, "Can not create file\n", 20);
		syscall(SYS_exit, 1);
	}
	syscall(SYS_write, fd, "Hello, World", 12);
	syscall(SYS_close, fd);
}
