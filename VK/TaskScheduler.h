//
//  TaskScheduler.h
//  VK
//
//  Created by Билялов Эльдар on 03.11.2024.
//

#ifndef TaskScheduler_h
#define TaskScheduler_h

#include <queue>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

class TaskScheduler {
public:
    void Add(function<void()> job, time_t time) {
        lock_guard<mutex> lock(mtx);
        tasks.push({time, job});
        cv.notify_one();
    }

    TaskScheduler() {
        worker = thread([this]() { this->Run(); });
    }

    ~TaskScheduler() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_one();
        worker.join();
    }

private:
    struct Task {
        time_t time;
        function<void()> job;
        bool operator<(const Task& other) const {
            return time > other.time;
        }
    };

    priority_queue<Task> tasks;
    thread worker;
    mutex mtx;
    condition_variable cv;
    bool stop = false;

    void Run() {
        while (true) {
            unique_lock<mutex> lock(mtx);
            if (stop && tasks.empty()) break;

            if (!tasks.empty()) {
                auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
                if (tasks.top().time <= now) {
                    auto task = tasks.top().job;
                    tasks.pop();
                    lock.unlock();
                    task();
                } else {
                    cv.wait_until(lock, chrono::system_clock::from_time_t(tasks.top().time));
                }
            } else {
                cv.wait(lock);
            }
        }
    }
};
#endif /* TaskScheduler_h */
