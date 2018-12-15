//
// Created by Gilad on 14-Dec-18.
//
#include "PCQueue.hpp"

template <typename T>
PCQueue<T>::PCQueue() : s(0), writers_lock(1){
    writers_waiting = 0;
    pthread_cond_init(&is_writer_waiting, NULL);
    pthread_mutex_init(&m, NULL);
}

template <typename T>
T PCQueue<T>::pop() {
    s.down();
    pthread_mutex_lock(&m);

    while(writers_waiting > 0){
        pthread_cond_wait(&is_writer_waiting, &m);
    }

    T data = q.pop();
    pthread_mutex_unlock(&m);
}

template <typename T>
void PCQueue<T>::push(const T &item) {
    writers_lock.down();
    writers_waiting++;
    writers_lock.up();

    pthread_mutex_lock(&m);
    q.push(item);
    pthread_mutex_unlock(&m);

    writers_lock.down();
    writers_waiting--;
    if(writers_waiting == 0){
        pthread_cond_signal(&is_writer_waiting);
    }
    writers_lock.up();

    s.up();
}
