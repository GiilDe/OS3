//
// Created by Gilad on 15-Dec-18.
//
#include "PCQueue.hpp"
#include "GameThread.hpp"

GameThread::GameThread(uint id, PCQueue<Job*>* jobs) : Thread(id), my_jobs() {
    this->jobs = jobs;
}

void GameThread::thread_workload() {
    Job* j = jobs->pop();
    my_jobs.push_back(j);
    j->run();
}

GameThread::~GameThread() {
    for(Job* j : my_jobs) {
        delete j;
    }
}
