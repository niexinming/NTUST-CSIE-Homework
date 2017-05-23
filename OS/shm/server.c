#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "shm.h"

// use linked list to save data
struct RECORD_NODE {
    struct RECORD_NODE *next;
    uint64_t hash;
    uint64_t count;
    uint64_t length;
    char data[0];
} *records = NULL;

// a data hash function
uint64_t simple_hash(void *s, size_t len)
{
    uint64_t a = 0xdeadbeef0badc0de;
    uint64_t b = 0xc87639487639487c;
    uint64_t c = 0xabcdef0123456789;
    uint64_t d;
    size_t i = 0;
    uint8_t *data = (uint8_t *)s;
    for(i = 0; i < len; i++) {
        a = a * 63 + data[i];
        b = (b << 3) ^ data[i] * 0x9487;
        c = ~c - data[i] * 0x7777777777777;

        d = a + b - c;

        a ^= d;
        b ^= d | a;
        c ^= a & b;
    }

    return (a - b) ^ c;
}

// create a node for specific data
struct RECORD_NODE * create_node(char *data, size_t len, uint64_t hash)
{
    size_t size = sizeof(struct RECORD_NODE) + len + 1;
    struct RECORD_NODE * record = (struct RECORD_NODE *)malloc(size);

    record->next = NULL;
    record->hash = hash == (uint64_t)-1 ? simple_hash(data, len) : hash;
    record->count = 0;
    record->length = len;
    strncpy(record->data, data, len + 1);
    return record;
}

// find or create new record in database
struct RECORD_NODE * find_or_create(char *data)
{
    size_t len = strlen(data);
    if(records == NULL) {
        return (records = create_node(data, len, -1));
    } else {
        uint64_t hash = simple_hash(data, len);
        struct RECORD_NODE *p = records, *last;
        while(p) {
            if(p->hash == hash && strncmp(data, p->data, len + 1) == 0) {
                return p;
            }
            last = p;
            p = p->next;
        }

        return (last->next = create_node(data, len, hash));
    }
}

int main()
{
    void *shm;
    if((shm = get_shm()) == NULL) {
        return 1;
    }

    struct TASK *task = (struct TASK *)shm;

    while(1) {
        int counter = 0;
		// deal with invalid state and wait for request
        while(1) {
            int stat = shm_stat(task);
            if(stat == SHM_LOCK_INVALID || counter >= 10) {
                shm_lock_clear(task);
                task->op = '\0';
                counter = 0;
            } else if(stat == SHM_LOCK_COMMITED) {
                break;
            } else if(stat == SHM_LOCK_LOCKED) {
                counter++;
            }
            usleep(100);
        }

        struct RECORD_NODE *record;

        switch(task->op) {
            case '\0':
                puts("[+] Task: nothing");
                break;
            case '%':
				record = records;
				records = NULL;
				while(record) {
					void *p = record;
					record = record->next;
					free(p);
				}
                break;
            case '+':
                puts("[+] Task: add recrod");
                record = find_or_create(task->data);
                record->count++;
                printf("-> Record \"%s\" added or incremented\n", task->data);
                break;
            case '#':
                puts("[+] Task: single record query");
                record = find_or_create(task->data);
                printf("-> Recrod %s\n", record->count ? "found" : "not found");
                break;
            case '!':
                puts("[+] Task: delete record");
                record = find_or_create(task->data);
                printf("-> Recrod %s\n", record->count ? "deleted" : "not found");
				record->count = 0;
                break;
            case '@':
                puts("[+] Task: statistics");
                record = records;
                while(record) {
                    if(record->count) {
                        printf("%4zd %-64s\n", record->count, record->data);
                    }
                    record = record->next;
                }
                break;
            default:
                puts("[-] Unkown task!");
        }

		// finish request
        task->op = '\0';
        shm_lock_free(task);
    }
}
