#ifndef __STRDUP__
#define __STRDUP__
#include <string.h>
char * mystrdup(const char *s) {
	size_t l = strlen(s);
	char *buff = malloc(l + 1);
	memcpy(buff, s, l + 1);
	return buff;
}
#endif
