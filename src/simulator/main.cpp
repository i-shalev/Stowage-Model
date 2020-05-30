#include "main.h"


#define PATH_TO_EMPTY_FILE R"(.\empty.empty_file)"

int main(int argc, char **argv){
    std::map<std::string, std::string> args;
    std::vector<std::string> errors;
    int numThreads;
    bool errorInCreateArgs = false;
    if(createArgs(args, argc, argv)){
        errorInCreateArgs = true;
        errors.emplace_back("ERROR: travel_path not provided!");
    }
//    printArgs(args);
    try{
        numThreads = stoi(args["-num_threads"]);
    } catch (const std::exception& e) {
        numThreads = 1;
        errors.emplace_back("ERROR: num_threads that provided is not a number!");
    }
    int res = folderIsExistOrCanBeBuilt(args["-output"]);
    if(res == 1){
        errors.emplace_back("Warning: output path that provided is non existent folder but we successfully create it.");
    } else if (res == 2) {
        errors.emplace_back("Warning: output path that provided is not a valid path, the output files will be under the folder you run the program.");
        args["-output"] = "./";
    }
    if(!errorInCreateArgs and !isFolderExist(args["-travel_path"])){
        errorInCreateArgs = true;
        errors.emplace_back("ERROR: travel_path that provided is not a valid path.");
    }
    if(errorInCreateArgs){
        writeErrorsToFile(args["-output"] + "/errors/" + "general_errors.errors", args["-output"] + "/errors/", &errors);
        return EXIT_FAILURE;
    }
    if(!isFolderExist(args["-algorithm_path"])){
        errors.emplace_back("Warning: algorithm_path that provided is not a valid path. so no algorithms runs.");
    } else {
      if(numThreads == 1) {
        runAllAlgo(args["-algorithm_path"], args["-travel_path"], args["-output"]);
      } else {
        // here we call the threads function.
        runThreads(numThreads, args["-algorithm_path"], args["-travel_path"], args["-output"]);
      }
    }
    writeErrorsToFile(args["-output"] + "/errors/" + "general_errors.errors", args["-output"] + "/errors/", &errors);
    return EXIT_SUCCESS;
}

