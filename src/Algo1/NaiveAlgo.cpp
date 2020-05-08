//
// Created by zivco on 05/04/2020.
//

#include "NaiveAlgo.h"

NaiveAlgo::NaiveAlgo(){
    this->shipRoute = nullptr;
    this->ship = nullptr;
    this->shipPlan = nullptr;
    this->calc = nullptr;
}

int NaiveAlgo::getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) {
    int rc = 0;
    if(ship == nullptr){
        if(shipPlan == nullptr)
            rc = turnToTrueBit(rc, 3);
        if(shipRoute == nullptr)
            rc = turnToTrueBit(rc, 7);
        return rc;
    }
    char* pathToDirChar = (char *)(malloc((output_full_path_and_file_name.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, output_full_path_and_file_name);
    std::remove(pathToDirChar);
    //create a port object from input file
    Port port;
    std::vector<bool> * res  =readPortContainers(&port, input_full_path_and_file_name);
    if(res->at(0)){ rc = turnToTrueBit(rc, 14);}
    if(res->at(1)){ rc = turnToTrueBit(rc, 16);}
    delete res;
    if(ship->lastStop()){
        if(port.hasContainers()){
            rc = turnToTrueBit(rc,17);
            std::vector<Container*> ignore;
            port.getVectorOfContainers(ignore);
            for(auto& cont: ignore){
                port.removeContainer(cont->getId());
                delete cont;
            }
        }
    }
    std::vector<Container*> problematics;
    std::vector<bool> errors;
    port.fixPort(errors, problematics);
    if(errors.at(0)){ rc = turnToTrueBit(rc, 10);}
    if(errors.at(1)){ rc = turnToTrueBit(rc, 12);}
    if(errors.at(2)){ rc = turnToTrueBit(rc, 13);}
    if(errors.at(3)){ rc = turnToTrueBit(rc, 15);}

    std::fstream fs;
    fs.open(output_full_path_and_file_name, std::ios::out | std::ios::app);

    for(auto& cont : problematics){
        fs << "R " << cont->getId() << std::endl;
        delete cont;
    }
    std::vector<Container*> conts;
    port.getVectorOfContainers(conts);
    for(auto& cont : conts){
        if(ship->hasContainer(cont->getId())) {
            fs << "R " << cont->getId() << std::endl;
            rc = turnToTrueBit(rc, 11);
            port.removeContainer(cont->getId());
            delete cont;
        }
    }

    std::vector<Container*> temporaryUnloaded;
    //first unload from ship all the containers with this destination
    for(int i=0; i<ship->getPlan().getLength();i++){
        for(int j=0; j<ship->getPlan().getWidth(); j++){
            int lowestFloor = -1;
            for(int level=0;level<ship->getPlan().getNumFloors(); level++){
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr)
                    break;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest()==ship->getCurrentDestination()){
                    //we need to unload all containers above
                    lowestFloor = level;
                    break;
                }
            }
            if(lowestFloor == -1) // no need to unload this tower
                continue;
            for(int level = ship->getPlan().getNumFloors()-1; level>=lowestFloor; level --){
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr)
                    continue;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getBlocked())
                    break;
                if(calc->tryOperation('U', ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getWeight(), i, j) != WeightBalanceCalculator::BalanceStatus::APPROVED){
//                    std::cout <<"unbalance..." << std::endl;
                }
                fs << "U "<< ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getId() << " " << level << " " << i << " " << j <<std::endl;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest()!=ship->getCurrentDestination()){
                    temporaryUnloaded.push_back(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j));
                }
            }
            int loadBackLevel=0;
            while(ship->getPlan().getFloor(loadBackLevel)->getContainerAtPosition(i,j)->getBlocked()){
                loadBackLevel++;
            }
            while(!(temporaryUnloaded.empty())){
                if(calc->tryOperation('L', temporaryUnloaded.back()->getWeight(), i, j) != WeightBalanceCalculator::BalanceStatus::APPROVED){
//                    std::cout <<"unbalance..." << std::endl;
                }
                fs << "L "<< temporaryUnloaded.back()->getId() << " " << loadBackLevel << " " << i << " " << j <<std::endl;
                loadBackLevel++;
                temporaryUnloaded.pop_back();
            }
        }
    }
    //now load everything from port to ship. check valid ids and destinations.
    std::vector<Container*> toLoad;
    port.getContainersByDistance(ship->getRoute(),toLoad);
    int emptyPlacesAtPosition;
    bool done = toLoad.empty();
    for(int i=0; i<ship->getPlan().getLength() && !done; i++){
        for(int j=0; j< ship->getPlan().getWidth() && !done; j++){
            emptyPlacesAtPosition  = this->emptyPlacesInPosition(i,j,ship->getCurrentDestination());
            for(int level = ship->getPlan().getNumFloors() - emptyPlacesAtPosition; level<ship->getPlan().getNumFloors() && !done; level++){
                if(checkContainer(toLoad.front())) {
                    if (calc->tryOperation('L', toLoad.front()->getWeight(),i, j) != WeightBalanceCalculator::BalanceStatus::APPROVED) {
//                        std::cout << "unbalance..." << std::endl;
                    }
                    fs << "L " << toLoad.front()->getId() << " " << level << " " << i << " " << j << std::endl;
                }
                else{
                    fs << "R " << toLoad.front()->getId() << std::endl;
                    level--;
                }
                toLoad.erase(toLoad.begin());
                if(toLoad.empty()){
                    done = true;
                }
            }
        }
    }
    while(!toLoad.empty()) {
        fs << "R " << toLoad.back()->getId() << std::endl;
        toLoad.pop_back();
        rc = turnToTrueBit(rc, 18);
    }
    fs.close();
    delete pathToDirChar;
    Crane crane(ship, &port);
    crane.executeOperationList(output_full_path_and_file_name);
    ship->getRoute().deleteFirst();
    return rc;
}

