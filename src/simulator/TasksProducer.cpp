//
// Created by zivco on 29/05/2020.
//

#include <algorithm>
#include "TasksProducer.h"
#include "../common/Files.h"
#include "main.h"

TasksProducer::TasksProducer(std::vector<std::string> *dirs, std::vector<std::string> &algoNames, const std::string &outputPath,
        const std::string &travelPath) : dirs(dirs), algoNames(algoNames), outputPath(outputPath), travelPath(travelPath){
    this->numAlgo = algoNames.size();

    createShipDetails();

    this->numTravels = dirsAfterCheck.size();

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
    for(const auto& dir:dirsAfterCheck)
        firstLine.push_back(dir);
    firstLine.emplace_back("Sum");
    firstLine.emplace_back("Num Errors");
    emptyFile(outputPath + "/simulation.results");
    writeToSuccessFile(outputPath + "/simulation.results", &firstLine);

    std::vector<int> vec;
    std::vector<int> sums;
    std::vector<int> numErrs;

    for(int j = 0 ; j < numAlgo ; j++){
        vec.push_back(j);
        int tmp, numErrors = 0, sum = 0;
        for(int i = 0 ; i < numTravels ; i++){
            tmp = results.at(i).at(j);
            if(tmp == -1)
                numErrors++;
            else
                sum += tmp;
        }
        sums.push_back(sum);
        numErrs.push_back(numErrors);
    }

    std::sort(std::begin(vec ), std::end(vec ),
            [sums, numErrs](int a, int b) {
                        if(numErrs.at(a) != numErrs.at(b)){
                            if(numErrs.at(a) > numErrs.at(b)){
                                return a < b;
                            } else {
                                return a > b;
                            }
                        } else {
                            if(sums.at(a) < sums.at(b)){
                                return a > b;
                            }
                            return a < b;
                        }
    });

    for(int k = 0 ; k < numAlgo ; k++){
        int j = vec.at(k);
        std::vector<std::string> lineResults;
        lineResults.push_back(algoNames.at(j));
        for(int i = 0 ; i < numTravels ; i++){
            lineResults.push_back(std::to_string(results.at(i).at(j)));
        }
        lineResults.push_back(std::to_string(sums.at(j)));
        lineResults.push_back(std::to_string(numErrs.at(j)));
        writeToSuccessFile(outputPath + "/simulation.results", &lineResults);
    }

    for(auto & shipPlan : shipPlans){
        delete shipPlan;
    }

    for(auto & shipRoute : shipRoutes){
        delete shipRoute;
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

std::optional<std::function<void(void)>> TasksProducer::getTask() {
    auto task_index = next_task_index(); // or: next_task_index_simple();
    if(task_index) {
        int travelIndex = (*task_index) / (this->numAlgo);
        int algoIndex = (*task_index) % (this->numAlgo);

        return [this, travelIndex ,algoIndex]{
            auto& registrar = AlgorithmRegistrar::getInstance();
            auto algo_iter = registrar.at(algoIndex);
            auto algo = (*algo_iter)();
            auto errors = std::make_unique<std::vector<std::string>>();
            bool fatalError = false;

            if(this->shipPlans.at(travelIndex) == nullptr){
                if(this->shipPlansPaths.at(travelIndex) == ""){
                    fatalError = true;
                    errors->push_back("ERROR: no shipPlan file!");
                }
            }
            if(this->shipRoutes.at(travelIndex) == nullptr){
                if(this->shipRoutesPaths.at(travelIndex) == ""){
                    fatalError = true;
                    errors->push_back("ERROR: no shipRoute file!");
                }
            }
            if(this->travelDoubleFilesCode.at(travelIndex) != 0){
                if(this->travelDoubleFilesCode.at(travelIndex) == 1 or this->travelDoubleFilesCode.at(travelIndex) == 3)
                    errors->push_back("Warning: there are more than 1 ship_plan file!");
                if(this->travelDoubleFilesCode.at(travelIndex) > 1)
                    errors->push_back("Warning: there are more than 1 route file!");
            }

            if(fatalError){
                writeErrorsToFile(this->outputPath + "/errors/" + this->algoNames.at(algoIndex) + "_" + this->dirsAfterCheck.at(travelIndex) + ".errors", this->outputPath + "/errors/", errors.get());
                this->results.at(travelIndex).at(algoIndex) = -1;
                return;
            }

            std::string path = this->shipPlansPaths.at(travelIndex);
            int errorCode = algo->readShipPlan(path);
            if(errorCode > 0){
                std::string errorCodeStr;
                getStringOfErrors(errorCode, errorCodeStr);
                errorCodeStr = "While read the ShipPlan, the algorithm return the errors: " + errorCodeStr;
                errors->push_back(errorCodeStr);
                if(containsFatalError(errorCode)){
                    writeErrorsToFile(this->outputPath + "/errors/" + this->algoNames.at(algoIndex) + "_" + this->dirsAfterCheck.at(travelIndex) + ".errors", this->outputPath + "/errors/", errors.get());
                    this->results.at(travelIndex).at(algoIndex) = -1;
                    return;
                }
            }

            errorCode = algo->readShipRoute(this->shipRoutesPaths.at(travelIndex));
            if(errorCode > 0){
                std::string errorCodeStr;
                getStringOfErrors(errorCode, errorCodeStr);
                errorCodeStr = "While read the ShipRoute, the algorithm return the errors: " + errorCodeStr;
                errors->push_back(errorCodeStr);
                if(containsFatalError(errorCode)){
                    writeErrorsToFile(this->outputPath + "/errors/" + this->algoNames.at(algoIndex) + "_" + this->dirsAfterCheck.at(travelIndex) + ".errors", this->outputPath + "/errors/", errors.get());
                    this->results.at(travelIndex).at(algoIndex) = -1;
                    return;
                }
            }

            if(this->shipPlans.at(travelIndex) == nullptr){
                errors->push_back("ERROR: no shipPlan file!");
                return;
            }
            if(this->shipRoutes.at(travelIndex) == nullptr){
                errors->push_back("ERROR: no shipRoute file!");
            }
            auto* shipPlan = new ShipPlan(*this->shipPlans.at(travelIndex));
            auto* shipRoute = new ShipRoute(*this->shipRoutes.at(travelIndex));

            WeightBalanceCalculator wb;
            algo->setWeightBalanceCalculator(wb);

            Ship* ship = new Ship(shipRoute, shipPlan);
            auto mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());
            auto mapPortFullNameToCargoPath = createMapPortFullNameToCargoPath(this->travelPath + "/" + this->dirsAfterCheck.at(travelIndex), mapPortVisits.get(),
                                                                               shipRoute->getDstList()->at(shipRoute->getDstList()->size()-1), errors.get());

            int numOp = 0;
            while(!ship->finishRoute()){
                std::string pathToInstructions = this->outputPath + "/" + this->algoNames.at(algoIndex) + "_" + this->dirsAfterCheck.at(travelIndex) + "_crane_instructions" + "/" + ship->getCurrentDestinationWithIndex() + ".instructions";
                createFolder(this->outputPath + "/" + this->algoNames.at(algoIndex) + "_" + this->dirsAfterCheck.at(travelIndex) + "_crane_instructions");
                int instErrorCode = algo->getInstructionsForCargo(mapPortFullNameToCargoPath.get()->at(ship->getCurrentDestinationWithIndex()), pathToInstructions);
                if(instErrorCode > 0){
                    std::string errorCodeStr;
                    getStringOfErrors(instErrorCode, errorCodeStr);
                    errorCodeStr = "While get instructions for port "+ ship->getCurrentDestinationWithIndex()
                                   + ", the algorithm return the errors: " + errorCodeStr;
                    errors->push_back(errorCodeStr);
                }
                std::vector<std::string> errorReason;
                int numOpTmp = runAlgoOnPort(ship, mapPortFullNameToCargoPath.get()->at(ship->getCurrentDestinationWithIndex()), pathToInstructions, errorReason);
                if(numOpTmp < 0){
                    numOp = -1;
                    if(!errorReason.empty())
                        errors->push_back(errorReason.at(0));
                    break;
                } else {
                    numOp += numOpTmp;
                }
                ship->moveToNextPort();
            }
            if(numOp > 0 and !ship->isEmpty()){
                numOp = -1;
                errors->push_back("The ship is not empty after finish all the instructions from the algorithm.");
            }

            writeErrorsToFile(outputPath + "/errors/" + this->algoNames.at(algoIndex) + "_" + this->dirsAfterCheck.at(travelIndex) + ".errors", outputPath + "/errors/", errors.get());
            delete(ship);
            this->results.at(travelIndex).at(algoIndex) = numOp;
            return;
        };
    }
    else return {};
}

void TasksProducer::createShipDetails() {
    for(auto dir:*dirs) {
        auto errors = std::make_unique<std::vector<std::string>>();
        std::string pathToDir = travelPath + "/" + dir;
        std::string shipPlanPath, shipRoutePath;
        int result = getShipPlanAndRoutePaths(pathToDir, shipPlanPath, shipRoutePath);
        ShipPlan* sp = nullptr;
        ShipRoute* sr = nullptr;
        bool spCreated = false;
        bool srCreated = false;

        // push the shipPlan for travel
        if(shipPlanPath.empty()){
            errors->push_back("There is no shipPlan file.");
        } else {
            int res = 0;
            sp = createShipPlan(res, shipPlanPath);
            if(containsFatalError(res)) {
                std::string errorCodeStr;
                getStringOfErrors(res, errorCodeStr);
                errorCodeStr = "The was a fatal error in the shipPlan file. The error code is: " + errorCodeStr;
                errors->push_back(errorCodeStr);
            } else {
                spCreated = true;
            }
        }

        // push the shipRoute for travel
        if(shipRoutePath.empty()){
            errors->push_back("There is no shipRoute file.");
        } else {
            int res = 0;
            sr = createShipRoute(res, shipRoutePath);
            if(containsFatalError(res)) {
                std::string errorCodeStr;
                getStringOfErrors(res, errorCodeStr);
                errorCodeStr = "The was a fatal error in the shipRoute file. The error code is: " + errorCodeStr;
                errors->push_back(errorCodeStr);
            } else {
                srCreated = true;
            }
        }

        if(srCreated and spCreated) {
            // push error code for travel - indicates more than 1 file
            travelDoubleFilesCode.push_back(result);
            shipPlans.push_back(sp);
            shipPlansPaths.push_back(shipPlanPath);
            shipRoutes.push_back(sr);
            shipRoutesPaths.push_back(shipRoutePath);
            dirsAfterCheck.push_back(dir);
        } else {
            if(spCreated){
                delete sp;
            }
            if(srCreated) {
                delete sr;
            }
            writeErrorsToFile(outputPath + "/errors/" + dir + ".errors", outputPath + "/errors/", errors.get());
        }
    }
}

