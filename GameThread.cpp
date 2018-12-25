//
// Created by Gilad on 15-Dec-18.
//
#include "PCQueue.hpp"
#include "GameThread.hpp"

GameThread::GameThread(uint id, PCQueue<Job*>* jobs, vector<float>* tile_hist,
                       Lock* tile_hist_lock) : Thread(id), my_jobs() {
    this->jobs = jobs;
    this->tile_hist = tile_hist;
    this->tile_hist_lock = tile_hist_lock;
}

void GameThread::thread_workload() {
    auto tile_start = std::chrono::system_clock::now();

    while(true) {
        if(jobs->empty()) {
            break;
        }
        Job* j = jobs->pop();
        my_jobs.push_back(j);
        j->run();
    }

    auto tile_end = std::chrono::system_clock::now();
    tile_hist_lock->lock();
    tile_hist->push_back((float) std::chrono::duration_cast<std::chrono::microseconds>(tile_end - tile_start).count());
    tile_hist_lock->unlock();
}

GameThread::~GameThread() {
    for(Job *j : my_jobs) {
        delete j;
    }
}
