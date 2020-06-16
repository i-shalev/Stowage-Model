//
// Created by itay on 16/06/2020.
//

#include "RunnerHelper.h"


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