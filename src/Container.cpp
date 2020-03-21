#include "Container.h"

Container::Container(int _weight, string _dest, string _id): dest(_dest), id(_id), weight(_weight){}

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
