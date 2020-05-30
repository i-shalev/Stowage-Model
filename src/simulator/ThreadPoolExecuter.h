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
    int numThreads;
    std::vector<std::thread> workers;
    std::atomic_bool running {false};
    std::atomic_bool stopped {false};
    static thread_local int num_tasks_finished;
    std::atomic_int total_num_tasks_finished { 0 };

    void worker_function();
public:
    ThreadPoolExecuter(NumThreads numThreads, std::vector<std::string> *dirs, std::vector<std::string> &algoNames, const std::string &outputPath,
            const std::string &travelPath) : producer(TasksProducer(dirs, algoNames, outputPath, travelPath)), numThreads(numThreads) {
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
