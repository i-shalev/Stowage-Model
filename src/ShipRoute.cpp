//
// Created by itay on 21/03/2020.
//

#include "ShipRoute.h"

#include <utility>

ShipRoute::ShipRoute(vector<string> _dstList)
{
    this->dstList = std::move(_dstList);
    for( int i = 0; i <this->dstList.size(); i++)
    {
        if(checkDestination(this->dstList.at(i)))
            this->dstList.erase(this->dstList.begin()+i);
    }
}

ShipRoute::ShipRoute()= default;

void ShipRoute::addDstAtEnd(string dst)
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
}

void ShipRoute::printList()
{
    for(const auto & i : this->dstList)
    {
        std::cout << i << std::endl;
    }
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