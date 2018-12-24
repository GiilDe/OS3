//
// Created by Miki Mints on 12/18/18.
//

#include "../PCQueue.hpp"
#include "macros.h"
#include <algorithm>

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
        // cout << "Producer: " << i << endl;
        q->push(i);
    }
    auto *retval = new int;
    (*retval) = 0;
    pthread_exit(retval);
}

bool testManyConsumersPCQueue() {


    return true;
}

int main() {
    RUN_TEST(testBasicPCQueue);
    RUN_TEST(testManyConsumersPCQueue);
    cout << "Part 1 Tests Passed" << endl;
    return 0;
}