int NaiveAlgo::emptyPlacesInPosition(int i, int j, const std::string& portSymbol){
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

bool NaiveAlgo::checkContainer(Container* cont){
    return cont->checkId() && ship->willVisit(cont->getDest());
}

int NaiveAlgo::readShipPlan(const std::string &full_path_and_file_name) {
    int errorCode = 0;
    int numFloors=0 , length=0, width=0, numLines;
    if(!getSizesShipPlan(full_path_and_file_name, numFloors, length, width, numLines)) {
        return turnToTrueBit(errorCode,3);
    }

    // create the ShipPlanVector
    auto* blocks = new std::vector<std::vector<int>>(numLines-1);
    auto* results = readShipPlanInFiles(*blocks, full_path_and_file_name, numFloors, length, width);
    if(results->at(1)){
        return turnToTrueBit(errorCode,3);
    }
    if(results->at(4)){
        return turnToTrueBit(errorCode,4);
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

    this->shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    delete blocks;
    delete results;
    createShip();
    return errorCode;
}

int NaiveAlgo::readShipRoute(const std::string &full_path_and_file_name) {
    int errorCode = 0;
    auto* ports = new std::vector<std::string>();
    auto* results = readShipPorts(*ports, full_path_and_file_name);
    if(results->at(2)){
        return turnToTrueBit(errorCode,7);
    }
    if(results->at(3)){
        return turnToTrueBit(errorCode,8);
    }
    if(results->at(0)){
        errorCode = turnToTrueBit(errorCode,5);
    }
    if(results->at(1)){
        errorCode = turnToTrueBit(errorCode,6);
    }
    this->shipRoute = new ShipRoute(ports);

    delete ports;
    delete results;
    createShip();
    return errorCode;
}

void NaiveAlgo::createShip() {
    if(this->shipPlan != nullptr and this->shipRoute != nullptr){
        ship = new Ship(shipRoute, shipPlan);
    }
}

void NaiveAlgo::printShipPlan(){
    this->shipPlan->printShipPlan();
}

void NaiveAlgo::printShipRoute() {
    this->shipRoute->printList();
}

int turnToTrueBit(int num, int bit){
    int mask = 1 << bit;
    return num | mask;
}

