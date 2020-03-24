//----Spinlock----
typedef struct spinlock_t {
    int x;
}spinlock_t;

int test_and_set(spinlock_t *s);
void spin_lock(spinlock_t *s);
void spin_unlock (spinlock_t *s);

//----Condition----
typedef struct condition {
    spinlock_t listLock;
}condition;

//----Semaphores----
typedef struct semaphore {
    int x;
    spinlock_t sl;
}semaphore;

void initsem(semaphore *sem, int val);
void P(semaphore *sem); //acquire semaphore
void V(semaphore *sem); //release semaphore 

//---Functionality---
void wait(condition *c, spinlock_t *s);
void do_signal(condition *c); //wake up one thread
void do_broadcast(condition *c); //wake up all threads 

//----Read Write Locks----
typedef struct rwlock {
    int nActive; //number of active readers, -1 if writer is active
    int nPendingReads;
    int nPendingWrites;
    spinlock_t sl;
    condition canRead;
    condition canWrite;
}rwlock;

void lockShared(rwlock *r);
void unlockShared(rwlock *r);
void lockExclusive(rwlock *r);
void unlockExclusive(rwlock *r);
//void upgrade(rwlock *r);
//void downgrade(rwlock *r);
