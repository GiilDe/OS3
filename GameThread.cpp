//
// Created by Gilad on 15-Dec-18.
//
#include "PCQueue.hpp"
#include "GameThread.hpp"
#include "TerminatingJob.hpp"

static inline bool terminating_job(Job* j) {
    return dynamic_cast<TerminatingJob*>((j)) != nullptr;
}

GameThread::GameThread(uint id, PCQueue<Job *> *jobs, Lock *tile_lock, vector<float>* tile_hist) :
        Thread(id),
        tile_lock(tile_lock),
        jobs(jobs),
        my_jobs(),
        tile_hist(tile_hist) {}

void GameThread::thread_workload() {
    auto tile_start = std::chrono::system_clock::now();

    while (true) {
        Job *j = jobs->pop();
        my_jobs.push_back(j);
        if (terminating_job(j)) break;
        j->run();
    }

    auto tile_end = std::chrono::system_clock::now();
    // TODO Append to shared tile history
    tile_lock->lock();
    tile_hist->push_back((float) std::chrono::duration_cast<std::chrono::microseconds>(tile_end - tile_start).count());
    tile_lock->unlock();

}

GameThread::~GameThread() {
    for (Job *j : my_jobs) {
        delete j;
    }
}
