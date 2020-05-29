//
// Created by zivco on 29/05/2020.
//

#include "TasksProducer.h"
#include "../common/Files.h"

TasksProducer::TasksProducer(std::vector<std::string>* dirs, std::vector<std::string>& algoNames,
        const std::string &outputPath) : dirs(dirs), algoNames(algoNames), outputPath(outputPath){
    this->numTravels = dirs->size();
    this->numAlgo = algoNames.size();
    for(int i = 0 ; i < numTravels ; i++){
        results.emplace_back();
        for(int j = 0 ; j < numAlgo ; j++){
            results.at(i).push_back(0);
        }
    }
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

//std::optional<int> TasksProducer::next_task_index(){
//    for(int curr_counter = task_counter.load(); curr_counter < 5; ) {
//        // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
//        // note that in case compare_exchange_weak fails because the value of
//        // task_counter != curr_counter than task_counter.load() is copied into curr_counter
//        // in case of spurious failure (value not checked) curr_counter would not change
//        if(task_counter.compare_exchange_weak(curr_counter, curr_counter + 1)) {
//            return {curr_counter}; // zero based
//        }
//    }
//    return {};
//}
//
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
//    auto task_index = next_task_index(); // or: next_task_index_simple();
//    if(task_index) {
//        return [task_index, this]{
//            for(int i=0; i<iterationsPerTask; ++i) {
//                std::lock_guard g{m};
//                std::cout << std::this_thread::get_id() << "-" << *task_index << ": " << i << std::endl;
//                std::this_thread::yield();
//            }
//        };
//    }
//    else return {};
    return {};
}


