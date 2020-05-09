
#include "main.h"
#include "../Algo/NaiveAlgo.h"

#define PATH_TO_EMPTY_FILE R"(.\empty.empty_file)"

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
    //std::cout << "travel_path: " << args["-travel_path"] << std::endl;
    //std::cout << "algorithm_path: " << args["-algorithm_path"] << std::endl;
    //std::cout << "output: " << args["-output"] << std::endl;
}

void runAllAlgo(const std::string& algoPath){
    emptyFile(PATH_TO_EMPTY_FILE);
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
    auto* errors = new std::vector<std::string>();
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
       // std::cout << "equal:" << errorCode << std::endl;
    } else {
        //std::cout << "not equal" << "sim: " << errorCode << " algo: " << res << std::endl;
    }
    if(shipPlan == nullptr){
        // TODO change that to fatal error bool
        return;
    }

    errorCode = 0;
    auto* shipRoute = createShipRoute(errorCode, shipRoutePath);

    res = algo.readShipRoute(shipRoutePath);
    if(errorCode == res){
        //std::cout << "equal:" << errorCode << std::endl;
    } else {
        //std::cout << "not equal" << "sim: " << errorCode << " algo: " << res << std::endl;
    }
    if(shipRoute == nullptr){
        // TODO change that to fatal error bool
        return;
    }

    Ship* ship = new Ship(shipRoute, shipPlan);
    auto* mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());
    auto* mapPortFullNameToCargoPath = createMapPortFullNameToCargoPath(pathToDir, mapPortVisits,
            shipRoute->getDstList()->at(shipRoute->getDstList()->size()-1), errors);

    for(const auto & i : *mapPortFullNameToCargoPath){
        //std::cout << i.first << " : " << i.second << std::endl;
    }
    for(const auto & i : *errors){
        //std::cout << i << std::endl;
    }

    while(!ship->finishRoute()){
        //std::cout << ship->getCurrentDestinationWithIndex() << std::endl;
        std::string pathToInstructions = pathToDir + "/" + ship->getCurrentDestinationWithIndex() + ".instructions";
        algo.getInstructionsForCargo(mapPortFullNameToCargoPath->at(ship->getCurrentDestinationWithIndex()), pathToInstructions);
        runAlgoOnPort(ship, mapPortFullNameToCargoPath->at(ship->getCurrentDestinationWithIndex()), pathToInstructions);
        ship->moveToNextPort();
    }

    delete(mapPortVisits);
    delete(ship);
    delete(errors);
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

std::map<std::string, std::string>* createMapPortFullNameToCargoPath(const std::string &pathToDir,
        std::map<std::string, int> *mapPortVisits, const std::string &lastPort, std::vector<std::string>* errors){
    auto* mapPortFullNameToCargoPath = new std::map<std::string, std::string>();
    addPortsWithFileToMap(pathToDir, mapPortVisits, mapPortFullNameToCargoPath, errors);
    addPortsWithNoFileToMap(pathToDir, mapPortVisits, mapPortFullNameToCargoPath, lastPort, errors);
    return mapPortFullNameToCargoPath;
}

