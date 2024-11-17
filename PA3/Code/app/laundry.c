#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

#include <stdio.h>
#include <dirent.h>

semaphore laundries;
extern thread_t *current_thread;

void fun1()
{
    write_formatted("Customer %d got in line\n", current_thread->thread_id);
    sem_wait(&laundries);
    write_formatted("Customer %d starting washing their clothes\n", current_thread->thread_id);
    thread_sleep(current_thread->appointmentDuration);
    write_formatted("Customer %d finished washing their clothes\n", current_thread->thread_id);
    sem_post(&laundries);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));

    int n, m;
    if (scanf("%d %d", &n, &m) != 2)
    {
        LOG_OUT("Error reading values from input.\n");
        return 1;
    }
    LOG_OUT("the value of n = %d : m = %d \n", n, m);
    sem_init(&laundries, n);
    thread_t *threads[m];
    thread_create(main_t, NULL);

    int values[m];
    for (int i = 0; i < m; i++)
    {
        threads[i] = malloc(sizeof(thread_t));
        if (threads[i] == NULL)
        {
            LOG_OUT("Error allocating memory for thread %d.\n", i);
            return 1;
        }
        if (scanf("%d", &values[i]) != 1)
        {
            LOG_OUT("Error reading value %d from input.\n", i);
            return 1;
        }
        threads[i]->appointmentDuration = values[i];
        threads[i]->appointmentTime = 0;
    }

    for (int i = 0; i < m; i++)
    {
        thread_create(threads[i], &fun1);
    }

    timer_start();

    thread_yield();

    for (int i = 0; i < m; i++)
    {
        thread_join(threads[i]);
    }

    LOG_OUT("All customers have left.\n");
    timer_stop();

    // Now, you have the values in the `n`, `m`, and `values` array.
    // Do something with the data...

    return 0;
}