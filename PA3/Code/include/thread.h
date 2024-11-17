#ifndef THREAD_H
#define THREAD_H

#include <signal.h>
#include <setjmp.h>
#include "util.h"
#include "threadHead.h"
#include "queue.h"

#define STACK_SIZE 4096 // in bytes
#define MAX_THREADS 64  // maximum number of threads

// global variables :

extern thread_t *current_thread;
extern int numberOfThreads;
extern thread_queue_t *readyQueue;
extern thread_queue_t *waitQueue;

void init_lib();
int thread_create(thread_t *thread, void(*start_routine));
void thread_exit();
void thread_join(thread_t *thread);
void thread_sleep(unsigned int milliseconds);
void thread_yield(void);
void context_switch(thread_t *prev, thread_t *next);

#endif // THREAD_H
