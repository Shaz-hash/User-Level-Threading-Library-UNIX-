#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

int test_val = 0;
semaphore sem1, sem2;

void fun1()
{
    LOG_OUT("Thread %d trying to acquire semaphore1\n", current_thread->thread_id);
    sem_wait(&sem1);
    LOG_OUT("Thread %d acquired semaphore1\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Thread %d releasing semaphore1\n", current_thread->thread_id);
    sem_post(&sem1);

    LOG_OUT("Thread %d trying to acquire semaphore2\n", current_thread->thread_id);
    sem_wait(&sem2);
    LOG_OUT("Thread %d acquired semaphore2\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Thread %d releasing semaphore2\n", current_thread->thread_id);
    sem_post(&sem2);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

void fun2()
{
    LOG_OUT("Thread %d trying to acquire semaphore1\n", current_thread->thread_id);
    sem_wait(&sem1);
    LOG_OUT("Thread %d acquired semaphore1\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Thread %d releasing semaphore1\n", current_thread->thread_id);
    sem_post(&sem1);

    LOG_OUT("Thread %d trying to acquire semaphore2\n", current_thread->thread_id);
    sem_wait(&sem2);
    LOG_OUT("Thread %d acquired semaphore2\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Thread %d releasing semaphore2\n", current_thread->thread_id);
    sem_post(&sem2);

    // Nested semaphore acquisition and release
    LOG_OUT("Thread %d trying to acquire semaphore1 (nested)\n", current_thread->thread_id);
    sem_wait(&sem1);
    LOG_OUT("Thread %d acquired semaphore1 (nested)\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Thread %d releasing semaphore1 (nested)\n", current_thread->thread_id);
    sem_post(&sem1);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t *t1, *t2;

    sem_init(&sem1, 1);
    sem_init(&sem2, 1);

    t1 = malloc(sizeof(thread_t));
    t2 = malloc(sizeof(thread_t));

    thread_create(main_t, NULL);

    thread_create(t1, &fun1);
    thread_create(t2, &fun2);

    timer_start();

    thread_yield();

    thread_join(t2);
    thread_join(t1);
    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
