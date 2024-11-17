
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include "threadHead.h"

#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

// Define the thread structure

// Define a structure for the queue node
typedef struct thread_node
{
    thread_t *data;
    struct thread_node *next;
} thread_node_t;

// Define the thread queue structure
typedef struct thread_queue
{
    thread_node_t *front;
    thread_node_t *rear;
} thread_queue_t;

// Initialize an empty thread queue
void init_thread_queue(thread_queue_t *queue);

// Check if the thread queue is empty
int is_thread_queue_empty(thread_queue_t *queue);

// Enqueue a thread in the thread queue
void enqueue_thread(thread_queue_t *queue, thread_t *data);

// Dequeue a thread from the thread queue and return a pointer to it
thread_t *dequeue_thread(thread_queue_t *queue);

// Clean up the thread queue (free memory)
void cleanup_thread_queue(thread_queue_t *queue);

// Removes a tcb from the queue
void remove_thread_from_queue(thread_queue_t *queue, thread_t *target_thread);

#endif /* THREAD_QUEUE_H */
