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
    if(ship == nullptr)
        return -1; //TODO: maybe return different error code
    char* pathToDirChar = (char *)(malloc((output_full_path_and_file_name.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, output_full_path_and_file_name);
    std::remove(pathToDirChar);
    //create a port object from input file
    std::vector<std::string> errors;
    Port port("CURENT",0,&errors);
    std::vector<std::string> errors_from_port_file;
    std::vector<bool> * res  =readPortContainers(&port, input_full_path_and_file_name, &errors_from_port_file);
    //TODO : do something with errors
    delete res;

    std::fstream fs;
    fs.open(output_full_path_and_file_name, std::ios::out | std::ios::app);

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
    }
    fs.close();
    delete pathToDirChar;
    Crane crane(ship, &port);
    crane.executeOperationList(output_full_path_and_file_name);
    return 0;
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
        return turnToTrueBit(errorCode,2);
    }
    if(results->at(3)){
        return turnToTrueBit(errorCode,3);
    }
    if(results->at(0)){
        errorCode = turnToTrueBit(errorCode,0);
    }
    if(results->at(1)){
        errorCode = turnToTrueBit(errorCode,1);
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

int turnToTrueBit(int num, int bit){
    int mask = 1 << bit;
    return num | mask;
}