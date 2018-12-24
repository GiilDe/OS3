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

bool testGameBasic() {
//    char cCurrentPath[FILENAME_MAX];
//
//    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
//    {
//        return false;
//    }
//
//    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
//
//    printf ("The current working directory is %s", cCurrentPath);

    filebuf buf;
    buf.open("temp", ios::out);
//    auto oldbuf = cout.rdbuf(&buf);

    game_params attrs = {
            10,
            5,
            "/Users/miki/CLionProjects/os_hw3/Tests/io/small.txt",
            true,
            true
    };
    Game game(attrs);
    game.run();

//    cout.rdbuf(oldbuf);

    ifstream f1("temp");
    string str1((istreambuf_iterator<char>(f1)), istreambuf_iterator<char>());
//    ASSERT_EQUALS(str1, small_board_expected);

    return true;
}

int main() {
    RUN_TEST(testThreadWrapper);
    RUN_TEST(testGameBasic);
    return 0;
}