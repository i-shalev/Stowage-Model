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
#include "../common/AbstractAlgorithm.h"

class TasksProducer {
    const std::vector<std::string>* dirs;
    std::vector<std::string> dirsAfterCheck;
    const std::vector<std::string> algoNames;
    std::vector<ShipPlan*> shipPlans;
    std::vector<std::string> shipPlansPaths;
    std::vector<ShipRoute*> shipRoutes;
    std::vector<std::string> shipRoutesPaths;
    std::vector<int> travelDoubleFilesCode;
    std::vector<std::vector<int>> results;
    int numTravels;
    int numAlgo;
    const std::string outputPath;
    const std::string travelPath;


    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index();

//    std::optional<int> next_task_index_simple();

public:
    TasksProducer(std::vector<std::string> *dirs, std::vector<std::string> &algoNames, const std::string &outputPath,
                  const std::string &travelPath);
    ~TasksProducer();
    std::optional<std::function<void(void)>> getTask();
    void createShipDetails();
};


#endif //STOWAGE_MODEL_TASKSPRODUCER_H