int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv){
    for(int i = 1; i < argc-1; i += 2){
        args[argv[i]] = argv[i+1];
    }
    if(args["-algorithm_path"].empty()){
        args["-algorithm_path"] = "./";
    }
    if(args["-num_threads"].empty()){
        args["-num_threads"] = "1";
    }
    if(args["-output"].empty()){
        args["-output"] = "./";
    }
    if(args["-travel_path"].empty()){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void printArgs(std::map<std::string, std::string>& args){
    std::cout << "travel_path: " << args["-travel_path"] << std::endl;
    std::cout << "num_threads: " << args["-num_threads"] << std::endl;
    std::cout << "algorithm_path: " << args["-algorithm_path"] << std::endl;
    std::cout << "output: " << args["-output"] << std::endl;
}

void runThreads(int numThreads, const std::string& algoPath, const std::string &travelPath, const std::string &outputPath){
    auto algoNames = getFileNamesEndWith(algoPath, ".so");
    if(algoNames->empty()) {
        std::cout << "There was no .so files in the algorithm_path" << std::endl;
        return;
    }
    auto& registrar = AlgorithmRegistrar::getInstance();

    emptyFile(PATH_TO_EMPTY_FILE);
    auto dirs = getDirsNamesFromRootDir(travelPath);
    std::vector<std::string> firstLine;


    std::vector<std::string> vectorAlgoNames;
    for(const auto& algoName:*algoNames) {
        std::string fullName = algoPath + "/" + algoName + ".so";
        char* fullNameChar = (char *)(malloc((fullName.size() + 1) * sizeof(char)));
        stringToCharStar(fullNameChar, fullName);

        std::string error;
        if(registrar.loadAlgorithmFromFile(fullNameChar, error)){
            vectorAlgoNames.push_back(algoName);
        } else {
            std::vector<std::string> errors;
            errors.push_back(error);
            writeErrorsToFile(outputPath + "/errors/" + algoName + ".errors", outputPath + "/errors/", &errors);
        }
    }

    //Amir's code
    ThreadPoolExecuter<TasksProducer> executer{NumThreads{numThreads}, dirs.get(),vectorAlgoNames, outputPath, travelPath };
    std::cout << "first cycle started" << std::endl;
    executer.start();
    executer.wait_till_finish();
    std::cout << "first cycle finished" << std::endl;
    // end of Amir's code


}

void runAllAlgo(const std::string& algoPath, const std::string &travelPath, const std::string &outputPath){
    auto algoNames = getFileNamesEndWith(algoPath, ".so");
    if(algoNames->empty()) {
        std::cout << "There was no .so files in the algorithm_path" << std::endl;
    }
    auto& registrar = AlgorithmRegistrar::getInstance();

    emptyFile(PATH_TO_EMPTY_FILE);
    emptyFile(outputPath + "/simulation.results");
    auto dirs = getDirsNamesFromRootDir(travelPath);
    std::vector<std::string> firstLine;
    firstLine.emplace_back("RESULTS");
    for(const auto& dir:*dirs)
        firstLine.push_back(dir);
    firstLine.emplace_back("Sum");
    firstLine.emplace_back("Num Errors");
    writeToSuccessFile(outputPath + "/simulation.results", &firstLine);

    std::vector<std::string> vectorAlgoNames;
    for(const auto& algoName:*algoNames) {
        std::string fullName = algoPath + "/" + algoName + ".so";
        char* fullNameChar = (char *)(malloc((fullName.size() + 1) * sizeof(char)));
        stringToCharStar(fullNameChar, fullName);

        std::string error;
        if(registrar.loadAlgorithmFromFile(fullNameChar, error)){
            vectorAlgoNames.push_back(algoName);
        } else {
            std::vector<std::string> errors;
            errors.push_back(error);
            writeErrorsToFile(outputPath + "/errors/" + algoName + ".errors", outputPath + "/errors/", &errors);
        }
    }
    int i = 0;
    for (auto algo_iter = registrar.begin(); algo_iter != registrar.end(); ++algo_iter) {
        auto algo = (*algo_iter)();
        runAlgoForAllTravels(*algo, travelPath, outputPath, vectorAlgoNames.at(i), dirs.get());
        i++;
    }
}

void runAlgoForAllTravels(AbstractAlgorithm &algo, const std::string &travelPath, const std::string &outputPath,
                          const std::string &algoName, std::vector<std::string>* dirs) {
    std::vector<std::string> results;
    int sum = 0, numErrors = 0, tmp;
    results.push_back(algoName);
    for(const auto& dir:*dirs) {
        tmp = runAlgoForTravel(algo, travelPath + "/" + dir, outputPath, algoName, dir);
        if(tmp == -1)
            numErrors++;
        else
            sum += tmp;
        results.push_back(std::to_string(tmp));
    }
    results.push_back(std::to_string(sum));
    results.push_back(std::to_string(numErrors));
    writeToSuccessFile(outputPath + "/simulation.results", &results);
}

int runAlgoForTravel(AbstractAlgorithm &algo, const std::string &pathToDir, const std::string &outputPath,
                     const std::string &algoName, const std::string &travelName) {
//    std::cout << "start" << algoName << " - " << travelName << pathToDir << std::endl;
    auto errors = std::make_unique<std::vector<std::string>>();
    bool fatalError = false;
    std::string shipPlanPath, shipRoutePath;
    int result = getShipPlanAndRoutePaths(pathToDir, shipPlanPath, shipRoutePath);
    if(shipPlanPath.empty()){
        fatalError = true;
        errors->push_back("ERROR: no shipPlan file!");
    }
    if(shipRoutePath.empty()){
        fatalError = true;
        errors->push_back("ERROR: no shipRoute file!");
    }
    if(fatalError){
        writeErrorsToFile(outputPath + "/errors/" + algoName + "_" + travelName + ".errors", outputPath + "/errors/", errors.get());
        return -1;
    }

    if(result != 0){
        if(result == 1 or result == 3)
            errors->push_back("Warning: there are more than 1 ship_plan file!");
        if(result > 1)
            errors->push_back("Warning: there are more than 1 route file!");
    }

    int res = 0;
    auto* shipPlan = createShipPlan(res, shipPlanPath);
    int errorCode = algo.readShipPlan(shipPlanPath);
    if(errorCode > 0){
        std::string errorCodeStr;
        getStringOfErrors(errorCode, errorCodeStr);
        errorCodeStr = "While read the ShipPlan, the algorithm return the errors: " + errorCodeStr;
        errors->push_back(errorCodeStr);
        if(containsFatalError(errorCode)){
            writeErrorsToFile(outputPath + "/errors/" + algoName + "_" + travelName + ".errors", outputPath + "/errors/",
                              errors.get());
            delete shipPlan;
            return -1;
        }
    }

    res = 0;
    auto* shipRoute = createShipRoute(res, shipRoutePath);

    errorCode = algo.readShipRoute(shipRoutePath);
    if(errorCode > 0){
        std::string errorCodeStr;
        getStringOfErrors(errorCode, errorCodeStr);
        errorCodeStr = "While read the ShipRoute, the algorithm return the errors: " + errorCodeStr;
        errors->push_back(errorCodeStr);
        if(containsFatalError(errorCode)){
            writeErrorsToFile(outputPath + "/errors/" + algoName + "_" + travelName + ".errors", outputPath + "/errors/",
                              errors.get());
            delete shipPlan;
            delete shipRoute;
            return -1;
        }
    }
    WeightBalanceCalculator wb;
    algo.setWeightBalanceCalculator(wb);

    Ship* ship = new Ship(shipRoute, shipPlan);
    auto mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());
    auto mapPortFullNameToCargoPath = createMapPortFullNameToCargoPath(pathToDir, mapPortVisits.get(),
            shipRoute->getDstList()->at(shipRoute->getDstList()->size()-1), errors.get());

    int numOp = 0;
    while(!ship->finishRoute()){
        //std::cout << ship->getCurrentDestinationWithIndex() << std::endl;
        std::string pathToInstructions = outputPath + "/" + algoName + "_" + travelName + "_crane_instructions" + "/" + ship->getCurrentDestinationWithIndex() + ".instructions";
        createFolder(outputPath + "/" + algoName + "_" + travelName + "_crane_instructions");
        int instErrorCode = algo.getInstructionsForCargo(mapPortFullNameToCargoPath.get()->at(ship->getCurrentDestinationWithIndex()), pathToInstructions);
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

    writeErrorsToFile(outputPath + "/errors/" + algoName + "_" + travelName + ".errors", outputPath + "/errors/", errors.get());
    delete(ship);
//    std::cout << "finished:" << algoName << " - " << travelName << pathToDir << std::endl;
    return numOp;
}

int getShipPlanAndRoutePaths(const std::string& pathToDir, std::string& shipPlanPath, std::string& shipRoutePath){
    int num = 0;
    auto res = getFileNamesEndWith(pathToDir, ".ship_plan");
    if(!res->empty()){
        shipPlanPath = pathToDir + "/" + res->at(0) + ".ship_plan";
        if(res->size() > 1)
            num += 1;
    }

    res = getFileNamesEndWith(pathToDir, ".route");
    if(!res->empty()) {
        shipRoutePath = pathToDir + "/" + res->at(0) + ".route";
        if(res->size() > 1)
            num += 2;
    }
    return num;
}

ShipPlan* createShipPlan(int &errorCode, const std::string& shipPlanPath){
    bool fatalError = false;
    int numFloors=0 , length=0, width=0, numLines;
    if(!getSizesShipPlan(shipPlanPath, numFloors, length, width, numLines)) {
        errorCode = turnToTrueBit(errorCode,3);
        return nullptr;
    }

    // create the ShipPlanVector
    auto blocks = std::make_unique<std::vector<std::vector<int>>>(numLines-1);
    auto results = readShipPlanInFiles(*blocks, shipPlanPath, numFloors, length, width);
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

    auto* shipPlan = new ShipPlan(numFloors, length, width, *(blocks.get()));

    return shipPlan;
}

ShipRoute* createShipRoute(int &errorCode, const std::string& shipRoutePath){
    bool fatalError = false;
    auto ports = std::make_unique<std::vector<std::string>>();
    auto results = readShipPorts(*ports, shipRoutePath);
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
    auto* shipRoute = new ShipRoute(ports.get());
    return shipRoute;
}

std::unique_ptr<std::map<std::string, int>> createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList) {
    auto mapPortVisits = std::make_unique<std::map<std::string, int>>();
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

std::unique_ptr<std::map<std::string, std::string>> createMapPortFullNameToCargoPath(const std::string &pathToDir,
        std::map<std::string, int> *mapPortVisits, const std::string &lastPort, std::vector<std::string>* errors){
    auto mapPortFullNameToCargoPath = std::make_unique<std::map<std::string, std::string>>();
    addPortsWithFileToMap(pathToDir, mapPortVisits, mapPortFullNameToCargoPath.get(), errors);
    addPortsWithNoFileToMap(mapPortVisits, mapPortFullNameToCargoPath.get(), lastPort, errors);
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
            if(res !=  mapPortVisits->end() and res->second >= indexNumber and indexNumber > 0) {
                fullname =  pathToDir + '/';
                fullname += name;
                fullname += R"(.cargo_data)";
                mapPortFullNameToCargoPath->insert({name, fullname});
            } else {
                errors->push_back("Warning: the file " + name + ".cargo_data is not necessary");
//                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        } else {
            errors->push_back("Warning: the file " + name + ".cargo_data is not in the right format");
//            std::cout << "Warning: the file " << name << ".cargo_data is not in the right format" << std::endl;
        }
    }
    delete pathToDirChar;
}

void addPortsWithNoFileToMap(std::map<std::string, int> *mapPortVisits,
                             std::map<std::string, std::string> *mapPortFullNameToCargoPath,
                             const std::string &lastPort, std::vector<std::string> *errors) {
    for(const auto& elem : *mapPortVisits)
    {
        for(int i = 1; i <= elem.second; i++)
        {
            std::string port_index = elem.first + "_" + std::to_string(i);
            auto res = mapPortFullNameToCargoPath->find(port_index);
            if(!mapPortFullNameToCargoPath->empty() && res!=mapPortFullNameToCargoPath->end()){
                continue;
            }

            if(!(lastPort == elem.first and i == elem.second)){
                errors->push_back("Warning: the file " + elem.first + "_" + std::to_string(i) + ".cargo_data is missing.");
//                std::cout << "Warning: the file " << elem.first + "_" + std::to_string(i) << ".cargo_data is missing." << std::endl;
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

bool findInVec(std::vector<Container*>& vec, const std::string& id){
    for(auto& cont : vec){
        if(cont->getId().compare(id) == 0){
            return true;
        }
    }
    return false;
}
bool findInVec(std::vector<std::string>& vec, const std::string& id){
    for(auto& cont : vec){
        if(cont.compare(id) == 0){
            return true;
        }
    }
    return false;
}

int runAlgoOnPort(Ship *ship, const std::string& cargoDataPath, const std::string& instructionsPath, std::vector<std::string>& errorReason){
    Port port;
    auto res  = readPortContainers(&port, cargoDataPath);
    std::vector<Container*> problematics;
    std::vector<bool> errors;
    port.fixPort(errors, problematics);
    Crane crane(ship, &port);
    std::vector<Container*> wasOnPort;
    port.getVectorOfContainers(wasOnPort);
    std::vector<std::string> wasOnShip;
    ship->getAllContainerIds(wasOnShip);
    std::vector<std::string> err;
    int result = crane.executeOperationList(instructionsPath, err);
    if (result == -1) {
        errorReason.emplace_back("The algorithm's instruction: " + err.at(0));
        return -1;
    } //algorithm Did some invalid operation
    std::vector<Container*> leftOnPort;
    port.getVectorOfContainers(leftOnPort);
    if(!ship->isFull()){
        //check if algo took all the containers from the port
        for(auto& cont : leftOnPort){
            if(ship->willVisit(cont->getDest()) && !ship->hasContainer(cont->getId()) && !findInVec(problematics, cont->getId()) && !findInVec(wasOnShip, cont->getId())){
                errorReason.push_back("Ship is not empty, reject container " + cont->getId() + " without reason.");
                return -1;
            }
        }
    }
    else{
        std::vector<Container*> byDist;
        port.getContainersByDistance(ship->getRoute(), byDist);
        if(!validateFarRejected(leftOnPort, wasOnPort,byDist)) {
            errorReason.emplace_back("The algorithm did not reject further containers.");
            return -1;
        }
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
            if(!found) {
                errorReason.emplace_back("The algorithm unload containers which is not their target (and didn't upload them back).");
                return -1;
            } // left on port container with different destination
        }
    }
    return result;

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

int getBitInNumber(int num, int bit){
    int mask =  1 << bit;
    int masked_n = num & mask;
    return masked_n >> bit;
}

void getStringOfErrors(int num, std::string& result){
    std::vector<int> errors;
    for( int i=0; i<19; i++){
        if(getBitInNumber(num, i))
            errors.push_back(i);
    }
    //assume num != 0
    for(size_t i=0; i<errors.size()-1; i++){
        result.append( std::to_string(errors.at(i)) + " ,");
    }
    result.append(std::to_string(errors.at(errors.size() - 1)));
}

bool containsFatalError(int errorCode){
    std::vector<int> fatalErrorCode = {3, 4, 7, 8};
    for(auto fed : fatalErrorCode){
        if(getBitInNumber(errorCode, fed))
            return true;
    }
    return false;
}

int turnToTrueBit(int num, int bit){
    int mask = 1 << bit;
    return num | mask;
}

// 0 the path is dir
// 1 the path is non existent dir but we create that
// 2 the path is non existent dir and we didn't create that or not a dir
int folderIsExistOrCanBeBuilt(const std::string& path){
    char* pathChar = (char *)(malloc((path.size() + 1) * sizeof(char)));
    stringToCharStar(pathChar, path);

    struct stat info;

    if( stat( pathChar, &info ) != 0 ){
        createFolder(path);
        if(isFolderExist(path))
            return 1;
        else
            return 2;
    }
    else if( info.st_mode & S_IFDIR )
        return 0;
    else
        return 2;
}

bool isFolderExist(const std::string& path){
    char* pathChar = (char *)(malloc((path.size() + 1) * sizeof(char)));
    stringToCharStar(pathChar, path);
    struct stat info;

    if( stat( pathChar, &info ) != 0 )
        return false;
    else return (info.st_mode & S_IFDIR) != 0;
}

void createFolder(const std::string& path){
    char* pathChar = (char *)(malloc((path.size() + 1) * sizeof(char)));
    stringToCharStar(pathChar, path);

    char* cmd = (char *)(malloc((path.size() + 1 + 10) * sizeof(char)));
    strcpy(cmd, "mkdir -p ");
    strcat(cmd,pathChar);
    system(cmd);
    delete cmd;
}