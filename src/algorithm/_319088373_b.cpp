//
// Created by itay on 13/06/2020.
//

#include "_319088373_b.h"

REGISTER_ALGORITHM(_319088373_b)
_319088373_b::_319088373_b(){
    this->shipRoute = nullptr;
    this->ship = nullptr;
    this->shipPlan = nullptr;
    this->calc = nullptr;
}
//return the position of the highest container, need to add 1 to height to get the position of the new one
std::tuple<int, int, int> getColWithSameDestOnTop(const std::string dest, Ship* ship){
    std::tuple<int, int, int> ans;
    for(int i=0; i<ship->getPlan().getLength();i++) {
        for (int j = 0; j < ship->getPlan().getWidth(); j++) {
            std::string highestDestSeen = "";
            if (ship->getPlan().getFloor(ship->getPlan().getNumFloors()-1)->getContainerAtPosition(i, j) != nullptr)
                continue; // col is full
            for (int level = 0; level < ship->getPlan().getNumFloors(); level++) {
                if (ship->getPlan().getFloor(level)->getContainerAtPosition(i, j) == nullptr)
                    break;
                highestDestSeen = ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getDest();
                ans = std::tuple<int, int, int>{i, j, level};
            }
            if (highestDestSeen == "")
                continue;
            if (highestDestSeen == dest) {
                return ans;
            }
        }
    }
    return std::tuple<int, int, int> {-1, -1, -1};
}

//return the position of the highest container, need to add 1 to height to get the position of the new one
std::tuple<int, int, int> getColWithoutContainers(Ship* ship){
    std::tuple<int, int, int> ans{-1, -1, -1};
    for(int i=0; i<ship->getPlan().getLength();i++) {
        for (int j = 0; j < ship->getPlan().getWidth(); j++) {
            for (int level = ship->getPlan().getNumFloors() -1 ; level >= 0; level--) {
                if (level == 0 && ship->getPlan().getFloor(level)->getContainerAtPosition(i, j) == nullptr)
                    return std::tuple<int, int, int>{i, j, -1};
                if (ship->getPlan().getFloor(level)->getContainerAtPosition(i, j) == nullptr)
                    continue;
                if (ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getBlocked()) {
                    if(level == ship->getPlan().getNumFloors() -1 )
                        break;
                    return std::tuple<int, int, int>{i, j, level};
                }
                else
                    break;
            }
        }
    }
    return ans;
}

//return the position of the highest container, need to add 1 to height to get the position of the new one
std::tuple<int, int, int> getColWithFarDestinationOnTop(const std::string dest, Ship* ship){
    std::tuple<int, int, int> res;
    std::tuple<int, int, int> ans;

    std::string minDest = "";

    for(int i=0; i<ship->getPlan().getLength();i++) {
        for (int j = 0; j < ship->getPlan().getWidth(); j++) {
            std::string highestDestSeen = "";
            if (ship->getPlan().getFloor(ship->getPlan().getNumFloors()-1)->getContainerAtPosition(i, j) != nullptr)
                continue; // col is full
            for (int level = 0; level < ship->getPlan().getNumFloors(); level++) {
                if (ship->getPlan().getFloor(level)->getContainerAtPosition(i, j) == nullptr)
                    break;
                highestDestSeen = ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getDest();
                ans = std::tuple<int, int, int>{i, j, level};
            }
            if (highestDestSeen == "")
                continue;
            if (ship->getRoute().seenBefore(dest, highestDestSeen)) { // this container will unload before
                if(minDest == ""){
                    minDest = highestDestSeen;
                    res = ans;
                } else {
                    if(ship->getRoute().seenBefore(highestDestSeen, minDest)) {
                        minDest = highestDestSeen;
                        res = ans;
                    }
                }
            }
        }
    }
    if(minDest != "") {
        return res;
    }
    return std::tuple<int, int, int>{-1, -1, -1};
}

//return the position of the highest container, need to add 1 to height to get the position of the new one
std::tuple<int, int, int> findEmptyLegalPositionExceptXY(Ship* ship, int x, int y){
    std::tuple<int, int, int> ans{-1, -1, -1};
    for(int i=0; i < ship->getPlan().getLength();i++) {
        for (int j = 0; j < ship->getPlan().getWidth(); j++) {
            if(i==x && j == y)
                continue;
            for (int level = 0 ; level < ship->getPlan().getNumFloors(); level++) {
                if (ship->getPlan().getFloor(level)->getContainerAtPosition(i, j) == nullptr)
                    return std::tuple<int, int, int>{i, j, level-1};
            }
        }
    }
    return ans;
}

