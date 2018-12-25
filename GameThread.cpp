//
// Created by Gilad on 15-Dec-18.
//
#include "PCQueue.hpp"
#include "GameThread.hpp"

GameThread::GameThread(uint id, PCQueue<Job*>* jobs) : Thread(id), my_jobs() {
    this->jobs = jobs;
//    this->tile_hist = tile_hist;
}

void GameThread::thread_workload() {
    auto tile_start = std::chrono::system_clock::now();

//    Job* j = jobs->pop();
//    my_jobs.push_back(j);
//    j->run();
    while(true) {
        if(jobs->empty()) {
            break;
        }
        Job* j = jobs->pop();
        my_jobs.push_back(j);
        j->run();
    }

    auto tile_end = std::chrono::system_clock::now();
    // TODO Append to shared tile history
//    tile_hist.push_back((float) std::chrono::duration_cast<std::chrono::microseconds>(tile_end - tile_start).count());

}

GameThread::~GameThread() {
    for(Job *j : my_jobs) {
        delete j;
    }
}
