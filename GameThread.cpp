//
// Created by Gilad on 15-Dec-18.
//
#include "PCQueue.hpp"
#include "GameThread.hpp"

GameThread::GameThread(uint id, PCQueue<Job>* jobs) : Thread(id){
    this->jobs = jobs;
}

void GameThread::thread_workload() {
    while(!jobs->empty()){
        Job j = jobs->pop();
        j.run();
    }
}