void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits,
                     std::map<std::string, std::string> *mapPortFullNameToCargoPath, std::vector<std::string>* errors) {
    char* pathToDirChar = (char *)(malloc((pathToDir.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, pathToDir);
    std::vector<std::string> namesOfFilesEndsWithCargoData;
    getCargoData(pathToDirChar, namesOfFilesEndsWithCargoData);

    int indexNumber;
    std::string portName;
    std::string fullname;


    for (const auto& name: namesOfFilesEndsWithCargoData) {
        if (handleNameOfFile(name, portName, indexNumber)) {
            auto res = mapPortVisits->find(portName);
            if(res !=  mapPortVisits->end() and res->second > indexNumber) {
                fullname =  pathToDir + '/';
                fullname += name;
                fullname += R"(.cargo_data)";
                mapPortFullNameToCargoPath->insert({name, fullname});
            } else {
                errors->push_back("Warning: the file " + name + ".cargo_data is not necessary");
                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        } else {
            errors->push_back("Warning: the file " + name + ".cargo_data is not in the right format");
            std::cout << "Warning: the file " << name << ".cargo_data is not in the right format" << std::endl;
        }
    }
    delete pathToDirChar;
}

void addPortsWithNoFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits,
        std::map<std::string, std::string> *mapPortFullNameToCargoPath,
        const std::string &lastPort, std::vector<std::string>* errors) {
    for(const auto& elem : *mapPortVisits)
    {
        for(int i = 0; i < elem.second; i++)
        {
            std::string port_index = elem.first + "_" + std::to_string(i);
            auto res = mapPortFullNameToCargoPath->find(port_index);
            if(!mapPortFullNameToCargoPath->empty() && res!=mapPortFullNameToCargoPath->end()){
                continue;
            }

            if(!(lastPort == elem.first and i == elem.second - 1)){
                errors->push_back("Warning: the file " + elem.first + "_" + std::to_string(i) + ".cargo_data is missing.");
                std::cout << "Warning: the file " << elem.first + "_" + std::to_string(i) << ".cargo_data is missing." << std::endl;
            } else {
                // the last port - dont need to do anything for now
            }

            mapPortFullNameToCargoPath->insert({port_index, PATH_TO_EMPTY_FILE});
        }
    }
}

bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber) {
    std::vector<std::string> elems;
    std::stringstream s(fileName);
    std::string word;
    while(getline(s, word, '_')) {
        elems.push_back(word);
    }
    if(elems.size() != 2) {
        return false;
    }
    portName = elems[0];
    try {
        indexNumber = stoi(elems[1]);
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}


int runAlgoOnPort(Ship *ship, const std::string& cargoDataPath, const std::string& instructionsPath){
    Port port;
    std::vector<bool> * res  =readPortContainers(&port, cargoDataPath);
    delete res;
    std::vector<Container*> problematics;
    std::vector<bool> errors;
    port.fixPort(errors, problematics);
    Crane crane(ship, &port);
    std::vector<Container*> wasOnPort;
    port.getVectorOfContainers(wasOnPort);
    int result = crane.executeOperationList(instructionsPath);
    if (result == -1) {return -1;} //Algo Did some invalid operation
    std::vector<Container*> leftOnPort;
    port.getVectorOfContainers(leftOnPort);
    if(!ship->isFull()){
        //check if algo took all the containers from the port
        for(auto& cont : leftOnPort){
            if(ship->willVisit(cont->getDest())){
                return -1;
            }
        }
    }
    else{
        std::vector<Container*> byDist;
        port.getContainersByDistance(ship->getRoute(), byDist);
        if(!validateFarRejected(leftOnPort, wasOnPort,byDist))
            return -1;
    }
    //check algo left on port only containers with correct destination
    for(auto& cont : leftOnPort){
        bool found = false;
        if(cont->getDest().compare(ship->getCurrentDestination())!=0){
            for(auto& tmp : wasOnPort){
                if(cont->getId().compare(tmp->getId()) == 0){
                    found = true;
                    break;
                }
            }
            if(!found) {return -1;} // left on port container with different destination
        }
    }
    return result;

}

bool findInVec(std::vector<Container*>& vec, const std::string& id){
    for(auto& cont : vec){
        if(cont->getId().compare(id) == 0){
            return true;
        }
    }
    return false;
}

bool validateFarRejected(std::vector<Container*>& left, std::vector<Container*>& was, std::vector<Container*> contByDist){
    std::vector<Container*> took;
    for(auto& cont : was){
        bool found = false;
        for(auto& tmp : left){
            if(cont->getId().compare(tmp->getId()) == 0){
                found = true;
                break;
            }
        }
        if(!found)
            took.push_back(cont);
    }
    std::set<std::string> tookDst;
    std::string firstRejected;
    for(auto& cont : contByDist){
        bool rejected = findInVec(left,cont->getId());
        if(firstRejected.empty() && !rejected) {
            continue; //we took this container and still not rejected
        }
        if(firstRejected.empty() && rejected){
            firstRejected = cont->getDest();
            continue; //first one we reject
        }
        if(rejected || (!rejected && firstRejected==cont->getDest())){
            continue; // Same distance - algo can choose
        }
        return false;

    }
    return true;
}
