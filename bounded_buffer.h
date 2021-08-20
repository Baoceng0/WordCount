#ifndef _BOUNDED_BUFFER_H
#define _BOUNDED_BUFFER_H
#include <pthread.h>

struct buffered_queue{
    void **buf;
    int size;
    int start_index;
    int end_index;
    pthread_mutex_t lock;
    pthread_cond_t cond;
};
// define a global variable
extern struct buffered_queue *buffer;

void buffered_queue_init(struct buffered_queue *queue, int size);

void buffered_queue_push(struct buffered_queue *queue, void *item);

void* buffered_queue_pop(struct buffered_queue *queue);

void buffered_queue_destroy(struct buffered_queue *queue);

#endif
