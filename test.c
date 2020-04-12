#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "func.h"

long long c = 0, c1 = 0, c2 = 0, run = 1;
int flag = 0;

spinlock_t sl;
mutex_t mutex;
semaphore_t bsem, csem;
rwlock rw;

/*---- SPINLOCK TESTING ----*/
void *thread1_Spin(void *arg) {
	while(run == 1) {
        spin_lock(&sl);
        //printf("lock acq thread 1\n");
        c++;
        spin_unlock(&sl);
        //printf("lock release thread 1\n");
        c1++;
	}
    pthread_exit(NULL);
}

void *thread2_Spin(void *arg) {
	while(run == 1) {
        spin_lock(&sl);
        //printf("lock acq thread 2\n");
        c++;
        spin_unlock(&sl);
        //printf("lock release thread 2\n");
        c2++;
	}
    pthread_exit(NULL);
}

void testSpinlock() {
    pthread_t th1, th2;

    spin_init(&sl);

    pthread_create(&th1, NULL, thread1_Spin, NULL);
	pthread_create(&th2, NULL, thread2_Spin, NULL);
    printf("Threads created\n");

	sleep(2);
	run = 0;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
    printf("Threads joined\n");

	fprintf(stdout, "c = %lld c1+c2 = %lld c1 = %lld c2 = %lld \n", c, c1+c2, c1, c2);
	fflush(stdout);
    return;
}

/*--- MUTEX TESTING ---*/
void *thread1_Mutex(void *arg) {
	while(run == 1) {
		mutex_lock(&mutex);
		//printf("lock acq thread 1\n");
		c++;
		mutex_unlock(&mutex);
		//printf("lock release thread 1 \n");
		c1++;
	}
    pthread_exit(NULL);
}

void *thread2_Mutex(void *arg) {
	while(run == 1) {
		mutex_lock(&mutex);
		//printf("lock acq thread 2\n");
		c++;
		mutex_unlock(&mutex);
		//printf("lock release thread 2\n");
		c2++;
	}
    pthread_exit(NULL);
}

void testMutex() {
    pthread_t th1, th2;
    c = 0, c1 = 0, c2 = 0, run = 1;
    flag = 0;

	mutex_init(&mutex);

	pthread_create(&th1, NULL, thread1_Mutex, NULL);
	pthread_create(&th2, NULL, thread2_Mutex, NULL);
	printf("Threads created\n");

	sleep(2);
	run = 0;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	printf("Threads joined\n");

	fprintf(stdout, "c = %lld c1+c2 = %lld c1 = %lld c2 = %lld \n", c, c1+c2, c1, c2);
	fflush(stdout);
    return;
}

/* --- TESTING BINARY SEMAPHORE ---*/
void *thread1_BSem(void *arg) {
	while(run == 1) {
        sem_acquire(&bsem);
        //printf("lock acq thread 1\n");
        c++;
        sem_release(&bsem);
        //printf("lock release thread 1 \n");
        c1++;
	}
    pthread_exit(NULL);
}

void *thread2_BSem(void *arg) {
	while(run == 1) {
        sem_acquire(&bsem);
        //printf("lock acq thread 2\n");
        c++;
        sem_release(&bsem);
        //printf("lock release thread 2\n");
        c2++;
	}
    pthread_exit(NULL);   
}

void testBinarySemaphore() {
    pthread_t th1, th2;
    c = 0, c1 = 0, c2 = 0, run = 1;
    flag = 0;

    sem_init(&bsem, 1);

    pthread_create(&th1, NULL, thread1_BSem, NULL);
	pthread_create(&th2, NULL, thread2_BSem, NULL);
    printf("Threads created\n");

	sleep(2);
	run = 0;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
    printf("Threads joined\n");

	fprintf(stdout, "c = %lld c1+c2 = %lld c1 = %lld c2 = %lld \n", c, c1+c2, c1, c2);
	fflush(stdout);
    return;
}

/* ---- TESTING COUNTING SEMAPHORE --- */
void *thread1(void *arg) {
	while(run == 1) {
        sem_acquire(&csem);
        printf("Thread %d acquired lock : in critical section \n", 1);
        sleep(2);
        printf("Thread %d released\n", 1);
        sem_release(&csem);

	}
    pthread_exit(NULL);
}
void *thread2(void *arg) {
	while(run == 1) {
        sem_acquire(&csem);
        printf("Thread %d acquired lock : in critical section \n", 2);
        sleep(2);
        printf("Thread %d released\n", 2);
        sem_release(&csem);
	}
    pthread_exit(NULL);
}
void *thread3(void *arg) {
	while(run == 1) {
        sem_acquire(&csem);
        printf("Thread %d acquired lock : in critical section \n", 3);
        sleep(2);
        printf("Thread %d released\n", 3);
        sem_release(&csem);
	}
    pthread_exit(NULL);
}

void testCountingSemaphore() {
    pthread_t th1, th2, th3;
    run = 1;

    sem_init(&csem, 2); //2 can aquire lock at a time 

    pthread_create(&th1, NULL, thread1, NULL);
	pthread_create(&th2, NULL, thread2, NULL);
    pthread_create(&th3, NULL, thread3, NULL);

	sleep(5);
	run = 0;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    printf("Threads joined\n");

	fflush(stdout);
    return;
}

/* --- TESTING READ WRITE LOCKS --- */
#define NO_OF_THREADS 10

void *writeData(void *i) {
    int a = *((int *) i);

    lockExclusive(&rw);
    printf("\nTh%d Exclusive lock, changing c \n", a);
    c++;
    printf("Active : %d Pending readers : %d Pending writers : %d\n", rw.nActive, rw.nPendingReads, rw.nPendingWrites);
    printf("Th%d Exclusive lock unlocking; wake all readers or single writer\n", a);
    unlockExclusive(&rw);

    pthread_exit(NULL);
}

void *readData(void *i) {
    int a = *((int *) i);

    lockShared(&rw);
    printf("\nTh%d Shared lock, reading c\n", a);
    printf("Th%d c : %lld\n", a, c);
    printf("Active : %d Pending readers : %d Pending writers : %d\n", rw.nActive, rw.nPendingReads, rw.nPendingWrites);
    printf("Th%d Shared lock unlocking; other readers continue or wake a single writer\n", a);
    unlockShared(&rw);

    pthread_exit(NULL);
}

void testRWLock() {
    c = 0, run = 1;
    pthread_t threads[NO_OF_THREADS];
    int i;

    initRwLock(&rw);

    //Threads requesting read and write permissions alternately 
    for(i = 0; i < NO_OF_THREADS; i++) {
        if(i%2 == 0)
            pthread_create(&threads[i], NULL, writeData, (void*)&i);
        else
            pthread_create(&threads[i], NULL, readData, (void*)&i);
    }
    
	sleep(2);
	run = 0;
	for(i = 0; i < NO_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Threads joined\n");

	fflush(stdout);
    return;
}

int main() {
    printf("\n --------- TESTING SPINLOCK --------------\n");
    testSpinlock();
    printf("\n --------- TESTING MUTEX -----------------\n");
    testMutex();
    printf("\n --------- TESTING BINARY SEMAPHORE ------\n");
    testBinarySemaphore();
    printf("\n --------- TESTING COUNTING SEMAPHORE (n = 2) ----\n");
    testCountingSemaphore();
    printf("\n --------- TESTING READ WRITE LOCKS ------\n");
    testRWLock();
    return 0;
}

