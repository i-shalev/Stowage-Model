//
// Created by zivco on 10/04/2020.
//

#include "Port.h"

Port::Port(){
    ContainersAwaiting = new std::vector<Container*>;
}

Port::~Port(){
//        std::cout << "Delete Port: " << this->getSymbol() << std::endl;
    for(const auto& elem : *this->ContainersAwaiting ) {
        delete elem;
    }
    this->ContainersAwaiting->clear();
    delete this->ContainersAwaiting;
}

const std::string& Port::getSymbol() const {
    return symbol;
}

void Port::addContainer(Container* container){
    ContainersAwaiting->push_back(container);
}

void Port::removeContainer(const std::string& id){
    std::vector<Container*> temp;
    for(auto& container: *(this->ContainersAwaiting)){
        if(container->getId().compare(id)!=0)
            temp.push_back(container);
    }
    this->ContainersAwaiting->clear();
    for(auto& container: temp){
        ContainersAwaiting->push_back(container);
    }

}

bool Port::hasContainers() const{
    return !ContainersAwaiting->empty();
}

Container* Port::getContainerByID(const std::string& id) const{
    for(auto& container: *(this->ContainersAwaiting)){
        if(container->getId().compare(id)==0)
            return container;
    }
    return nullptr;
}

void Port::getVectorOfContainers(std::vector<Container*>& vector) const{
    for(const auto& elem : *this->ContainersAwaiting )
    {
        vector.push_back(elem);
    }
}
void Port::getContainersWithDest(const std::string& dest, std::vector<Container*>& vec) const{
    for(const auto& elem : *(this->ContainersAwaiting) )
    {
        if(elem->getDest().compare(dest)==0)
            vec.push_back(elem);
    }
}
void Port::getContainersByDistance(ShipRoute& sr, std::vector<Container*>& vec) const{
    std::set<std::string> seenBefore;
    for(auto elem : *(sr.getDstList())){
        if(seenBefore.find(elem) == seenBefore.end()) {
            getContainersWithDest(elem, vec);
            seenBefore.insert(elem);
        }
    }
    for(auto elem : *(this->ContainersAwaiting)){
        if(seenBefore.find(elem->getDest())==seenBefore.end()){
            vec.push_back(elem);
        }
    }
}

// errors[0] - 2^10, errors[1] - 2^12, errors[2] - 2^13, errors[3] - 2^15
void Port::fixPort(std::vector<bool>& errors, std::vector<Container*>& reject) {
    for(int i=0; i<4; i++)
        errors.push_back(false);
    std::vector<Container*> tmp;
    std::set<std::string> IDs;
    for(auto& cont : *this->ContainersAwaiting){
        if(!(cont->checkId())){
            reject.push_back(cont);
            errors.at(3) = true;
            continue;
        }
        if(cont->getWeight() <= 0){
            reject.push_back(cont);
            errors.at(1) = true;
            continue;
        }
        if(!(cont->checkDestination())){
            reject.push_back(cont);
            errors.at(2) = true;
            continue;
        }
        if(IDs.find(cont->getId()) != IDs.end()){
            reject.push_back(cont);
            errors.at(0) = true;
            continue;
        }
        IDs.insert(cont->getId());
        tmp.push_back(cont);
    }
    this->ContainersAwaiting->clear();
    for(auto& container: tmp){
        ContainersAwaiting->push_back(container);
    }

}