//
// Created by itay on 21/03/2020.
//

#include "ShipRoute.h"

ShipRoute::ShipRoute(const std::vector<std::string>* _dstList, std::vector<std::string>* _errors): dstList(new std::vector<std::string>())
{
    errors = _errors;
    for(const auto & i : *_dstList)
    {
        this->addDstAtEnd(i);
    }
}

ShipRoute::ShipRoute(): dstList(new std::vector<std::string>()){}

ShipRoute::~ShipRoute() {
    delete this->dstList;
}

void ShipRoute::addDstAtEnd(const std::string& dst)
{
    if(checkDestination(dst))
        this->dstList->push_back(dst);
    else
        errors->push_back("Warning: " + dst + " is not legal Port ID");
}

void ShipRoute::deleteFirst()
{
    if(!this->dstList->empty()) {
        this->dstList->erase(this->dstList->begin());
    }
}

std::string ShipRoute::getHead() const
{
    if(!this->dstList->empty()) {
        return this->dstList->at(0);
    }
    return "";
}

void ShipRoute::printList() const
{
    for(const auto & i : *this->dstList)
    {
        std::cout << i << std::endl;
    }
}

bool ShipRoute::willVisit(const std::string& dest) const{
    for(const auto& i : *this->dstList)
    {
        if(i==dest){
            return true;
        }
    }
    return false;
}

int ShipRoute::getRouteLength() const{
    return this->dstList->size();
}

std::vector<std::string>* ShipRoute::getDstList(){
    return this->dstList;
}

bool checkDestination(std::string dest){
    if(dest.length() != 5)
        return false;
    for(int i = 0; i < 5; i++) {
        char ch = dest.at(i);
        if (!(ch >= 'A' && ch <= 'Z'))
            return false;
    }
    return true;
}

