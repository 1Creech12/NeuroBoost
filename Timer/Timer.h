#pragma once
#include <thread>
#include <atomic>
using namespace std;

class Timer {
    atomic<int> seconds;
    atomic<bool> running;
    thread t;

public:
    Timer();
    ~Timer();
    void reset();
    int get() const;
    void stop();
};
