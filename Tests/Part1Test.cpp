//
// Created by Miki Mints on 12/18/18.
//

#include "../Code_Skeleton/Part1/PCQueue.hpp"
#include "macros.h"
#include <algorithm>

Semaphore print_sem(1);
PCQueue<int> global_q;

void* thread1(void* arg) {
    auto *q = (PCQueue<int>*)arg;
    for(int i = 0; i < 1000; i++) {
        q->push(i);
    }
    auto *retval = new int;
    (*retval) = 0;
    pthread_exit(retval);
}

void* thread2(void* arg) {
    auto *q = (PCQueue<int>*)arg;
    auto *retval = new int;
    (*retval) = -1;
    for(int i = 0; i < 1000; i++) {
        int val = q->pop();
        if(val != i) {
            pthread_exit(retval);
        }
    }
    (*retval) = 0;
    pthread_exit(retval);
}

bool testBasicPCQueue() {

    PCQueue<int> queue;

    pthread_t t1, t2;
    pthread_create(&t1, nullptr, thread1, &queue);
    pthread_create(&t2, nullptr, thread2, &queue);

    void* stat1, *stat2;

    pthread_join(t1, &stat1);
    pthread_join(t2, &stat2);
    ASSERT_EQUALS(*(int*)stat1, 0);
    ASSERT_EQUALS(*(int*)stat2, 0);

    delete stat1;
    delete stat2;
    return true;
}

void* producer(void* arg) {
    auto *q = (PCQueue<int>*)arg;
    for(int i = 0; i < 1000; i++) {
        q->push(i);
    }
    auto *retval = new int;
    (*retval) = 0;
    pthread_exit(retval);
}

void* consumer(void* arg) {
    auto *q = (PCQueue<int>*)arg;
    auto *retval = new int;
    (*retval) = -1;
    while(global_q.empty())
    for(int i = 0; i < 1000; i++) {
        int val = q->pop();
        global_q.push(val);
    }
    (*retval) = 0;
    pthread_exit(retval);
}

bool testManyConsumersPCQueue() {
    PCQueue<int> queue;

    pthread_t t1, t2;
    pthread_create(&t1, nullptr, producer, &queue);
    for(int i = 0; i < 10; i++) {
        pthread_create(&t2, nullptr, consumer, &queue);
    }

    int i = 0;
    while(!global_q.empty()) {
        ASSERT_EQUALS(i, global_q.pop());
        i++;
    }
}

int main() {
    RUN_TEST(testBasicPCQueue);
    RUN_TEST(testManyConsumersPCQueue);
    return 0;
}