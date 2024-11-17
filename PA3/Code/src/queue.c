#ifndef QUEUE_H
#define QUEUE_H

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

// Define a structure for the queue node
// typedef struct thread_node
// {
//     thread_t *data;
//     struct thread_node *next;
// } thread_node_t;

// // Define the thread queue structure
// typedef struct thread_queue
// {
//     thread_node_t *front;
//     thread_node_t *rear;
// } thread_queue_t;

// Initialize an empty thread queue
void init_thread_queue(thread_queue_t *queue)
{
    queue->front = queue->rear = NULL;
}

// Check if the thread queue is empty
int is_thread_queue_empty(thread_queue_t *queue)
{
    return queue->front == NULL;
}

// Enqueue a thread in the thread queue
void enqueue_thread(thread_queue_t *queue, thread_t *data)
{
    thread_node_t *new_node = (thread_node_t *)malloc(sizeof(thread_node_t));
    new_node->data = data;
    new_node->next = NULL;

    if (is_thread_queue_empty(queue))
    {
        queue->front = queue->rear = new_node;
    }
    else
    {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

// Dequeue a thread from the thread queue and return a pointer to it
thread_t *dequeue_thread(thread_queue_t *queue)
{
    if (is_thread_queue_empty(queue))
    {
        // You should handle underflow (e.g., returning an error or exiting)
        // This is a simple example and does not include error handling.
        return NULL;
    }

    thread_node_t *temp = queue->front;
    thread_t *data = temp->data;
    queue->front = queue->front->next;

    // free(temp);
    return data;
}

// Clean up the thread queue (free memory)
void cleanup_thread_queue(thread_queue_t *queue)
{
    while (!is_thread_queue_empty(queue))
    {
        thread_t *dequeued_thread = dequeue_thread(queue);
        free(dequeued_thread);
    }
}

// removes a tcb from the queue
void remove_thread_from_queue(thread_queue_t *queue, thread_t *target_thread)
{
    thread_node_t *current = queue->front;
    thread_node_t *prev = NULL;

    while (current != NULL)
    {
        if (current->data == target_thread)
        {
            if (current == queue->front)
            {
                // If the target thread is at the front of the queue
                queue->front = current->next;
                if (queue->front == NULL)
                {
                    // If the queue becomes empty after removal
                    queue->rear = NULL;
                }
            }
            else if (current == queue->rear)
            {
                // If the target thread is at the rear of the queue
                queue->rear = prev;
                prev->next = NULL;
            }
            else
            {
                // If the target thread is in the middle of the queue
                prev->next = current->next;
            }

            // free(current);
            return; // Target thread found and removed
        }

        prev = current;
        current = current->next;
    }

    // Target thread not found in the queue
}

#endif // QUEUE_H