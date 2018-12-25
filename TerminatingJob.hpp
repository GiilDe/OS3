//
// Created by Miki Mints on 12/25/18.
//

#ifndef OS_HW3_TERMINATINGJOB_H
#define OS_HW3_TERMINATINGJOB_H

#include "Headers.hpp"
#include "Job.hpp"

class TerminatingJob : public Job {

public:
    TerminatingJob() : Job(0, 0, nullptr, nullptr) {};
    void run() override {}
};


#endif //OS_HW3_TERMINATINGJOB_H
