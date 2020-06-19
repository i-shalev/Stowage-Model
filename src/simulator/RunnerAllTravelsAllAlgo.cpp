//
// Created by itay on 16/06/2020.
//

#include "RunnerAllTravelsAllAlgo.h"


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
    std::cout << "start "<< numThreads <<" threads" << std::endl;
    executer.start();
    executer.wait_till_finish();
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

    std::vector<std::vector<int>> results;
    std::vector<int> sums;
    std::vector<int> numErrs;
    int numTravels = dirs->size();
    int numAlgo = algoNames->size();
    for(int i = 0 ; i < numTravels ; i++){
        results.emplace_back();
        sums.push_back(0);
        numErrs.push_back(0);
        for(int j = 0 ; j < numAlgo ; j++){
            results.at(i).push_back(0);
        }
    }

    int i = 0;
    for (auto algo_iter = registrar.begin(); algo_iter != registrar.end(); ++algo_iter) {
        runAlgoForAllTravels(&algo_iter, travelPath, outputPath, vectorAlgoNames.at(i), dirs.get(), results, sums, numErrs, i);
        i++;
    }
    std::vector<int> vec;
    for(int j = 0 ; j < numAlgo ; j++){
        vec.push_back(j);
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
        lineResults.push_back(algoNames->at(j));
        for(i = 0 ; i < numTravels ; i++){
            lineResults.push_back(std::to_string(results.at(i).at(j)));
        }
        lineResults.push_back(std::to_string(sums.at(j)));
        lineResults.push_back(std::to_string(numErrs.at(j)));
        writeToSuccessFile(outputPath + "/simulation.results", &lineResults);
    }
}

void runAlgoForAllTravels(AlgorithmRegistrar::const_iterator *algoFactory, const std::string &travelPath, const std::string &outputPath,
                          const std::string &algoName, std::vector<std::string>* dirs, std::vector<std::vector<int>>& results,
                          std::vector<int>& sums, std::vector<int>& numErrs, int index) {
//    std::vector<std::string> results;
    int sum = 0, numErrors = 0, tmp;
//    results.push_back(algoName);
    int j = 0;
    for(const auto& dir:*dirs) {
        auto algo = (**algoFactory)();
        tmp = runAlgoForTravel(*algo, travelPath + "/" + dir, outputPath, algoName, dir);
        results.at(j).at(index) = tmp;
        if(tmp == -1)
            numErrors++;
        else
            sum += tmp;
        j++;
    }
    sums.at(index) = sum;
    numErrs.at(index) = numErrors;
//    results.push_back(std::to_string(sum));
//    results.push_back(std::to_string(numErrors));
//    writeToSuccessFile(outputPath + "/simulation.results", &results);
}

int runAlgoForTravel(AbstractAlgorithm &algo, const std::string &pathToDir, const std::string &outputPath,
                     const std::string &algoName, const std::string &travelName) {
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
    return numOp;
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
            if(ship->willVisit(cont->getDest()) && !ship->hasContainer(cont->getId()) && !findInVec(problematics, cont->getId())
               && !findInVec(wasOnShip, cont->getId()) && (ship->getCurrentDestination().compare(cont->getDest()) != 0)){
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
    for(int level=0; level<ship->getPlan().getNumFloors(); level++){
        for (int i = 0; i < ship->getPlan().getLength(); ++i) {
            for (int j = 0; j < ship->getPlan().getWidth(); ++j) {
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) != nullptr
                   && ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest().compare(ship->getCurrentDestination())==0){
                    errorReason.emplace_back("The algorithm visited a port and didnt unload all the containers that needs to be unloaded at this port.");
                    return -1;
                }
            }

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