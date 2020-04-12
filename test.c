#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "func.h"

long long c = 0, c1 = 0, c2 = 0, run = 1;
int flag = 0;

spinlock_t sl;
mutex_t mutex;

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

void testBinarySemaphore() {
    return;
}

void testCountingSemaphore() {
    return;
}

void testRWLock() {
    return;
}

int main() {
    printf("\n --------- TESTING SPINLOCK --------------\n");
    testSpinlock();
    printf("\n --------- TESTING MUTEX -----------------\n");
    testMutex();
    printf("\n --------- TESTING BINARY SEMAPHORE ------\n");
    testBinarySemaphore();
    printf("\n --------- TESTING COUNTING SEMAPHORE ----\n");
    testCountingSemaphore();
    printf("\n --------- TESTING READ WRITE LOCKS ------\n");
    testRWLock();
    return 0;
}

