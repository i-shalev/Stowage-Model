//
// Created by itay on 21/03/2020.
//

#include "ShipRoute.h"

ShipRoute::ShipRoute(const vector<string>& _dstList)
{
    for(const auto & i : _dstList)
    {
        this->addDstAtEnd(i);
    }
}

ShipRoute::ShipRoute()= default;

void ShipRoute::addDstAtEnd(const string& dst)
{
    if(checkDestination(dst))
        this->dstList.push_back(dst);
}

void ShipRoute::deleteFirst()
{
    if(!this->dstList.empty())
        this->dstList.erase(this->dstList.begin());
}

string ShipRoute::getHead()
{
    if(!this->dstList.empty())
        this->dstList.at(0);
    return "";
}

void ShipRoute::printList()
{
    for(const auto & i : this->dstList)
    {
        std::cout << i << std::endl;
    }
}

bool ShipRoute::willVisit(string dest){
    for(const auto i : this->dstList)
    {
        if(i.compare(dest)==0){return true;}
    }
    return false;
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