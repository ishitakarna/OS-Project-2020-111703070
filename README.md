# Synchronization Library in C & Linux from scratch
# OS-Project-2020-111703070


## Linux from scratch
**Linux from scratch system implemented using Linux host system on virtual box.**

    Grub
![Grub](https://user-images.githubusercontent.com/43277968/78422158-ef3c1000-767a-11ea-9ca5-44441197c25e.png)

    Login Prompt
![Login](https://user-images.githubusercontent.com/43277968/78422174-1692dd00-767b-11ea-8d7a-bbaa669b32aa.png)

    '/' Folder
![MainFolder](https://user-images.githubusercontent.com/43277968/78422188-30342480-767b-11ea-8ef2-537ec3c259c8.png)

    Network configured for active internet connection
![InternetWorking](https://user-images.githubusercontent.com/43277968/78422194-3c1fe680-767b-11ea-81a0-322d13e33ebe.png)


##  Synchronization Library
**Implementing a C library with various synchronization primitives namely spinlocks, semaphores, mutexes and read-write locks.**

### How to Run 
1. make 
2. ./project 

#### Spinlock
* Data Type : spinlock_t
* Functions : Init, Lock, Unlock 

#### Mutex
* Data Type : mutex_t
* Functions : Init, Lock, Unlock

#### Semaphore
* Data Type : semaphore_t
* Functions : Init, Acquire, Release

#### Read Write locks
* Data Type : rwlock
* Functions : Init, LockShared, UnlockShared, LockExclusive, UnlockExclusive

#### Testing 

   Spinlock, mutex, binary semaphore and counting semaphore(n = 2) tested 
![Testing1](https://user-images.githubusercontent.com/43277968/79074204-adf5d100-7d08-11ea-8db8-c0393c00b428.png)

    Read Write Locks tested 
![Testing2](https://user-images.githubusercontent.com/43277968/79074284-26f52880-7d09-11ea-8fec-81c3d8527b18.png)

    Read Write Lock testing with detailed information
![Testing3](https://user-images.githubusercontent.com/43277968/79074400-c3b7c600-7d09-11ea-994f-36d171aa5dce.png)



