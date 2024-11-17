// بِسْمِ اللهِ الرَّحْمٰنِ الرَّحِيْمِ

#include "thread.h"
#include "queue.h"
#include "jmpbuf-offsets.h"
#include "util.h"
#include "time_control.h"
#include "threadHead.h"

extern struct itimerval timer;

thread_t *current_thread = NULL;
int numberOfThreads = 0;
int threadID = -1;
thread_queue_t *readyQueue = NULL;
thread_queue_t *waitQueue = NULL;
thread_queue_t *finishQueue = NULL;

sigjmp_buf env;

// function for the initialization of thread :

void init_lib()
{
    current_thread = NULL;
    numberOfThreads = 0;
    readyQueue = (thread_queue_t *)malloc(sizeof(thread_queue_t));
    waitQueue = (thread_queue_t *)malloc(sizeof(thread_queue_t));
    finishQueue = (thread_queue_t *)malloc(sizeof(thread_queue_t));
    init_thread_queue(readyQueue);
    init_thread_queue(waitQueue);
}

// create thread function :

int thread_create(thread_t *thread, void(*start_routine))
{
    thread->stack = malloc(STACK_SIZE);
    thread->rsp = (addr_t)thread->stack + STACK_SIZE - sizeof(int);
    thread->pc = (addr_t)start_routine;
    numberOfThreads++;
    threadID++;
    thread->maxThreadSize = 10;
    thread->thread_id = threadID;
    thread->listOfWaitingThreads = (thread_t **)malloc(10 * sizeof(thread_t *));
    thread->numberOfWaitingThreads = 0;

    if (numberOfThreads == 1)
    {
        current_thread = thread;
        current_thread->state = RUNNING;
    }
    else
    {
        sigsetjmp(thread->context, 1);
        thread->context->__jmpbuf[JB_RSP] = mangle(thread->rsp);
        thread->context->__jmpbuf[JB_PC] = mangle(thread->pc);
        // sigemptyset(&env[1]->__saved_mask);
        sigemptyset(&(thread->context->__saved_mask));

        // enqueing it into my queue :
        enqueue_thread(readyQueue, thread);
        thread->state = READY;
    }
    return threadID;
}

void context_switch(thread_t *prev, thread_t *next)
{
    // reset the timer ::
    // write_formatted("yess3\n");

    setInterrupts(1);
    resetTimer();
    //   could be possible that current thread is null as it just exited --> cater to that case pls.

    int ret_val = sigsetjmp(current_thread->context, 1);
    // write_formatted("yep10\n");
    if (ret_val == 0)
    {
        // changing the current thread to next one :
        prev->state = READY;
        enqueue_thread(readyQueue, prev);
        // write_formatted("yess4\n");

        // upading the current thread
        next->state = RUNNING;
        current_thread = next;

        setInterrupts(0);
        siglongjmp(current_thread->context, 1);
    }
    else
    {
        setInterrupts(0);
    }
    return;
}

void thread_exit()
{
    // perform context switch with null
    // write_formatted("MY ID IS : %d \n", current_thread->thread_id);
    setInterrupts(1);
    resetTimer();
    int counter = 0;
    while (counter < current_thread->numberOfWaitingThreads)
    {
        // write_formatted("yep0\n");
        thread_t *ptr = current_thread->listOfWaitingThreads[counter];
        remove_thread_from_queue(waitQueue, ptr);
        enqueue_thread(readyQueue, ptr);
        counter++;
    }
    // free(current_thread->listOfWaitingThreads);
    // current_thread->listOfWaitingThreads = NULL;
    // current_thread->numberOfWaitingThreads = 0;

    current_thread->state = COMPLETED;
    enqueue_thread(finishQueue, current_thread);
    numberOfThreads = numberOfThreads - 1;
    // performing the context switching :
    // write_formatted("yep1\n");
    // free(current_thread);
    current_thread = NULL;

    if (is_thread_queue_empty(readyQueue))
    {
        // write_formatted("yep0\n");
        // write_formatted("yep1\n");
        // write_formatted("MY ID IS : %d \n", current_thread->thread_id);
        setInterrupts(0);
        return;
    }
    else
    {
        // free(current_thread);

        if (is_thread_queue_empty(readyQueue))
        {
            // write_formatted("yep3\n");
            setInterrupts(0);
        }
        else
        {
            // write_formatted("yep4\n");
            current_thread = dequeue_thread(readyQueue);
            if (current_thread == NULL)
            {
                write_formatted("wow");
                return;
            }
            // write_formatted("yep4 : %p\n", (void *)current_thread);
            // write_formatted("MY ID IS : %d \n", current_thread->thread_id);
            current_thread->state = READY;
            // write_formatted("yep4\n");

            setInterrupts(0);
            siglongjmp(current_thread->context, 1);
        }
    }
}

void thread_join(thread_t *thread)
{
    // write_formatted("yes i am here %d \n", thread->thread_id);
    if (thread->state == COMPLETED)
    {
        // write_formatted("yes i am here\n");
        return;
    }
    resetTimer();
    setInterrupts(1);

    thread->numberOfWaitingThreads = thread->numberOfWaitingThreads + 1;
    if (thread->numberOfWaitingThreads > thread->maxThreadSize)
    {

        int newMaxSize = thread->maxThreadSize + 10;
        thread_t **newList = (thread_t **)malloc(newMaxSize * sizeof(thread_t *));

        if (newList != NULL)
        {
            for (int i = 0; i < thread->numberOfWaitingThreads; i++)
            {
                newList[i] = thread->listOfWaitingThreads[i];
            }
            free(thread->listOfWaitingThreads);
            thread->maxThreadSize = newMaxSize;
            thread->listOfWaitingThreads = newList;
        }
        else
        {
            setInterrupts(0);
            // write_formatted("Couldnt add the thread into the list of thread on whcih join is called\n");
            return;
        }
    }
    thread->listOfWaitingThreads[thread->numberOfWaitingThreads - 1] = current_thread;
    current_thread->state = BLOCKED;
    enqueue_thread(waitQueue, current_thread);

    // Performing context switch :

    int ret_val = sigsetjmp(current_thread->context, 1);
    if (ret_val == 0)
    {
        current_thread = dequeue_thread(readyQueue);
        current_thread->state = READY;
        setInterrupts(0);
        siglongjmp(current_thread->context, 1);
    }
    else
    {
        // write_formatted("MY ID IS : %d \n", current_thread->thread_id);
        setInterrupts(0);
        return;
    }
}

void thread_yield(void)
{
    interrupt_generated(10);
}

void thread_sleep(unsigned int milliseconds)
{
    if (milliseconds == 0)
    {
        return;
    }

    unsigned int startTime = 0;

    unsigned int finalTime = get_time() + milliseconds;
    // write_formatted("finalTine %d\n", finalTime);
    // write_formatted("starttime %d\n", startTime);
    while (startTime < finalTime)
    {
        // write_formatted("..MY ID = %d..\n", current_thread->thread_id);
        startTime = get_time();
    }
}
