/*---Typedef's---*/
typedef struct spinlock_t {
    int val;
}spinlock_t;

typedef struct semaphore_t {
    int val;
    spinlock_t sl;
}semaphore_t;

typedef struct mutex_t {
    semaphore_t sem;
}mutex_t;

/*typedef struct condition {
    spinlock_t listLock;
}condition;

typedef struct rwlock {
    int nActive; //number of active readers, -1 if writer is active
    int nPendingReads;
    int nPendingWrites;
    spinlock_t sl;
    condition canRead;
    condition canWrite;
}rwlock;*/

/*---Functions---*/

//Spinlock
void spin_init(spinlock_t *s);
void spin_lock(spinlock_t *s);
void spin_unlock (spinlock_t *s);

//Futex
int futex_wait(void* addr, int val);
int futex_wake(void* addr, int n);

//Semaphore
void sem_init(semaphore_t *sem, int initval);
void block(semaphore_t *sem);
void sem_acquire(semaphore_t *sem); //acquire semaphore
void sem_release(semaphore_t *sem); //release semaphore 

//Mutex
void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);

//Functionality
/*void wait(condition *c, spinlock_t *s);
void do_signal(condition *c); //wake up one thread
void do_broadcast(condition *c); //wake up all threads 

//Read write locks
void lockShared(rwlock *r);
void unlockShared(rwlock *r);
void lockExclusive(rwlock *r);
void unlockExclusive(rwlock *r);
//void upgrade(rwlock *r);
//void downgrade(rwlock *r);*/
