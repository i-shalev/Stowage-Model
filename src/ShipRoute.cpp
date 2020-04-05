//
// Created by itay on 21/03/2020.
//

#include "ShipRoute.h"

ShipRoute::ShipRoute(const vector<string>* _dstList): dstList(new vector<string>())
{
    for(const auto & i : *_dstList)
    {
        this->addDstAtEnd(i);
    }
}

ShipRoute::~ShipRoute() {
    delete this->dstList;
}

ShipRoute::ShipRoute()= default;

void ShipRoute::addDstAtEnd(const string& dst)
{
    if(checkDestination(dst))
        this->dstList->push_back(dst);
}

void ShipRoute::deleteFirst()
{
    if(!this->dstList->empty())
        this->dstList->erase(this->dstList->begin());
}

string ShipRoute::getHead()
{
    if(!this->dstList->empty()) {
        return this->dstList->at(0);
    }
    return "";
}

void ShipRoute::printList()
{
    for(const auto & i : *this->dstList)
    {
        std::cout << i << std::endl;
    }
}

bool ShipRoute::willVisit(string dest){
    for(const auto& i : *this->dstList)
    {
        if(i==dest){
            return true;
        }
    }
    return false;
}

int ShipRoute::getRouteLength() {
    return this->dstList->size();
}

vector<string>* ShipRoute::getDstList(){
    return this->dstList;
}


bool checkDestination(string dest){
    if(dest.length() != 5)
        return false;
    for(int i = 0; i < 5; i++) {
        char ch = dest.at(i);
        if (!(ch >= 'A' && ch <= 'Z'))
            return false;
    }
    return true;
}

