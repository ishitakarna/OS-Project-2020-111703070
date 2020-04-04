#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "func.h"

#define NO_OF_THREADS 10

long c = 0, run = 1;

rwlock rw;
//pthread_rwlock_t rwlock1;

void *writeData(void *i) {
    int a = *((int *) i);

    lockExclusive(&rw);
    //pthread_rwlock_wrlock(&rwlock1);
    printf("\nTh%d Exclusive lock, changing c \n", a);
    c++;
    printf("Active : %d Pending readers : %d Pending writers : %d\n", rw.nActive, rw.nPendingReads, rw.nPendingWrites);
    printf("Th%d Exclusive lock unlocking; wake all readers or single writer\n", a);
    unlockExclusive(&rw);
    //pthread_rwlock_unlock(&rwlock1);
}

void *readData(void *i) {
    int a = *((int *) i);

    lockShared(&rw);
    //pthread_rwlock_rdlock(&rwlock1);
    printf("\nTh%d Shared lock, reading c\n", a);
    printf("c : %ld\n", c);
    printf("Active : %d Pending readers : %d Pending writers : %d\n", rw.nActive, rw.nPendingReads, rw.nPendingWrites);
    printf("Th%d Shared lock unlocking; other readers continue or wake a single writer\n", a);
    unlockShared(&rw);
    //pthread_rwlock_rdlock(&rwlock1);

}


int main() {
	pthread_t threads[NO_OF_THREADS];
    int i;

    initRwLock(&rw);
    //pthread_rwlock_init(&rwlock1, NULL);

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
    return 0;
}