//
// Created by Gilad on 15-Dec-18.
//

#include "Thread.hpp"
#include "../Part1/Headers.hpp"

#ifndef OS3_GAME_THREAD_H
#define OS3_GAME_THREAD_H


class Game_thread : public Thread{
public:
    Game_thread(uint id, PCQueue<Job>* jobs);
private:
    PCQueue<Job>* jobs;
    void thread_workload() override;
};


#endif //OS3_GAME_THREAD_H
