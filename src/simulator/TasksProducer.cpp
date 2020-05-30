//
// Created by zivco on 29/05/2020.
//

#include "TasksProducer.h"
#include "../common/Files.h"
#include "main.h"

TasksProducer::TasksProducer(std::vector<std::string> *dirs, std::vector<std::string> &algoNames, const std::string &outputPath,
        const std::string &travelPath) : dirs(dirs), algoNames(algoNames), outputPath(outputPath), travelPath(travelPath){
    this->numTravels = dirs->size();
    this->numAlgo = algoNames.size();
    for(int i = 0 ; i < numTravels ; i++){
        results.emplace_back();
        for(int j = 0 ; j < numAlgo ; j++){
            results.at(i).push_back(0);
        }
    }

    createShipDetails();
}

TasksProducer::~TasksProducer() {
    std::vector<std::string> firstLine;
    firstLine.emplace_back("RESULTS");
    for(const auto& dir:*dirs)
        firstLine.push_back(dir);
    firstLine.emplace_back("Sum");
    firstLine.emplace_back("Num Errors");
    writeToSuccessFile(outputPath + "/simulation.results", &firstLine);

    for(int j = 0 ; j < numAlgo ; j++){
        std::vector<std::string> lineResults;
        int sum = 0, numErrors = 0, tmp;
        lineResults.push_back(algoNames.at(j));
        for(int i = 0 ; i < numTravels ; i++){
            tmp = results.at(i).at(j);
            if(tmp == -1)
                numErrors++;
            else
                sum += tmp;
            lineResults.push_back(std::to_string(tmp));
        }
        lineResults.push_back(std::to_string(sum));
        lineResults.push_back(std::to_string(numErrors));
        writeToSuccessFile(outputPath + "/simulation.results", &lineResults);
    }
}

std::optional<int> TasksProducer::next_task_index(){
    for(int curr_counter = task_counter.load(); curr_counter < (this->numAlgo * this->numTravels); ) {
        // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
        // note that in case compare_exchange_weak fails because the value of
        // task_counter != curr_counter than task_counter.load() is copied into curr_counter
        // in case of spurious failure (value not checked) curr_counter would not change
        if(task_counter.compare_exchange_weak(curr_counter, curr_counter + 1)) {
            return {curr_counter}; // zero based
        }
    }
    return {};
}

//std::optional<int> TasksProducer::next_task_index_simple(){
//    // this is a more simple approach for getting the next task index
//    // it will return each time an unused index in the range[0, numTasks)
//    // the difference from the one above is that at certain point of time
//    // the variable task_counter may exceed numTasks by any number between 1 and numThreads-1
//    // however, we will not return such values
//    if(task_counter < numTasks) {
//        int next_counter = ++task_counter; // atomic operation
//        // another thread could have increment task_counter after we passed the prev if
//        // so we must check again - following check is on a local variable so no race
//        if(next_counter <= numTasks) {
//            return {next_counter - 1}; // zero based
//        }
//        else {
//            // just so that at the end we will have: task_counter == numTasks
//            // this is not critical in our scenario but might be in other scenarios
//            --task_counter;
//        }
//    }
//    return {};
//}

std::optional<std::function<void(void)>> TasksProducer::getTask() {
    auto task_index = next_task_index(); // or: next_task_index_simple();
    if(task_index) {
        int travelIndex = (*task_index) / (this->numAlgo);
        int algoIndex = (*task_index) % (this->numAlgo);
        return [this, travelIndex ,algoIndex]{
            std::lock_guard g{m};
            std::cout << "travel: " << this->dirs->at(travelIndex) << ", algo index: " << algoIndex << ", code: " << this->travelDoubleFilesCode.at(travelIndex) << std::endl;
//            for(int i=0; i<iterationsPerTask; ++i) {
//
//                std::cout << std::this_thread::get_id() << "-" << *task_index << ": " << i << std::endl;
            std::this_thread::yield();
//            }
        };
    }
    else return {};
}

void TasksProducer::createShipDetails() {
    for(const auto& dir:*dirs) {
        std::string pathToDir = travelPath + "/" + dir;
        std::string shipPlanPath, shipRoutePath;
        int result = getShipPlanAndRoutePaths(pathToDir, shipPlanPath, shipRoutePath);

        // push the shipPlan for travel - null if file don't exist
        if(shipPlanPath.empty()){
            shipPlans.push_back(nullptr);
        } else {
            int res = 0;
            shipPlans.push_back(createShipPlan(res, shipPlanPath));
        }

        // push the shipRoute for travel - null if file don't exist
        if(shipRoutePath.empty()){
            shipRoutes.push_back(nullptr);
        } else {
            int res = 0;
            shipRoutes.push_back(createShipRoute(res, shipRoutePath));
        }

        // push error code for travel - indicates more than 1 file
        travelDoubleFilesCode.push_back(result);

//        if(result != 0){
//            if(result == 1 or result == 3)
//                errors->push_back("Warning: there are more than 1 ship_plan file!");
//            if(result > 1)
//                errors->push_back("Warning: there are more than 1 route file!");
//        }
    }
}

