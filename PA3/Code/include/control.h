// بِسْمِ اللهِ الرَّحْمٰنِ الرَّحِيْمِ
#ifndef CONTROL_H
#define CONTROL_H

#include "control.h"
#include "thread.h"
#include "util.h"
#include "queue.h"
#include <stdio.h>
#include "time_control.h"

typedef struct mutex_t
{
    thread_queue_t *waitQueue;
    int *lockValue;
    int numberOfThreadsinWaiting;
} mutex;

typedef struct semaphore_t
{
    thread_queue_t *waitQueue;
    int value;
    mutex *lock;

} semaphore;

void mutex_init(mutex *m);
void mutex_acquire(mutex *m);
void mutex_release(mutex *m);

void sem_init(semaphore *sem, int value);
void sem_wait(semaphore *sem);
void sem_post(semaphore *sem);

#endif // CONTROL_H