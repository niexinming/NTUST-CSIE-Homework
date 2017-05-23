#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "shm.h"

int main()
{
    puts("Usage:");
    puts("  @       print statistics data");
    puts("  %       clear all data");
    puts("  #word   query if this word in database");
	puts("  !word   delete specfic entry");
    puts("  word    add word to database");
    puts("");

    void *shm;
    if((shm = get_shm()) == NULL) {
        return 1;
    }
    struct TASK *task = (struct TASK*)shm;

    char buff[4096], *nl;
    while(1) {
		// read data from input
        if(fgets(buff, sizeof(buff) - 1, stdin) == NULL) {
            break;
        }
		// strip ending newline
        nl = strchr(buff, '\n');
        if(nl) *nl = '\0';

		// acquire lock
        puts("[+] wait for lock...");
        shm_lock_acquire(task);

		// buff[0] is operator or 1st char of data to add
        switch(buff[0]) {
            case '#':
            case '!':
                task->op = buff[0];
                strncpy(task->data, buff + 1, shm_get_size() - sizeof(struct TASK));
                break;
            case '@':
			case '%':
                task->op = buff[0];
                break;
            default:
                task->op = '+';
                strncpy(task->data, buff, shm_get_size() - sizeof(struct TASK));
        }

		// commit request, server can process this request now
        puts("[+] task commited");
        shm_lock_commit(task);
    }
}
