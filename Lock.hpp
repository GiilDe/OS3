//
// Created by Gilad on 16-Dec-18.
//

#ifndef OS3_LOCK_HPP
#define OS3_LOCK_HPP

#include "Headers.hpp"
#include "Semaphore.hpp"

/**
 * Implements a Lock, using a single-entry Semaphore
 */
class Lock {
private:
    Semaphore s;

public:
    Lock() : s(1) {}
    void lock() { s.down(); }
    void unlock() { s.up(); }
};

#endif //OS3_LOCK_HPP
