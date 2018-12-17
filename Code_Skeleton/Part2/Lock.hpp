//
// Created by Gilad on 16-Dec-18.
//

#ifndef OS3_LOCK_HPP
#define OS3_LOCK_HPP

#include "../Part1/Headers.hpp"

class Lock {
public:
    Lock() : s(1){}
    void lock(){
        s.down();
    }
    void unlock(){
        s.up();
    }

private:
    Semaphore s;
};


#endif //OS3_LOCK_HPP
