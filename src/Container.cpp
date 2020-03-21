//
// Created by itay on 21/03/2020.
//

#include "Container.h"

Container::Container(int weight, string dest, string id)
{
    this->weight    = weight;
    this->dest      = dest;
    this->id        = id;
}

int Container::getWeight()
{
    return this->weight;
}

string Container::getDest()
{
    return this->dest;
}

string Container::getId()
{
    return this->id;
}

void Container::setWeight(int weight)
{
    this->weight = weight;
}

void Container::setId(string id)
{
    this->id = id;
}

void Container::setDest(string dest)
{
    this->dest = dest;
}