//return the position of the highest container, need to add 1 to height to get the position of the new one
std::tuple<int, int, int> findEmptyLegalPositionInPlan(Ship* ship){
    std::tuple<int, int, int> res{-1, -1, -1};

    std::string maxDest = "";

    for(int i=0; i < ship->getPlan().getLength();i++) {
        for (int j = 1; j < ship->getPlan().getWidth(); j++) {
            if (ship->getPlan().getFloor(ship->getPlan().getNumFloors() - 1)->getContainerAtPosition(i, j) != nullptr)
                continue;
            for (int level = ship->getPlan().getNumFloors() - 1 ; level >= 0 ; level--) {
                if (ship->getPlan().getFloor(level)->getContainerAtPosition(i, j) != nullptr) {
                    if(ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getBlocked()){
                        break;
                    }
                    std::string highestDestSeen = ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getDest();
                    if(maxDest == ""){
                        maxDest = highestDestSeen;
                        res = std::tuple<int, int, int>{i, j, level};
                    } else {
                        if(ship->getRoute().seenBefore(maxDest, highestDestSeen)) {
                            maxDest = highestDestSeen;
                            res = std::tuple<int, int, int>{i, j, level};
                        }
                    }
                    break;
                }
            }
        }
    }
    return res;
}

std::tuple<int, int, int> findBestPlaceToLoad(const std::string dest, Ship* ship){
    std::tuple<int, int, int> ans;
    ans = getColWithSameDestOnTop(dest, ship);
    if(std::get<0>(ans) != -1){
        //std::cout << "best: same dest" << std::endl;
        return std::tuple<int, int, int>{ std::get<0>(ans), std::get<1>(ans), std::get<2>(ans) + 1};
    }
    ans = getColWithFarDestinationOnTop(dest, ship);
    if(std::get<0>(ans) != -1){
        //std::cout << "best: on far dest" << std::endl;
        return std::tuple<int, int, int>{ std::get<0>(ans), std::get<1>(ans), std::get<2>(ans) + 1};
    }
    ans = getColWithoutContainers(ship);
    if(std::get<0>(ans) != -1){
        //std::cout << "best: empty col" << std::endl;
        return std::tuple<int, int, int>{ std::get<0>(ans), std::get<1>(ans), std::get<2>(ans) + 1};
    }
    ans = findEmptyLegalPositionInPlan(ship);
    if(std::get<0>(ans) != -1) {
        //std::cout << "best: I have no options..." << std::endl;
        return std::tuple<int, int, int>{std::get<0>(ans), std::get<1>(ans), std::get<2>(ans) + 1};
    }

    ans = findEmptyLegalPositionExceptXY(ship, -1, -1);
    if(std::get<0>(ans) != -1) {
        //std::cout << "best: I have no options..." << std::endl;
        return std::tuple<int, int, int>{std::get<0>(ans), std::get<1>(ans), std::get<2>(ans) + 1};
    }
    return std::tuple<int,int,int>{-1,-1,-1}; // ship is full...

}

int calculateHeight(Ship* ship, int x, int y){
    for (int level = 0 ; level < ship->getPlan().getNumFloors(); level++) {
        if (ship->getPlan().getFloor(level)->getContainerAtPosition(x, y) == nullptr)
            return level;
    }
    return -1;
}

