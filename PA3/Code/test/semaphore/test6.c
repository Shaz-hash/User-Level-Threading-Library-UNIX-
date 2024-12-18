#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

int test_val = 0;
semaphore sem;

void fun1()
{
    LOG_OUT("Thread %d trying to acquire semaphore\n", current_thread->thread_id);
    sem_wait(&sem);
    LOG_OUT("Thread %d acquired semaphore\n", current_thread->thread_id);
    test_val++;
    if (current_thread->thread_id == 1)
    {
        thread_sleep(1000);
    }
    LOG_OUT("Thread %d releasing semaphore\n", current_thread->thread_id);
    sem_post(&sem);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

void fun2()
{
    LOG_OUT("Thread %d trying to acquire semaphore\n", current_thread->thread_id);
    sem_wait(&sem);
    LOG_OUT("Thread %d acquired semaphore\n", current_thread->thread_id);
    test_val++;
    if (current_thread->thread_id != 1)
    {
        thread_sleep(500);
    }
    LOG_OUT("Thread %d releasing semaphore\n", current_thread->thread_id);
    sem_post(&sem);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

void fun3()
{
    LOG_OUT("Thread %d trying to acquire semaphore\n", current_thread->thread_id);
    sem_wait(&sem);
    LOG_OUT("Thread %d acquired semaphore\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Thread %d releasing semaphore\n", current_thread->thread_id);
    sem_post(&sem);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

void fun4()
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        LOG_OUT("Thread %d trying to acquire semaphore\n", current_thread->thread_id);
        sem_wait(&sem);
        LOG_OUT("Thread %d acquired semaphore\n", current_thread->thread_id);
        test_val++;
        LOG_OUT("Thread %d releasing semaphore\n", current_thread->thread_id);
        sem_post(&sem);
    }

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t *t1, *t2, *t3, *t4;

    sem_init(&sem, 1);

    t1 = malloc(sizeof(thread_t));
    t2 = malloc(sizeof(thread_t));
    t3 = malloc(sizeof(thread_t));
    t4 = malloc(sizeof(thread_t));

    thread_create(main_t, NULL);

    thread_create(t1, &fun1);
    thread_create(t2, &fun2);
    thread_create(t3, &fun3);
    thread_create(t4, &fun4);

    timer_start();

    thread_yield();

    thread_join(t4);
    thread_join(t3);
    thread_join(t2);
    thread_join(t1);
    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
