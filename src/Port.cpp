//
// Created by zivco on 10/04/2020.
//

#include "Port.h"

Port::Port(std::string  symbol, int number, vector<string>* _errors): symbol(std::move(symbol)), number(number){
    this->errors = _errors;
    ContainersAwaiting = new map<std::string, Container*>;
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
        errors->push_back("Warning: the port already have this container");
//            std::cout << "Warning: the port already have this container" << std::endl;
        return;
    }
    ContainersAwaiting->insert({container->getId(),container});
}

Container* Port::removeContainer(const std::string& id){
    auto res = ContainersAwaiting->find(id);
    Container* ans = res->second;
    if(res == ContainersAwaiting->end()){
        errors->push_back("Warning: container not exists in the port");
//            std::cout << "Warning: container not exists in the port" << std::endl;
        return nullptr;
    }
    ContainersAwaiting->erase(id);
    return ans;

}

bool Port::hasContainers() {
    return !ContainersAwaiting->empty();
}

Container* Port::getContainerByID(const string& id){
    auto it = ContainersAwaiting->find(id);
    if(it!=ContainersAwaiting->end())
        return it->second;
    return nullptr;
}

void Port::getVectorOfContainers(vector<Container*>& vector) {
    for(const auto& elem : *this->ContainersAwaiting )
    {
        vector.push_back(elem.second);
    }
}
void Port::getContainersWithDest(const string& dest, vector<Container*>& vec){
    for(const auto& elem : *(this->ContainersAwaiting) )
    {
        if(elem.second->getDest().compare(dest)==0)
            vec.push_back(elem.second);
    }
}
void Port::getContainersByDistance(ShipRoute& sr, vector<Container*>& vec){
    set<string> seenBefore;
    for(auto elem : *(sr.getDstList())){
        if(seenBefore.find(elem) == seenBefore.end()) {
            getContainersWithDest(elem, vec);
            seenBefore.insert(elem);
        }
    }
}