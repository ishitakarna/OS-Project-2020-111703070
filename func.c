#include "func.h"

/*---Spinlock---*/
void spin_lock(spinlock_t *s) {
    while(test_and_set(s) != 0);
}

void spin_unlock (spinlock_t *s) {
    s->x = 0;
}

/*---Semaphore---*/
void initsem(semaphore *sem, int val) {
    sem->x = val;
    sem->sl.x = 0; 
}

void P(semaphore *sem); //acquire semaphore 
void V(semaphore *sem); //release semaphore 

/*---Functionality---*/
void wait(condition *c, spinlock_t *s) {
    spin_lock(&c->listLock);
    //add self to linked list 
    spin_unlock(&c->listLock);
    spin_unlock(s); //release spinlock before blocking 
    //perform context switch 
    spin_lock(s);
    return;
}

void do_signal(condition *c) { //wake up one thread
    spin_lock(&c->listLock);
    //remove one thread from linked list if non empty 
    spin_unlock(&c->listLock);
    //thread removed -> make it runnable 
    return;
}

void do_broadcast(condition *c) { //wake up all threads 
    spin_lock(&c->listLock);
    //while(linked list is non empty) {
        //remove a thread from linked list 
        //make it runnable
    //}
    spin_unlock(&c->listLock);
}

/*---Read Write Locks ---*/
void lockShared(rwlock *r) {
    spin_lock(&r->sl);
    r->nPendingReads++;
    if(r->nPendingWrites > 0)
        wait(&r->canRead, &r->sl); //no starving writers 
    while(r->nActive < 0) //exclusive lock present
        wait(&r->canRead, &r->sl); 
    r->nActive++;
    r->nPendingReads--;
    spin_unlock(&r->sl);
}

void unlockShared(rwlock *r) {
    spin_lock(&r->sl);
    r->nActive--;
    if(r->nActive == 0) { //no other readers active
        spin_unlock(&r->sl);
        do_signal(&r->canWrite);
    }
    else 
        spin_unlock(&r->sl);
}

void lockExclusive(rwlock *r) {
    spin_lock(&r->sl);
    r->nPendingWrites++;
    while(r->nActive)
        wait(&r->canWrite, &r->sl);
    r->nPendingWrites--;
    r->nActive = -1;
    spin_unlock(&r->sl);
}

void unlockExclusive(rwlock *r) {
    int wakeReaders;
    spin_lock(&r->sl);
    r->nActive = 0;
    wakeReaders = (r->nPendingReads != 0);
    spin_unlock(&r->sl);
    if(wakeReaders)
        do_broadcast(&r->canRead);
    else
        do_signal(&r->canWrite); 
}