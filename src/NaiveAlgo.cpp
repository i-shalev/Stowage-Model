//
// Created by zivco on 05/04/2020.
//

#include "NaiveAlgo.h"

void NaiveAlgo::getInstructionForCargo(const std::string& outputPath) {
    char* pathToDirChar = (char *)(malloc((outputPath.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, outputPath);
    std::remove(pathToDirChar);
    std::fstream fs;
    fs.open(outputPath, std::ios::out | std::ios::app);

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
                if(tryOperation(/*'U', ship->getPlan().getFloor(level)->getContainerAtPosition(i, j)->getWeight(), 0, i,
                                j, 0,
                                0, 0*/) != APPROVED){
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
                if(tryOperation(/*'L', temporaryUnloaded.back()->getWeight(), 0, i, j, 0, 0, 0*/) != APPROVED){
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
    ship->getCurrentPort()->getContainersByDistance(ship->getRoute(),toLoad);
    int emptyPlacesAtPosition;
    bool done = toLoad.empty();
    for(int i=0; i<ship->getPlan().getLength() && !done; i++){
        for(int j=0; j< ship->getPlan().getWidth() && !done; j++){
            emptyPlacesAtPosition  = this->emptyPlacesInPosition(i,j,ship->getCurrentDestination());
            for(int level = ship->getPlan().getNumFloors() - emptyPlacesAtPosition; level<ship->getPlan().getNumFloors() && !done; level++){
                if(checkContainer(toLoad.front())) {
                    if (tryOperation(/*'L', toLoad.front()->getWeight(), 0, i, j, 0, 0, 0*/) != APPROVED) {
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