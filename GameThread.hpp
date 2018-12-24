//
// Created by Gilad on 15-Dec-18.
//

#ifndef OS3_GAME_THREAD_H
#define OS3_GAME_THREAD_H

#include "Headers.hpp"
#include "Thread.hpp"
#include "Job.hpp"

class GameThread : public Thread {
public:
    GameThread(uint id, PCQueue<Job>* jobs);
private:
    PCQueue<Job>* jobs;
    void thread_workload() override;
};


#endif //OS3_GAME_THREAD_H
