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
    thread_t *main_t = malloc(sizeof(thread_t));
    const char *input_file_path = "test/laundry/inputs/3.in"; // Replace with the actual path to your input file
    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    int n, m;
    if (fscanf(input_file, "%d %d", &n, &m) != 2)
    {
        fprintf(stderr, "Error reading values from the input file.\n");
        fclose(input_file);
        return 1;
    }
    printf("the value of n = %d : m = %d \n", n, m);
    sem_init(&laundries, n);
    thread_t *threads[n];
    thread_create(main_t, NULL);

    // thread_t **threads = malloc((m + 1) * sizeof(thread_t *));
    int values[n];
    // threads[0] = malloc(sizeof(thread_t));
    for (int i = 0; i < m; i++)
    {
        // threads[i + 1] = malloc(sizeof(thread_t));
        // if (threads[i + 1] == NULL)
        // {
        //     fprintf(stderr, "Error allocating memory for thread %d.\n", i);
        //     return 1;
        // }
        // if (fscanf(input_file, "%d", &values[i]) != 1)
        // {
        //     fprintf(stderr, "Error reading value %d from the input file.\n", i);
        //     fclose(input_file);
        //     return 1;
        // }
        // threads[i + 1]->appointmentDuration = values[i];
        // threads[i + 1]->appointmentTime = 0;
    }
    init_lib();
    thread_create(threads[0], NULL);

    for (int i = 1; i < m + 1; i++)
    {
        thread_create(threads[i], &fun1);
    }

    timer_start();

    thread_yield();

    for (int i = 1; i < m + 1; i++)
    {
        thread_join(threads[i]);
    }

    printf("All customers have left.\n");
    timer_stop();

    fclose(input_file);

    // Now, you have the values in the `n`, `m`, and `values` array.
    // Do something with the data...

    return 0;
}