//
// Created by zivco on 29/05/2020.
//

#ifndef STOWAGE_MODEL_SIMPLETASKSPRODUCER_H
#define STOWAGE_MODEL_SIMPLETASKSPRODUCER_H

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>
#include "HelperCleses.h"

class SimpleTasksProducer {
    const int numTasks = -1;
    const int iterationsPerTask = -1;
    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index();

    std::optional<int> next_task_index_simple();

public:
    SimpleTasksProducer(NumTasks numTasks, IterationsPerTask iterations)
            : numTasks(numTasks), iterationsPerTask(iterations) {}
    SimpleTasksProducer(SimpleTasksProducer&& other)
            : numTasks(other.numTasks), iterationsPerTask(other.iterationsPerTask), task_counter(other.task_counter.load()) {}
    std::optional<std::function<void(void)>> getTask();
};


#endif //STOWAGE_MODEL_SIMPLETASKSPRODUCER_H
