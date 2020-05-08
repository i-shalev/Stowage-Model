//
// Created by zivco on 10/04/2020.
//

#include "Port.h"

Port::Port(){
    ContainersAwaiting = new std::map<std::string, Container*>;
}

Port::~Port(){
//        std::cout << "Delete Port: " << this->getSymbol() << std::endl;
    for(const auto& elem : *this->ContainersAwaiting ) {
        delete elem.second;
    }
    this->ContainersAwaiting->clear();
    delete this->ContainersAwaiting;
}

const std::string& Port::getSymbol() const {
    return symbol;
}

void Port::addContainer(Container* container){
    auto res = ContainersAwaiting->find(container->getId());
    if(!ContainersAwaiting->empty() && res!=ContainersAwaiting->end()){
        std::cout << "Warning: the port already have this container" << std::endl;
        return;
    }
    ContainersAwaiting->insert({container->getId(),container});
}

Container* Port::removeContainer(const std::string& id){
    auto res = ContainersAwaiting->find(id);
    Container* ans = res->second;
    if(res == ContainersAwaiting->end()){
        std::cout << "Warning: container not exists in the port" << std::endl;
        return nullptr;
    }
    ContainersAwaiting->erase(id);
    return ans;

}

bool Port::hasContainers() const{
    return !ContainersAwaiting->empty();
}

Container* Port::getContainerByID(const std::string& id) const{
    auto it = ContainersAwaiting->find(id);
    if(it!=ContainersAwaiting->end())
        return it->second;
    return nullptr;
}

void Port::getVectorOfContainers(std::vector<Container*>& vector) const{
    for(const auto& elem : *this->ContainersAwaiting )
    {
        vector.push_back(elem.second);
    }
}
void Port::getContainersWithDest(const std::string& dest, std::vector<Container*>& vec) const{
    for(const auto& elem : *(this->ContainersAwaiting) )
    {
        if(elem.second->getDest().compare(dest)==0)
            vec.push_back(elem.second);
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
        if(seenBefore.find(elem.second->getDest())==seenBefore.end()){
            vec.push_back(elem.second);
        }
    }
}

// errors[0] - 2^10, errors[1] - 2^12, errors[2] - 2^13, errors[3] - 2^15
void Port::fixPort(std::vector<bool>& errors, std::vector<Container*> reject) {
    for(int i=0; i<4; i++)
        errors.push_back(false);
    std::set<std::string> IDs;
    for(auto& pair : *this->ContainersAwaiting){
        if(!(pair.second->checkId())){
            reject.push_back(pair.second);
            this->removeContainer(pair.first);
            errors.at(3) = true;
            continue;
        }
        if(pair.second->getWeight() < 0){
            reject.push_back(pair.second);
            this->removeContainer(pair.first);
            errors.at(1) = true;
            continue;
        }
        if(!(pair.second->checkDestination())){
            reject.push_back(pair.second);
            this->removeContainer(pair.first);
            errors.at(2) = true;
            continue;
        }
        if(IDs.find(pair.first) != IDs.end()){
            reject.push_back(pair.second);
            this->removeContainer(pair.first);
            errors.at(0) = true;
            continue;
        }
        IDs.insert(pair.first);

    }


}