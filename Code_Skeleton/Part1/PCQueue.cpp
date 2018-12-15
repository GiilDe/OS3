//
// Created by Gilad on 14-Dec-18.
//
#include "PCQueue.hpp"

template <typename T>
PCQueue<T>::PCQueue() : s(){
    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);
}

template <typename T>
T PCQueue<T>::pop() {
    s.down();
    pthread_mutex_lock(&m1);
    pthread_mutex_lock(&m2);
    T data = q.pop();
    pthread_mutex_unlock(&m2);
    pthread_mutex_unlock(&m1);
}

template <typename T>
void PCQueue<T>::push(const T &item) {
    pthread_mutex_lock(&m2);
    q.push(item);
    pthread_mutex_unlock(&m2);
    s.up();
}
