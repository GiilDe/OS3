//
// Created by Gilad on 15-Dec-18.
//

#include "Game_thread.hpp"

Game_thread::Game_thread(uint id, PCQueue<Job>* jobs) : Thread(id){
    this->jobs = jobs;
}

void Game_thread::thread_workload() {
    while(!jobs->empty()){
        Job j = jobs->pop();
        j.run();
    }
}