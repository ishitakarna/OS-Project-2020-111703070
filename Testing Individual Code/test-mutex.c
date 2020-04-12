#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "func.h"

long c = 0, c1 = 0, c2 = 0, run = 1;
int flag = 0;

mutex_t mutex;

void *thread1(void *arg) {
	while(run == 1) {
		mutex_lock(&mutex);
		printf("lock acq thread 1\n");
		c++;
		mutex_unlock(&mutex);
		printf("lock release thread 1 \n");
		c1++;
	}
}

void *thread2(void *arg) {
	while(run == 1) {
		mutex_lock(&mutex);
		printf("lock acq thread 2\n");
		c++;
		mutex_unlock(&mutex);
		printf("lock release thread 2\n");
		c2++;
	}
    
}

int main() {
	pthread_t th1, th2;

	mutex_init(&mutex);

	pthread_create(&th1, NULL, thread1, NULL);
	pthread_create(&th2, NULL, thread2, NULL);
	printf("Threads created\n");

	sleep(2);
	run = 0;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	printf("Threads joined\n");

	fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
	fflush(stdout);
	return 0;
}
