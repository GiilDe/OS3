//
// Created by Gilad on 14-Dec-18.
//
#include "PCQueue.hpp"

template <typename T>
PCQueue<T>::PCQueue() : s(0){
    writers_waiting = 0;
    inside = 0;
    pthread_cond_init(&writers, NULL);
    pthread_cond_init(&readers, NULL);
    pthread_mutex_init(&m, NULL);
}

template <typename T>
T PCQueue<T>::pop() {
    s.down();
    pop_lock();
    T data = q.pop();
    unlock();
}

template <typename T>
void PCQueue<T>::push(const T &item) {
    push_lock();
    q.push(item);
    unlock();
    s.up();
}

template <typename T>
void PCQueue<T>::push_lock(){
    pthread_mutex_lock(&m);
    writers_waiting++;
    while(inside > 0){
        pthread_cond_wait(&writers, &m);
    }
    inside++;
    writers_waiting--;
    pthread_mutex_unlock(&m);
}

template <typename T>
void PCQueue<T>::pop_lock(){
    pthread_mutex_lock(&m);
    while(inside > 0 || writers_waiting > 0){
        pthread_cond_wait(&readers, &m);
    }
    inside++;
    pthread_mutex_unlock(&m);
}

template <typename T>
void PCQueue<T>::unlock(){
    pthread_mutex_lock(&m);
    inside--;
    if(writers_waiting > 0){
        pthread_cond_signal(&writers);
    }
    else{
        pthread_cond_signal(&readers);
    }
    pthread_mutex_unlock(&m);
}
