//
// Created by zivco on 29/05/2020.
//

#ifndef STOWAGE_MODEL_TASKSPRODUCER_H
#define STOWAGE_MODEL_TASKSPRODUCER_H

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>
#include "HelperCleses.h"
#include "../common/ShipPlan.h"
#include "../common/ShipRoute.h"

class TasksProducer {
    const std::vector<std::string>* dirs;
    const std::vector<std::string> algoNames;
    const std::vector<ShipPlan> shipPlans;
    const std::vector<ShipRoute> shipRoutes;
    std::vector<std::vector<int>> results;
    int numTravels;
    int numAlgo;
    const std::string outputPath;



    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index();

    std::optional<int> next_task_index_simple();

public:
    TasksProducer(std::vector<std::string>* dirs, std::vector<std::string>& algoNames, const std::string &outputPath);
    ~TasksProducer();
    std::optional<std::function<void(void)>> getTask();
    void createShipDetails();
};


#endif //STOWAGE_MODEL_TASKSPRODUCER_H
