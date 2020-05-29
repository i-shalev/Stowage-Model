//
// Created by zivco on 29/05/2020.
//

#include "ThreadPoolExecuter.h"

template<> void ThreadPoolExecuter<TasksProducer>::worker_function()  {
    while(!stopped) {
        auto task = producer.getTask();
        if(!task) break;
        (*task)();
        ++num_tasks_finished;
        ++total_num_tasks_finished;
    }
    if(stopped) {
        std::cout << std::this_thread::get_id() << " - stopped gracefully after processing " << num_tasks_finished << " task(s)" << std::endl;
    }
    else {
        std::cout << std::this_thread::get_id() << " - finished after processing " << num_tasks_finished << " task(s)" << std::endl;
    }
}


template<> bool ThreadPoolExecuter<TasksProducer>::start() {
    bool running_status = false;
    // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
    if(!running.compare_exchange_strong(running_status, true)) {
        return false;
    }
    for(int i=0; i<numThreads; ++i) {
        workers.push_back(std::thread([this]{
            worker_function();
        }));
    }
    return true;
}