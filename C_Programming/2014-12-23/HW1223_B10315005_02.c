#include <stdio.h>
#include <string.h>

#define IS_BLANK(X) ((X) == ' ' || (X) == '\t' || (X) == '\n' || (X) == '\r')

char* trim_blanks(char *trimmed, const char *to_trim)
{
    const char *start = to_trim, *end;
    char *result = trimmed;
    while (IS_BLANK(*start))
        start++;
    end = start + strlen(start) - 1;
    while (IS_BLANK(*end))
        end--;
    while (start <= end) {
        *trimmed++ = *start++;
    }
    *trimmed = '\0';
    return result;
}

int main()
{
    char buffer[1024], output[1024];
    gets(buffer);
    printf("Result: \"%s\"\n", trim_blanks(output, buffer));
    return 0;
}
