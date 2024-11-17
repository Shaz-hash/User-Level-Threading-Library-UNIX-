#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

#include <stdio.h>
#include <dirent.h>

mutex doctor;
extern thread_t *current_thread;

void fun1()
{
    LOG_OUT("Patient %d got in line\n", current_thread->thread_id);
    thread_sleep(current_thread->appointmentTime);
    mutex_acquire(&doctor);
    LOG_OUT("Patient %d starting seeing the doctor\n", current_thread->thread_id);
    thread_sleep(current_thread->appointmentDuration);
    LOG_OUT("Patient %d finished seeing the doctor\n", current_thread->thread_id);
    mutex_release(&doctor);
    thread_exit();
}

int main()
{
    init_lib();
    mutex_init(&doctor);
    thread_t *main_t = malloc(sizeof(thread_t));

    // char input_file_path[] = "test/doctor/inputs/3.in"; // replace with the actual path to your file

    // FILE *input_file = fopen(input_file_path, "r");
    // if (input_file == NULL)
    // {
    //     fLOG_OUT(stderr, "Error opening input file: %s\n", input_file_path);
    //     return 1;
    // }

    int n; // Number of elements
    if (scanf("%d", &n) != 1)
    {
        LOG_OUT("Error reading number of elements from the input file\n");
        return 1;
    }

    int arr1[n], arr2[n];
    thread_t *threads[n];
    thread_create(main_t, NULL);
    for (int i = 0; i < n; i++)
    {
        int value1, value2;
        if (scanf("%d %d", &value1, &value2) != 2)
        {
            LOG_OUT("Error reading input values from the input file\n");
            return 1;
        }
        arr1[i] = value1;
        arr2[i] = value2;

        // creating zombie threads :
        thread_t *t = malloc(sizeof(thread_t));
        t->appointmentTime = value1;
        t->appointmentDuration = value2;
        threads[i] = t;
        thread_create(t, &fun1);
        // LOG_OUT("the value is %d", arr1[i]);
    }

    timer_start();

    thread_yield();

    for (int i = 0; i < n; i++)
    {
        thread_join(threads[i]);
    }

    LOG_OUT("All patients have left.\n");
    timer_stop();

    // fclose(input_file);
    return 0;
}