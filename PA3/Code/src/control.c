#include "control.h"
#include "thread.h"
#include "queue.h"
#include "util.h"
#include <stdio.h>
#include "time_control.h"
#include "control.h"

void mutex_init(mutex *m)
{
    m->lockValue = (int *)malloc(sizeof(int));
    m->waitQueue = (thread_queue_t *)malloc(sizeof(thread_queue_t));
    m->lockValue[0] = 0;
    m->numberOfThreadsinWaiting = 0;
}

void mutex_acquire(mutex *m)
{
    // diable the interrupts
    setInterrupts(1);
    // change
    resetTimer();
    m->numberOfThreadsinWaiting++;
    if (m->lockValue[0] == 0)
    {
        // write_formatted("yep1\n");
        m->lockValue[0] = 1;
        setInterrupts(0);
        return;
    }
    else
    {
        current_thread->state = BLOCKED;
        enqueue_thread(m->waitQueue, current_thread);

        // performing the context switch where the next thread in ready queue can take over :
        int ret_val = sigsetjmp(current_thread->context, 1);
        // write_formatted("yep1\n");
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
}

void mutex_release(mutex *m)
{
    setInterrupts(1);
    m->numberOfThreadsinWaiting = m->numberOfThreadsinWaiting - 1;
    // write_formatted("number of waiting left :  %d \n", m->numberOfThreadsinWaiting);

    if (m->numberOfThreadsinWaiting == 0)
    {
        // write_formatted("yep1\n");
        m->lockValue[0] = 0;
        setInterrupts(0);
    }
    else
    {
        // releasing a thread from lock's queue to the ready queue :
        // write_formatted("yep1\n");
        thread_t *releasedThread = dequeue_thread(m->waitQueue);
        if (releasedThread == NULL)
        {
            write_formatted("yep1\n");
        }
        releasedThread->state = READY;
        enqueue_thread(readyQueue, releasedThread);
        setInterrupts(0);
    }
}

void sem_init(semaphore *sem, int value)
{
    // write_formatted("yep1\n");
    sem->lock = (mutex *)malloc(sizeof(mutex));
    mutex_init(sem->lock);
    sem->value = value;
    sem->waitQueue = (thread_queue_t *)malloc(sizeof(thread_queue_t));
}

void sem_wait(semaphore *sem)
{
    // mutex_acquire(sem->lock);
    setInterrupts(1);

    sem->value = sem->value - 1;
    if (sem->value < 0)
    {

        current_thread->state = BLOCKED;
        enqueue_thread(sem->waitQueue, current_thread);
        // need to perform the context switching :
        // timer_stop();
        // timer_start();
        // resetTimer();
        int ret_val = sigsetjmp(current_thread->context, 1);
        if (ret_val == 0)
        {
            // write_formatted("yep2\n");
            current_thread = dequeue_thread(readyQueue);
            current_thread->state = RUNNING;
            // write_formatted("THREAD ID : %d \n", current_thread->thread_id);
            resetTimer();
            setInterrupts(0);
            siglongjmp(current_thread->context, 1);
        }
        else
        {
            // write_formatted("yep1\n");
            setInterrupts(0);
            return;
        }
    }
    else
    {
        // write_formatted("yep1\n");
        setInterrupts(0);
    }
}

void sem_post(semaphore *sem)
{
    setInterrupts(1);
    // write_formatted("yep1\n");
    sem->value = sem->value + 1;
    if (sem->value > 0)
    {
        // write_formatted("yep2\n");
        setInterrupts(0);
        return;
    }
    else
    {
        //
        // write_formatted("yep1\n");
        thread_t *releasedThread = dequeue_thread(sem->waitQueue);
        releasedThread->state = READY;
        enqueue_thread(readyQueue, releasedThread);
        setInterrupts(0);
        return;
    }
}
