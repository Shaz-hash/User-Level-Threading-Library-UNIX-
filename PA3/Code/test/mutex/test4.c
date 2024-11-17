#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

int test_val = 0;
mutex m;

void fun1()
{
    LOG_OUT("Thread %d trying to acquire mutex\n", current_thread->thread_id);
    mutex_acquire(&m);
    LOG_OUT("Thread %d acquired mutex\n", current_thread->thread_id);
    if (current_thread->thread_id < 5)
    {
        test_val++;
        thread_yield();
    }
    LOG_OUT("Thread %d releasing mutex\n", current_thread->thread_id);
    mutex_release(&m);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t **t = malloc(sizeof(thread_t *) * 10);

    for (int i = 0; i < 10; i++)
    {
        t[i] = malloc(sizeof(thread_t));
    }

    mutex_init(&m);

    thread_create(main_t, NULL);

    for (int i = 0; i < 10; i++)
    {
        thread_create(t[i], &fun1);
    }

    timer_start();

    thread_yield();

    for (int i = 0; i < 10; i++)
    {
        thread_join(t[i]);
    }

    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
