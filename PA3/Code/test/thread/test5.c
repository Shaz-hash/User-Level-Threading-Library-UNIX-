#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

void fun1()
{
    for (int i = 0; i < 5; i++)
    {
        LOG_OUT("Thread %d: %d\n", current_thread->thread_id, i);
        thread_yield();
    }

    thread_exit();
}

void fun2()
{
    thread_t *t1, *t2, *t3, *t4;
    t1 = malloc(sizeof(thread_t));
    t2 = malloc(sizeof(thread_t));
    t3 = malloc(sizeof(thread_t));
    t4 = malloc(sizeof(thread_t));

    thread_create(t1, &fun1);
    thread_create(t2, &fun1);
    thread_create(t3, &fun1);
    thread_create(t4, &fun1);

    LOG_OUT("Thread %d created four threads\n", current_thread->thread_id);

    thread_join(t1);
    thread_join(t2);
    thread_join(t3);
    thread_join(t4);
    LOG_OUT("Thread %d joined four threads\n", current_thread->thread_id);

    thread_exit();
}

void fun3()
{
    LOG_OUT("Thread %d sleeping for 1 second\n", current_thread->thread_id);
    thread_sleep(1000);
    LOG_OUT("Thread %d woke up\n", current_thread->thread_id);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t *t1, *t2;

    t1 = malloc(sizeof(thread_t));
    t2 = malloc(sizeof(thread_t));

    thread_create(main_t, NULL);

    thread_create(t1, &fun2);
    thread_create(t2, &fun3);

    timer_start();

    thread_yield();

    thread_join(t1);
    thread_join(t2);
    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
