//====================================================================
// a simple thread pool
// @ Amir Kirsh
//====================================================================

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>
#include "TasksProducer.h"
#include "HelperCleses.h"

template<typename Producer>
class ThreadPoolExecuter {
    Producer producer;
    const int numThreads = -1;
    std::vector<std::thread> workers;
    std::atomic_bool running {false};
    std::atomic_bool stopped {false};
    static thread_local int num_tasks_finished;
    std::atomic_int total_num_tasks_finished { 0 };

    void worker_function();
public:
    ThreadPoolExecuter(Producer producer, NumThreads numThreads): producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }
    bool start();
    void stop_gracefully() {
        stopped = true;
        wait_till_finish();
    }
    void wait_till_finish() {
        for(auto& t : workers) { t.join();}
        std::cout << "thread pool finished/stopped after processing " << total_num_tasks_finished << " task(s)" << std::endl;
    }
};

// ThreadPoolExecuter.h - being a template - but outside of the class:
template<typename Producer>
thread_local int ThreadPoolExecuter<Producer>::num_tasks_finished { 0 };
