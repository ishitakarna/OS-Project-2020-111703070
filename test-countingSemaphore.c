#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "func.h"

//TODO : TESTING INCOMPLETE

//long c = 0, c1 = 0, c2 = 0, run = 1;
//int flag = 0;
long run = 1;

semaphore_t sem;

void *thread1(void *arg) {
	while(run == 1) {
        sem_acquire(&sem);
        printf("Thread %d acquired lock : in critical section \n", 1);
        sleep(2);
        sem_release(&sem);
        printf("Thread %d released\n", 1);

	}
}
void *thread2(void *arg) {
	while(run == 1) {
        sem_acquire(&sem);
        printf("Thread %d acquired lock : in critical section \n", 2);
        sleep(2);
        sem_release(&sem);
        printf("Thread %d released\n", 2);

	}
}
void *thread3(void *arg) {
	while(run == 1) {
        sem_acquire(&sem);
        printf("Thread %d acquired lock : in critical section \n", 3);
        sleep(2);
        sem_release(&sem);
        printf("Thread %d released\n", 3);

	}
}

int main() {
	pthread_t th1, th2, th3;

    sem_init(&sem, 2); //2 can aquire lock at a time 

    pthread_create(&th1, NULL, thread1, NULL);
	pthread_create(&th2, NULL, thread2, NULL);
    pthread_create(&th3, NULL, thread3, NULL);
    printf("Threads created\n");

	sleep(5);
	run = 0;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    printf("Threads joined\n");

	//fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
	fflush(stdout);
    return 0;
}