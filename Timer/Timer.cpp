#include "Timer.h"
#include <chrono>

Timer::Timer() : seconds(0), running(true) {
    t = thread([this]() {
        while (running) {
            this_thread::sleep_for(chrono::seconds(1));
            seconds++;
        }
    });
}

Timer::~Timer() {
    running = false;
    t.join();
}

void Timer::reset() { seconds = 0; }

int Timer::get() const { return seconds; }
