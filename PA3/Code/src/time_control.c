#include "time_control.h"
#include "util.h"
#include "thread.h"
#include "queue.h"

// the timer for the alarm clock
struct itimerval timer;

/*
    mask used to ignore some signals
    we need to ignore the alarm clock signal when we are doing critical stuff
    an empty masks means all signals are enabled
*/
sigset_t signal_mask;

void interrupt_generated(int signum);

// pass 1 to block and 0 to unblock
void setInterrupts(int button)
{
    if (button)
    {
        sigprocmask(SIG_BLOCK, &signal_mask, NULL);
    }
    else
    {
        sigprocmask(SIG_UNBLOCK, &signal_mask, NULL);
    }
}

int get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);

    return t.tv_sec * 1000 + t.tv_usec / 1000;
}

void timer_start()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = TIME_SLICE;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = TIME_SLICE;

    // when a time slice is over, call the scheduler function
    // by raising the SIGALRM signal
    signal(SIGALRM, interrupt_generated);

    // this mask will be used to mask the alarm signal when we are
    // performing critical tasks that cannot be left midway
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask, SIGALRM); // add only the alarm signal

    // start the timer
    setitimer(ITIMER_REAL, &timer, NULL);
}

void timer_stop()
{
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);
}

// resets the timer
void resetTimer()
{
    timer_stop();
    timer_start();
}

void interrupt_generated(int signum)
{
    if (signum == -100)
    {
        write_formatted("Yesss: %d\n", signum);
    }
    if (!(is_thread_queue_empty(readyQueue)))
    {
        // åresetTimer();
        // write_formatted("Yesss Interrupt: %d\n", signum);
        //   next thread to run :
        thread_t *newThread = dequeue_thread(readyQueue);
        // write_formatted("Yesss2: %d\n", signum);
        context_switch(current_thread, newThread);
    }
}