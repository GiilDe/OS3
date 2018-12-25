//
// Created by Miki Mints on 12/23/18.
//
#include "macros.h"
#include "../PCQueue.hpp"
#include "../Game.hpp"
#include "../Headers.hpp"
#include <algorithm>
#include <unistd.h>
#include "TestAux.hpp"
#define GetCurrentDir getcwd

PCQueue<int> q;

class WorkerThread1 : public Thread {
public:
    WorkerThread1() : Thread(1) {}

    ~WorkerThread1() override {}

protected:
    void thread_workload() override {
        for(int i = 0; i < 100; i++) {
            q.push(i);
        }
    }
};

class WorkerThread2 : public Thread {
private:
    vector<int> v;
public:
    WorkerThread2() : Thread(2) {}

    ~WorkerThread2() override {}
    vector<int> getVector() { return v; }

protected:
    void thread_workload() override {
        for(int i = 0; i < 100; i++) {
            this->v.push_back(q.pop());
        }
    }
};

bool testThreadWrapper() {
    WorkerThread1 t1;
    WorkerThread2 t2;

    ASSERT_TRUE(t1.start());
    ASSERT_TRUE(t2.start());

    ASSERT_EQUALS(t1.thread_id(), 1);
    ASSERT_EQUALS(t2.thread_id(), 2);

    t1.join();
    t2.join();

    vector<int> inserted = t2.getVector();
    int i = 0;
    for(int k : inserted) {
        ASSERT_EQUALS(i, k);
        i++;
    }
    return true;
}

static inline game_params newGame(uint gens, uint threads, string filename, bool animated, bool print) {
    game_params attrs = {
            gens,
            threads,
            filename,
            animated,
            print
    };
    return attrs;
}

string testGameBasicThreads(uint threads) {
    filebuf buf;
    REDIRECT_COUT(buf);

    Game game(newGame(10, threads, "/Users/miki/CLionProjects/os_hw3/Tests/io/small.txt", false, true));
    game.run();

    END_REDIRECT_COUT();
    string out = cout_redir_str();
    return out;
}

bool testGameBasic1() {
    ASSERT_EQUALS(testGameBasicThreads(1), small_board_expected);
    return true;
}

bool testGameBasic2() {
    ASSERT_EQUALS(testGameBasicThreads(2), small_board_expected);
    return true;
}

bool testGameBasic3() {
    ASSERT_EQUALS(testGameBasicThreads(7), small_board_expected);
    return true;
}

int main() {
    RUN_TEST(testThreadWrapper);
    RUN_TEST(testGameBasic1);
    sleep(5);
    RUN_TEST(testGameBasic2);
    sleep(5);
    RUN_TEST(testGameBasic3);
    return 0;
}