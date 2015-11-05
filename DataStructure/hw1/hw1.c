#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// #define UNIT_TEST

#define MAX_LEN 1024*1024
#define STR_NOT_FOUND ((ptrdiff_t)-1)
#define STR_REPLACE_FAIL ((size_t)-1)
#define min(x, y) ((x < y) ? (x) : (y))

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define COLOR_BLACK   ""
#define COLOR_RED     ""
#define COLOR_GREEN   ""
#define COLOR_YELLOW  ""
#define COLOR_BLUE    ""
#define COLOR_PURPLE  ""
#define COLOR_MAGENTA ""
#define COLOR_CYAN    ""
#else
#define COLOR_BLACK   "\e[30m"
#define COLOR_RED     "\e[31m"
#define COLOR_GREEN   "\e[32m"
#define COLOR_YELLOW  "\e[33m"
#define COLOR_BLUE    "\e[34m"
#define COLOR_MAGENTA "\e[35m"
#define COLOR_CYAN    "\e[36m"
#define COLOR_WHITE   "\e[37m"
#endif

char buffer[MAX_LEN + 1],
     second_buffer[MAX_LEN + 1],
     third_buffer[MAX_LEN + 1],
     fourth_buffer[MAX_LEN + 1];

size_t strlen_(char *s)
{
    size_t n = 0;
    while(s[n]) n++;
    return n;
}

size_t strncpy_(char *dst, char *src, size_t n)
{
    size_t i = 0;
    do dst[i] = src[i]; while(src[i] && ++i < n);
    return i;
}

void strncat_(char *dst, char *src, size_t n)
{
    strncpy_(dst + strlen_(dst), src, n);
}

void memcpy_(char *dst, char *src, size_t n)
{
    while(n--) *dst++ = *src++;
}

int memcmp_(char *a, char *b, ptrdiff_t l)
{
    int n;
    while(l-- > 0) {
        int n = *a++ - *b++;
        if(n) return n;
    }
    return 0;
}

ptrdiff_t substring_index(char *buffer, char *to_find, ptrdiff_t index)
{
    size_t buffer_len = strlen_(buffer), to_find_len = strlen_(to_find);
    ptrdiff_t max_pos = buffer_len - to_find_len;
    while(index <= max_pos)
        if(memcmp_(buffer + index, to_find, to_find_len) == 0) return index; else index++;
    return STR_NOT_FOUND;
}

size_t frequency_count(char *buffer, char *to_find)
{
    ptrdiff_t index = -1;
    size_t count = -1;
    do {
        index = substring_index(buffer, to_find, index + 1);
        count++;
    } while(index != STR_NOT_FOUND);
    return count;
}

size_t string_replace(char *dst, char *src, char *to_find, char *to_place, size_t max_size)
{
#ifdef UNIT_TEST
    printf("str_replace(\"%s\", \"%s\", \"%s\");\n", src, to_find, to_place);
#endif
    size_t sz = 0, to_find_len = strlen_(to_find), left_size = max_size;
    while(sz < max_size)
    {
        ptrdiff_t next_occurrence = substring_index(src, to_find, 0);
        if(next_occurrence == STR_NOT_FOUND)
        {
            sz += strncpy_(dst, src, left_size);
            break;
        }

        size_t sz_tmp;
        if (next_occurrence > 0) {
            sz_tmp = strncpy_(dst, src, min(next_occurrence, left_size));
            sz += sz_tmp;
            dst += sz_tmp;
            src += sz_tmp;
            left_size -= sz_tmp;
        }
        sz_tmp = strncpy_(dst, to_place, left_size);
        dst += sz_tmp;
        src += to_find_len;
        left_size -= sz_tmp;
        sz += sz_tmp;
    }
    return sz;
}

size_t string_delete(char *dst, char *src, char *to_delete, size_t max_size)
{
    return string_replace(dst, src, to_delete, "", max_size);
}

void fgets_strip_newline(char *buffer, size_t max_size, FILE* file)
{
    fgets(buffer, max_size, file);
    size_t l = strlen_(buffer);
    if(buffer[l - 1] == '\n')
        buffer[l - 1] = '\0';
}