void unloadProcedure(Ship* ship, Port* port, std::fstream& fs){
    Crane c(ship,port);
    std::vector<std::tuple<char, std::string, int, int, int, int, int, int>> operations;
    for(int i=0; i<ship->getPlan().getLength();i++){
        for(int j=0; j<ship->getPlan().getWidth(); j++){
            int lowestFloor = -1;
            for(int level=0;level<ship->getPlan().getNumFloors(); level++){
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr)
                    break;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest()==ship->getCurrentDestination()){
                    lowestFloor = level;
                    break;
                }
            }
            if(lowestFloor == -1)
                continue;
            for(int level = ship->getPlan().getNumFloors()-1; level>=lowestFloor; level --){
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr)
                    continue;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getBlocked())
                    break;
                std::string contId = ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getId();
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest() == ship->getCurrentDestination()) {
                    c.Unload(contId, level, i, j);
                    fs << "U, "<< contId << ", " << level << ", " << i << ", " << j <<std::endl;
                    continue;
                }
                auto ans = findEmptyLegalPositionExceptXY(ship, i, j);
                if(std::get<0>(ans)!=-1){
                    c.Move(contId,level,i,j, std::get<2>(ans)+1, std::get<0>(ans), std::get<1>(ans));
                    operations.push_back(std::tuple<char, std::string, int, int, int, int, int, int>{'M', contId,level,i,j, std::get<2>(ans)+1, std::get<0>(ans), std::get<1>(ans)});
                    fs << "M, "<< contId << ", " << level << ", " << i << ", " << j << ", " <<
                       (std::get<2>(ans) + 1) << ", " << std::get<0>(ans) << ", " << std::get<1>(ans)  << std::endl;
                }
                else{
                    c.Unload(contId,level,i,j);
                    operations.push_back(std::tuple<char, std::string, int, int, int, int, int, int>{'U', contId,level,i,j,-1,-1,-1});
                    fs << "U, "<< contId << ", " << level << ", " << i << ", " << j <<std::endl;
                }
            }
            // rollback
            while(!operations.empty()){
                auto op = operations.back();
                if(std::get<0>(op) == 'U') {
                    int x = std::get<3>(op);
                    int y = std::get<4>(op);
                    int height =  calculateHeight(ship, x, y);
                    c.Load(std::get<1>(op), height, x, y);
                    fs << "L, "<< std::get<1>(op) << ", " << height << ", " << x << ", " << y <<std::endl;
                }
                operations.pop_back();
            }
        }
    }
}

bool checkContainer(Ship* ship, Container* cont) {
    return cont->checkId() && ship->willVisit(cont->getDest()) &&
           (ship->getCurrentDestination().compare(cont->getDest()) != 0);
}

bool LoadProcedure(Ship* ship, Port* port, std::vector<Container*> toLoad, std::fstream& fs){
    Crane c(ship, port);
    bool rejectBecauseFull = false;
    while(!toLoad.empty()) {
        if(checkContainer(ship, toLoad.front())) {
            //std::cout << "calculate best place for "<<toLoad.front()->getId() << std::endl;
            auto pos = findBestPlaceToLoad(toLoad.front()->getDest(), ship);
            //std::cout << "found" << toLoad.front()->getId() << ", " << std::get<2>(pos) << ", " << std::get<0>(pos) << ", " << std::get<1>(pos) << std::endl;
            int x = std::get<0>(pos);
            int y = std::get<1>(pos);
            int height = std::get<2>(pos);
            //std::cout << "found" << toLoad.front()->getId() << ", " << height << ", " << x << ", " << y << std::endl;
            if(x != -1){
                fs << "L, " << toLoad.front()->getId() << ", " << height << ", " << x << ", " << y << std::endl;
                c.Load(toLoad.front()->getId(), height, x, y);
            }
            else{
                fs << "R, " << toLoad.front()->getId() << std::endl;
                rejectBecauseFull = true;
            }
        }
        else{
            fs << "R, " << toLoad.front()->getId() << std::endl;
        }
        toLoad.erase(toLoad.begin());
    }
    return rejectBecauseFull;
}

