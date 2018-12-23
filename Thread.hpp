
#ifndef __THREAD_H
#define __THREAD_H

#include "Headers.hpp"

class Thread {
protected:
    /** Implement this method in your subclass with the code you want your thread to run. */
    virtual void thread_workload() = 0;

    uint tid; // A number from 0 -> Number of threads initialized, providing a simple numbering for you to use

private:
    static void *entry_func(void *thread) {
        ((Thread *) thread)->thread_workload();
        return NULL;
    }

    pthread_t m_thread;

public:
    Thread(uint thread_id) {
        // Only places thread_id
        this->tid = thread_id;
    }

    virtual ~Thread() {} // Does nothing

    /** Returns true if the thread was successfully started, false if there was an error starting the thread */
    bool start() {
        int res = pthread_create(&m_thread, nullptr, entry_func, this);
        return !res;
    }

    /** Will not return until the internal thread has exited. */
    void join() {
        pthread_join(m_thread, nullptr);
    }

    /** Returns the thread_id **/
    uint thread_id() {
        return this->tid;
    }
};
#endif
