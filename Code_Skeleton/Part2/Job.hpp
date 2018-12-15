//
// Created by Gilad on 15-Dec-18.
//

#ifndef OS3_JOB_HPP
#define OS3_JOB_HPP

#include "../Part1/Headers.hpp"
#include "Game_field.hpp"

class Job {
public:
    Job(int lower, int upper, Game_field* game_field){
        lower_row = lower;
        upper_row = upper;
        field = game_field;
    }
    void run();

private:
    int lower_row;
    int upper_row;
    Game_field* field;
};


#endif //OS3_JOB_HPP
