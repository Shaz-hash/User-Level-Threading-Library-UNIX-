
#ifndef THREAD_HEAD_H
#define THREAD_HEAD_H

#include <signal.h>
#include <setjmp.h>
#include "util.h"

enum THREAD_STATES
{
    RUNNING,
    READY,
    BLOCKED,
    COMPLETED
};

typedef struct thread
{
    int thread_id;
    enum THREAD_STATES state;
    void *stack;
    addr_t rsp;
    addr_t pc;
    int numberOfWaitingThreads;
    int maxThreadSize;
    struct thread **listOfWaitingThreads;
    sigjmp_buf context;
    int appointmentTime;
    int appointmentDuration;
} thread_t; // struct for defining thread control block

#endif
