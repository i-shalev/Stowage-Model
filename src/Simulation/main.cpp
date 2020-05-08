
#include "main.h"
#include "../Algo/NaiveAlgo.h"

int main(int argc, char **argv){
    std::map<std::string, std::string> args;

    if(createArgs(args, argc, argv)){
        // fatal error
        return EXIT_FAILURE;
    }
    NaiveAlgo algo;
    runAlgoForAllTravels(algo, args["-travel_path"], args["-output"]);

    return EXIT_SUCCESS;
}

int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv){
    for(int i = 1; i < argc-1; i += 2){
        args[argv[i]] = argv[i+1];
    }

    if(args["-travel_path"].empty()){
        // fatal error
        return EXIT_FAILURE;
    }
    if(args["-algorithm_path"].empty()){
        args["-algorithm_path"] = "./";
    }
    if(args["-output"].empty()){
        args["-output"] = "./";
    }

    return EXIT_SUCCESS;
}

void printArgs(std::map<std::string, std::string>& args){
    std::cout << "travel_path: " << args["-travel_path"] << std::endl;
    std::cout << "algorithm_path: " << args["-algorithm_path"] << std::endl;
    std::cout << "output: " << args["-output"] << std::endl;
}

void runAllAlgo(const std::string& algoPath){
    auto* algoNames = getFileNamesEndWith(algoPath, ".so");
    for(const auto& algoName:*algoNames) {
        std::cout << algoName << std::endl;
    }

    delete(algoNames);
}

void runAlgoForAllTravels(AbstractAlgorithm& algo, const std::string &travelPath, const std::string &outputPath) {
    auto* dirs = getDirsNamesFromRootDir(travelPath);
    for(const auto& dir:*dirs) {
        runAlgoForTravel(algo, travelPath + "/" + dir, outputPath);
    }
    delete(dirs);
}

void runAlgoForTravel(AbstractAlgorithm& algo, const std::string &pathToDir, const std::string &outputPath) {
    std::cout << "dir: " << pathToDir << std::endl;
    std::string shipPlanPath, shipRoutePath;
    getShipPlanAndRoutePaths(pathToDir, shipPlanPath, shipRoutePath);
    if(shipPlanPath.empty()){
        // ERROR : no shipPlan file!
        std::cout << "ERROR : no shipPlan file!" << std::endl;
    }
    if(shipRoutePath.empty()){
        // ERROR : no shipPlan file!
        std::cout << "ERROR : no shipPlan file!" << std::endl;
    }

    int errorCode = 0;
    auto* shipPlan = createShipPlan(errorCode, shipPlanPath);
    int res = algo.readShipPlan(shipPlanPath);
    if(errorCode == res){
        std::cout << "equal:" << errorCode << std::endl;
    } else {
        std::cout << "not equal" << "sim: " << errorCode << " algo: " << res << std::endl;
    }
//    std::cout << errorCode << std::endl;
    if(shipPlan == nullptr){
        return;
    }
//    shipPlan->printShipPlan();
    errorCode = 0;
    auto* shipRoute = createShipRoute(errorCode, shipRoutePath);

    res = algo.readShipRoute(shipRoutePath);
    if(errorCode == res){
        std::cout << "equal:" << errorCode << std::endl;
    } else {
        std::cout << "not equal" << "sim: " << errorCode << " algo: " << res << std::endl;
    }
//    std::cout << errorCode << std::endl;
    if(shipRoute == nullptr){
        return;
    }

    delete(shipPlan);
    delete(shipRoute);
}

void getShipPlanAndRoutePaths(const std::string& pathToDir, std::string& shipPlanPath, std::string& shipRoutePath){
    auto* res = getFileNamesEndWith(pathToDir, ".ship_plan");
    if(!res->empty())
        shipPlanPath = pathToDir + "/" + res->at(0) + ".ship_plan";
    delete(res);

    res = getFileNamesEndWith(pathToDir, ".route");
    if(!res->empty())
        shipRoutePath = pathToDir + "/" + res->at(0) + ".route";
    delete(res);
}

ShipPlan * createShipPlan(int &errorCode, const std::string& shipPlanPath){
    bool fatalError = false;
    int numFloors=0 , length=0, width=0, numLines;
    if(!getSizesShipPlan(shipPlanPath, numFloors, length, width, numLines)) {
        errorCode = turnToTrueBit(errorCode,3);
        return nullptr;
    }

    // create the ShipPlanVector
    auto* blocks = new std::vector<std::vector<int>>(numLines-1);
    auto* results = readShipPlanInFiles(*blocks, shipPlanPath, numFloors, length, width);
    if(results->at(1)){
        errorCode = turnToTrueBit(errorCode,3);
        fatalError = true;
    }
    if(results->at(4)){
        errorCode = turnToTrueBit(errorCode,4);
        fatalError = true;
    }
    if(results->at(0)){
        errorCode = turnToTrueBit(errorCode,2);
    }
    if(results->at(2)){
        errorCode = turnToTrueBit(errorCode,0);
    }
    if(results->at(3)){
        errorCode = turnToTrueBit(errorCode,1);
    }

    if(fatalError)
        return nullptr;

    auto* shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    delete blocks;
    delete results;

    return shipPlan;
}

ShipRoute* createShipRoute(int &errorCode, const std::string& shipRoutePath){
    bool fatalError = false;
    auto* ports = new std::vector<std::string>();
    auto* results = readShipPorts(*ports, shipRoutePath);
    if(results->at(2)){
        errorCode = turnToTrueBit(errorCode,7);
        fatalError = true;
    }
    if(results->at(3)){
        errorCode = turnToTrueBit(errorCode,8);
        fatalError = true;
    }
    if(results->at(0)){
        errorCode = turnToTrueBit(errorCode,5);
    }
    if(results->at(1)){
        errorCode = turnToTrueBit(errorCode,6);
    }
    if(fatalError)
        return nullptr;
    auto* shipRoute = new ShipRoute(ports);
    delete ports;
    delete results;
    return shipRoute;
}