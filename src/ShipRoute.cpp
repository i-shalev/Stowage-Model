//
// Created by itay on 21/03/2020.
//

#include "ShipRoute.h"

ShipRoute::ShipRoute(vector<string> _dstList)
{
    this->dstList = _dstList;
    for( int i = 0; i <this->dstList.size(); i++)
    {
        if(checkDestination(this->dstList.at(i)))
            this->dstList.erase(this->dstList.begin()+i);
    }
}

ShipRoute::ShipRoute(){}

void ShipRoute::addDstAtEnd(string dst)
{
    if(checkDestination(dst))
        this->dstList.push_back(dst);
}

void ShipRoute::deleteFirst()
{
    if(this->dstList.size() > 0)
        this->dstList.erase(this->dstList.begin());
}

string ShipRoute::getHead()
{
    if(this->dstList.size() > 0)
        this->dstList.at(0);
}

void ShipRoute::printList()
{
    for( int i = 0; i <this->dstList.size(); i++)
    {
        std::cout << this->dstList.at(i) << std::endl;
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