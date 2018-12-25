//
// Created by Gilad on 15-Dec-18.
//

#ifndef OS3_GAME_THREAD_H
#define OS3_GAME_THREAD_H

#include "Headers.hpp"
#include "Thread.hpp"
#include "Job.hpp"

class GameThread : public Thread{
public:
    ~GameThread() override;
    GameThread(uint id, PCQueue<Job*>* jobs, vector<float>* tile_hist,
               Lock* tile_hist_lock);

private:
    PCQueue<Job*>* jobs;
    vector<Job*> my_jobs;
    Lock* tile_hist_lock;
    vector<float>* tile_hist;
    void thread_workload() override;
};


#endif //OS3_GAME_THREAD_H
