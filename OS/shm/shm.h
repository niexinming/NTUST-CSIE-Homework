#include <sys/types.h>

struct TASK;
extern void* get_shm();
extern size_t shm_get_size();
extern void shm_lock_acquire(struct TASK *task);
extern void shm_lock_free(struct TASK *task);
extern void shm_lock_commit(struct TASK *task);
extern void shm_lock_clear(struct TASK *task);
extern int shm_stat(struct TASK *task);
extern void shm_lock_free(struct TASK *task);

// available for request
#define SHM_LOCK_FREED 0
// writing request data
#define SHM_LOCK_LOCKED 0xcafe
// request data finished, server can process the request
#define SHM_LOCK_COMMITED 0xc8763
// invalid state
#define SHM_LOCK_INVALID 0xdead

struct __attribute__((__packed__)) TASK {
    uint32_t counter;
    uint32_t lock;
    uint64_t op;
    char data[0];
};
