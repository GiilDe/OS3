//
// Created by Gilad on 15-Dec-18.
//

#ifndef OS3_JOB_HPP
#define OS3_JOB_HPP

#include "../Part1/Headers.hpp"
#include "Game_field.hpp"

class Job {
public:
    Job(uint lower, uint upper, Game_field* current, Game_field* next, Lock* lower_s,
        Lock* upper_s){
        lower_row = lower;
        upper_row = upper;
        this->current = current;
        this->next = next;
        this->lower_lock = lower_s;
        this->upper_lock = upper_s;
    }
    void run();

private:
    uint lower_row;
    uint upper_row;
    Game_field* current;
    Game_field* next;
    Lock* lower_lock;
    Lock* upper_lock;
};


#endif //OS3_JOB_HPP
