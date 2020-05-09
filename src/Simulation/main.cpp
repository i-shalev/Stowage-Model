
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
        std::cout << "ERROR : no shipPlan file!" << std::endl;
    }
    if(shipRoutePath.empty()){
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
    if(shipPlan == nullptr){
        return;
    }

    errorCode = 0;
    auto* shipRoute = createShipRoute(errorCode, shipRoutePath);

    res = algo.readShipRoute(shipRoutePath);
    if(errorCode == res){
        std::cout << "equal:" << errorCode << std::endl;
    } else {
        std::cout << "not equal" << "sim: " << errorCode << " algo: " << res << std::endl;
    }
    if(shipRoute == nullptr){
        return;
    }

    Ship* ship = new Ship(shipRoute, shipPlan);
    auto* mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());

    delete(mapPortVisits);
    delete(shipPlan);
    delete(shipRoute);
    delete(ship);
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

ShipPlan* createShipPlan(int &errorCode, const std::string& shipPlanPath){
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

std::map<std::string, int>* createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList) {
    auto* mapPortVisits = new std::map<std::string, int>();
    for(const auto& port : *portList) {
        auto res = mapPortVisits->find(port);
        int ans;
        if(res == mapPortVisits->end()) {
            ans = 0;
        } else {
            ans = res->second;
            mapPortVisits->erase(port);
        }
        mapPortVisits->insert({port, ans+1});
    }
    return mapPortVisits;
}

void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits, std::map<std::string, std::string>* mapPortFullNameToCargoPath) {
    char* pathToDirChar = (char *)(malloc((pathToDir.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, pathToDir);
    std::vector<std::string> namesOfFilesEndsWithCargoData;
    getCargoData(pathToDirChar, namesOfFilesEndsWithCargoData);

    int indexNumber;
    std::string portName;
    std::string fullname;


    for (const auto& name: namesOfFilesEndsWithCargoData) {
        if (handleNameOfFile(name, portName, indexNumber)) {
            fullname =  pathToDir + '/';
            fullname += name;
            fullname += R"(.cargo_data)";
            mapPortFullNameToCargoPath->insert({name, fullname});
            auto res = mapPortVisits->find(portName);
            if(res !=  mapPortVisits->end() and res->second > indexNumber) {
                Port *port = new Port(portName, indexNumber, errors);

                if (readPortContainers(port, fullname, errors)) {
                    mapPortFullNameToCargoPath->insert({name, port});
                }
            } else {
//                errors->push_back("Warning: the file " + name + ".cargo_data is not necessary");
                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        } else {
//            errors->push_back("Warning: the file " + name + ".cargo_data is not in the right format");
            std::cout << "Warning: the file " << name << ".cargo_data is not in the right format" << std::endl;
        }
    }
    delete pathToDirChar;
}

bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber) {
    std::vector<std::string> elems;
    std::stringstream s(fileName);
    std::string word;
    while(getline(s, word, '_')) {
        elems.push_back(word);
    }
    if(elems.size() != 2) {
//        errors->push_back("Warning: the file name: " + fileName + " is not legal.");
        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    portName = elems[0];
    try {
        indexNumber = stoi(elems[1]);
    } catch (const std::exception& e) {
//        errors->push_back("Warning: the file name: " + fileName + " is not legal.");
        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    return true;
}