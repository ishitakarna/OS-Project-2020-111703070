#include "func.h"
#include <stdio.h>
#include <linux/futex.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <limits.h>

int futex_addr;

/*---Spinlock---*/
void spin_init(spinlock_t *s) {
    s->val = 0;
}

void spin_lock(spinlock_t *s) {
    while(__sync_lock_test_and_set(&(s->val), 1));
}

void spin_unlock (spinlock_t *s) {
    s->val = 0;
}

/*---Futex---*/
int futex_wait(void* addr, int val){
  return syscall(SYS_futex, &addr ,val, NULL, NULL, 0);
}
int futex_wake(void* addr, int n){
  return syscall(SYS_futex, addr, FUTEX_WAKE, n, NULL, NULL, 0);
}

/*---Semaphore---*/
void sem_init(semaphore_t *sem, int initval) {
    sem->val = initval;
    sem->sl.val = 0; 
}

void block(semaphore_t *sem) {
    futex_addr = 0;
    spin_unlock(&sem->sl);
    futex_wait(&futex_addr, 0);
}

void sem_acquire(semaphore_t *sem) {
    spin_lock(&sem->sl);
    while(sem->val <= 0) {
        block(sem);
        spin_lock(&sem->sl);
    }
    sem->val--;
    spin_unlock(&sem->sl);
} 

void sem_release(semaphore_t *sem) {
    spin_lock(&sem->sl);
    sem->val++;
    futex_wake(&futex_addr, 1);
    spin_unlock(&sem->sl);
} 

/*Mutex*/
void mutex_init(mutex_t *m) {
    sem_init(&m->sem, 1);
}

void mutex_lock(mutex_t *m) {
    sem_acquire(&m->sem);
}

void mutex_unlock(mutex_t *m) {
    sem_release(&m->sem);
}

/*---Functionality---*/
void wait(int type, spinlock_t *s, rwlock *r) {
    if(type == 0) {
        r->futex_read_addr = 0;
        spin_unlock(s); //release spinlock before blocking 
        futex_wait(&r->futex_read_addr, 0);
    }
    else {
        r->futex_write_addr = 0;
        spin_unlock(s); //release spinlock before blocking 
        futex_wait(&r->futex_write_addr, 0);
    }
    spin_lock(s);
    return;
}

/*void do_signal(int type) { //wake up one thread
    spin_lock(&c->listLock);
    //remove one thread from linked list if non empty 
    spin_unlock(&c->listLock);
    //thread removed -> make it runnable

    return;
}

void do_broadcast(int type) { //wake up all threads 
    spin_lock(&c->listLock);
    //while(linked list is non empty) {
        //remove a thread from linked list 
        //make it runnable
    //}
    spin_unlock(&c->listLock);
}
*/

/*---Read Write Locks ---*/
void initRwLock(rwlock *r) {
    r->nActive = 0;
    r->nPendingReads = 0;
    r->nPendingWrites = 0;
    r->sl.val = 0;
}

void lockShared(rwlock *r) {
    spin_lock(&r->sl);
    r->nPendingReads++;
    if(r->nPendingWrites > 0) 
        wait(0, &r->sl, r); //no starving writers 
    while(r->nActive < 0) //exclusive lock present
        wait(0, &r->sl, r); 
    r->nActive++;
    r->nPendingReads--;
    spin_unlock(&r->sl);
}

void unlockShared(rwlock *r) {
    spin_lock(&r->sl);
    r->nActive--;
    if(r->nActive == 0) { //no other readers active
        spin_unlock(&r->sl);
        //do_signal(); 
        futex_wake(&r->futex_write_addr, 1); //wake a single writer 
    }
    else 
        spin_unlock(&r->sl);
}

void lockExclusive(rwlock *r) {
    spin_lock(&r->sl);
    r->nPendingWrites++;
    while(r->nActive) //writer waiting for readers
        wait(1, &r->sl, r);
    r->nPendingWrites--;
    r->nActive = -1;
    spin_unlock(&r->sl);
}

void unlockExclusive(rwlock *r) {
    int wakeReaders;
    spin_lock(&r->sl);
    r->nActive = 0;
    if(r->nPendingReads != 0)
        wakeReaders = 1;
    else
        wakeReaders = 0;

    if(wakeReaders) {
        //do_broadcast(&r->canRead); //wake all readers
        spin_unlock(&r->sl);
        futex_wake(&r->futex_read_addr, INT_MAX);
    }
    else {
        //do_signal(&r->canWrite); //wake single writer
        spin_unlock(&r->sl);
        futex_wake(&r->futex_write_addr, 1); 
    }
}