//
// Created by Gilad on 15-Dec-18.
//

#ifndef OS3_GAME_THREAD_H
#define OS3_GAME_THREAD_H

#include "../Part1/Headers.hpp"
#include "Thread.hpp"
#include "Job.hpp"

class Game_thread : public Thread{
public:
    Game_thread(uint id, PCQueue<Job>* jobs);
private:
    PCQueue<Job>* jobs;
    void thread_workload() override;
};


#endif //OS3_GAME_THREAD_H