int main_menu()
{
    puts(
        COLOR_GREEN
        "\n"
        " 0> exit\n"
        " 1> input string\n"
        " 2> frequency count\n"
        " 3> substring delete\n"
        " 4> string replacing\n"
    );
    printf(COLOR_CYAN "Your choice >" COLOR_WHITE);
    fgets_strip_newline(fourth_buffer, MAX_LEN, stdin);
    return atoi(fourth_buffer);
}

int main()
{
#ifdef UNIT_TEST
    void self_test(); self_test(); return 0;
#endif
    while(1)
    {
        switch(main_menu())
        {
            case 0:
                return 0;
            case 1:
                printf(COLOR_CYAN "Input string >" COLOR_YELLOW);
                fgets_strip_newline(buffer, MAX_LEN, stdin);
                break;
            case 2:
                printf(COLOR_CYAN "Input string to search >" COLOR_YELLOW);
                fgets_strip_newline(second_buffer, MAX_LEN, stdin);
                printf(COLOR_MAGENTA "%llu" COLOR_BLUE " times\n", (uint64_t)frequency_count(buffer, second_buffer));
                break;
            case 3:
                printf(COLOR_CYAN "Input string to delete >" COLOR_YELLOW);
                fgets_strip_newline(third_buffer, MAX_LEN, stdin);
                string_delete(second_buffer, buffer, third_buffer, MAX_LEN);
                strncpy_(buffer, second_buffer, MAX_LEN);
                break;
            case 4:
                printf(COLOR_CYAN "Input string to replace >" COLOR_YELLOW);
                fgets_strip_newline(third_buffer, MAX_LEN, stdin);
                printf(COLOR_CYAN "Input string to place >" COLOR_MAGENTA);
                fgets_strip_newline(fourth_buffer, MAX_LEN, stdin);
                string_replace(second_buffer, buffer, third_buffer, fourth_buffer, MAX_LEN);
                strncpy_(buffer, second_buffer, MAX_LEN);
                break;
            default:
                continue;
        }

        printf(COLOR_GREEN "String now:" COLOR_WHITE " %s\n", buffer);
    }
}

#ifdef UNIT_TEST
//  _____         _
// |_   _|__  ___| |_
//   | |/ _ \/ __| __|
//   | |  __/\__ \ |_
//   |_|\___||___/\__|
//
#define TEST_STRSTRSTRINGINGING "Test StrStringinginginging"
#define bigtext(t) { int r = system("figlet " t); if(r) puts(t); }
#include <string.h>

void self_test()
{
    bigtext("Testing...");

    assert(strncpy_(buffer, "", 100) == 0);
    assert(strncpy_(buffer, TEST_STRSTRSTRINGINGING, sizeof(buffer)) == strlen(TEST_STRSTRSTRINGINGING));
    strncpy(second_buffer, TEST_STRSTRSTRINGINGING, sizeof(second_buffer));

    assert(strcmp(buffer, TEST_STRSTRSTRINGINGING) == 0);
    assert(strcmp(buffer, second_buffer) == 0);

    assert(strlen_("") == 0);
    assert(strlen_(buffer) == strlen(buffer));

    assert(memcmp_("asdf", "asdf", 4) == 0);
    assert(memcmp_("bsdf", "asdf", 4) > 0);
    assert(memcmp_("asdf", "asdz", 4) < 0);

    assert(frequency_count(buffer, "ing") == 5);
    assert(frequency_count("aaaaa", "a") == 5);
    assert(frequency_count("ababa", "ab") == 2);
    assert(frequency_count("aaaaa", "aaa") == 3);

    string_replace(second_buffer, buffer, "String", "Integer", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);
    string_replace(second_buffer, buffer, "ing", "INT", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);
    string_replace(second_buffer, buffer, "Test", "==Test==", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);
    string_replace(second_buffer, buffer, "In", "Out", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);
    string_replace(second_buffer, buffer, "T", "#", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);

    strncpy(buffer, TEST_STRSTRSTRINGINGING, MAX_LEN);

    string_delete(second_buffer, buffer, "String", MAX_LEN);
    puts("Test string_delete");
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);
    string_delete(second_buffer, buffer, "String", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);
    string_delete(second_buffer, buffer, "String", MAX_LEN);
    strncpy_(buffer, second_buffer, MAX_LEN);
    printf("> %s\n", buffer);

    bigtext("All Test Are Pass!");
}
#endif
