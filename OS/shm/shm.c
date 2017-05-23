#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include "shm.h"

#define KB (1<<10)
#define MB (1<<20)

// return size of shared memory
size_t shm_get_size()
{
    return 1 * MB;
}

// create or attach shared memory
void* get_shm()
{
    int shmid = shmget(0xdeadbeef, shm_get_size(), IPC_CREAT | 0770);
    if(shmid == -1) {
        puts("Can not get shared memory");
        return NULL;
    }

    void *shm = shmat(shmid, NULL, 0);
    if(shm == (void*)-1) {
        puts("Can not attach shared memory");
        return NULL;
    }

    return shm;
}

// acquire lock to write reqeust
void shm_lock_acquire(struct TASK *task)
{
    int success = 0;
    do {
        usleep(rand() % 24);

        if(task->lock) {
            continue;
        }

        ++task->counter;
        usleep(rand() % 8);
        success = 1;
        if(task->counter > 1) {
            success = 0;
            continue;
        }

        task->lock = 1;
    } while(!success);
}

// commit request, tell server to process the request
void shm_lock_commit(struct TASK *task)
{
    task->lock = SHM_LOCK_COMMITED;
}

// request done. free the reqeust
void shm_lock_free(struct TASK *task)
{
    --task->counter;
    task->lock = SHM_LOCK_FREED;
}

// force clear lock
void shm_lock_clear(struct TASK *task)
{
    task->counter = 0;
    task->lock = SHM_LOCK_FREED;
}

// return shared memory lock status immeidiately
int shm_stat_immeidiate(struct TASK *task)
{
    if(task->lock == SHM_LOCK_LOCKED && task->counter == 1) {
        return SHM_LOCK_LOCKED;
    } else if (task->lock == SHM_LOCK_FREED && task->counter == 0) {
        return SHM_LOCK_FREED;
    } else if (task->lock == SHM_LOCK_COMMITED) {
        return SHM_LOCK_COMMITED;
    } else {
        return SHM_LOCK_INVALID;
    }
}

// return shared memory lock status, with some sleep to ensure status
int shm_stat(struct TASK *task)
{
    for(int i = 0; i < 10 && shm_stat_immeidiate(task) == SHM_LOCK_INVALID; i++) {
        usleep(12);
    }
    return shm_stat_immeidiate(task);
}
