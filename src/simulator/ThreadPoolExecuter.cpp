//
// Created by zivco on 29/05/2020.
//

#include "ThreadPoolExecuter.h"

template<> void ThreadPoolExecuter<SimpleTasksProducer>::worker_function()  {
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