int _319088373_b::getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) {
    int rc = 0;
    if(ship == nullptr){
        if(shipPlan == nullptr){rc = turnToTrueBit(rc, 3);}
        if(shipRoute == nullptr){rc = turnToTrueBit(rc, 7);}
        return rc;
    }
    char* pathToDirChar = (char *)(malloc((output_full_path_and_file_name.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, output_full_path_and_file_name);
    std::remove(pathToDirChar);
    Port port;
    auto res  =readPortContainers(&port, input_full_path_and_file_name);
    if(res->at(0)){ rc = turnToTrueBit(rc, 14);}
    if(res->at(1)){ rc = turnToTrueBit(rc, 16);}
    if(ship->lastStop()){
        if(port.hasContainers()){
            rc = turnToTrueBit(rc,17);
            std::vector<Container*> ignore;
            port.getVectorOfContainers(ignore);
            for(auto& cont: ignore){ port.removeContainer(cont->getId()); delete cont;}
        }
    }
    std::vector<Container*> problematics; std::vector<bool> errors; port.fixPort(errors, problematics);
    if(errors.at(0)){ rc = turnToTrueBit(rc, 10);}
    if(errors.at(1)){ rc = turnToTrueBit(rc, 12);}
    if(errors.at(2)){ rc = turnToTrueBit(rc, 13);}
    if(errors.at(3)){ rc = turnToTrueBit(rc, 15);}
    std::fstream fs; fs.open(output_full_path_and_file_name, std::ios::out | std::ios::app);
    for(auto& cont : problematics){ fs << "R, " << cont->getId() << std::endl; delete cont;}
    std::vector<Container*> conts;
    port.getVectorOfContainers(conts);
    for(auto& cont : conts){
        if(ship->hasContainer(cont->getId())) {
            fs << "R, " << cont->getId() << std::endl;
            rc = turnToTrueBit(rc, 11);
            port.removeContainer(cont->getId());
            delete cont;
        }
    }

    unloadProcedure(ship, &port, fs);
    std::vector<Container*> toLoad;
    int numPlacesOnShip = ship->numEmptyPlaces();
    port.getContainersByDistance(ship->getRoute(),toLoad);
    std::vector<Container*> toLoadInOrder;
    int maxIndex = ((int)toLoad.size() < (int)numPlacesOnShip)? toLoad.size() : numPlacesOnShip;
    for(int i = maxIndex-1; i >= 0; i--) {
        toLoadInOrder.push_back(toLoad.at(i));
    }
    for(size_t i = maxIndex; i < toLoad.size(); i++) {
        toLoadInOrder.push_back(toLoad.at(i));
    }
    // load procedure
    if(LoadProcedure(ship,&port, toLoadInOrder, fs))
        rc = turnToTrueBit(rc, 18);
    fs.close();
    delete pathToDirChar;
    ship->moveToNextPort();
    return rc;
}

int _319088373_b::emptyPlacesInPosition(int i, int j, const std::string& portSymbol){
    int sum = 0;
    for(int level=0; level<ship->getPlan().getNumFloors(); level++){
        if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)== nullptr ||
           (!ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getBlocked() &&
            ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest()==portSymbol)){
            sum++;
        }
    }
    return sum;
    //sum last positions are empty
}

bool _319088373_b::checkContainer(Container* cont){
    return cont->checkId() && ship->willVisit(cont->getDest()) && (ship->getCurrentDestination().compare(cont->getDest()) != 0);
}

int _319088373_b::readShipPlan(const std::string &full_path_and_file_name) {
    int errorCode = 0;
    bool fatalError = false;
    int numFloors=0 , length=0, width=0, numLines;
    if(!getSizesShipPlan(full_path_and_file_name, numFloors, length, width, numLines)) {
        return turnToTrueBit(errorCode,3);
    }

    // create the ShipPlanVector
    auto blocks = std::make_unique<std::vector<std::vector<int>>>(numLines-1);
    auto results = readShipPlanInFiles(*(blocks.get()), full_path_and_file_name, numFloors, length, width);
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
        return errorCode;

    this->shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    createShip();
    return errorCode;
}

int _319088373_b::readShipRoute(const std::string &full_path_and_file_name) {
    int errorCode = 0;
    bool fatalError = false;
    auto ports = std::make_unique<std::vector<std::string>>();
    auto results = readShipPorts(*(ports.get()), full_path_and_file_name);
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
        return errorCode;

    this->shipRoute = new ShipRoute(ports.get());

    createShip();
    return errorCode;
}

void _319088373_b::createShip() {
    if(this->shipPlan != nullptr and this->shipRoute != nullptr and this->calc != nullptr){
        ship = new Ship(shipRoute, shipPlan);
    }
}

void _319088373_b::printShipPlan(){
    this->shipPlan->printShipPlan();
}

void _319088373_b::printShipRoute() {
    this->shipRoute->printList();
}

int _319088373_b::setWeightBalanceCalculator(WeightBalanceCalculator& calculator){
    this->calc = &calculator;
    createShip();
    return SUCCESS;
}

int turnToTrueBit(int num, int bit){
    int mask = 1 << bit;
    return num | mask;
}