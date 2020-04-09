#include "Container.h"

#include <utility>

Container::Container(int _weight, string  _dest, string  _id, bool _blocked): dest(std::move(_dest)), id(std::move(_id)), weight(_weight), blocked(_blocked)
{
    this->valid = checkId() && checkDestination();
}

//Container::~Container() {
////    std::cout << "Delete Container: " << this->id << std::endl;
//}

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

bool Container::getValid()
{
    return this->valid;
}

void Container::setWeight(int _weight)
{
    this->weight = _weight;
}

bool Container::checkId()
{
    int result = 0;
    int mul = 1;
    int digitCheck;
    char ch;
    if(id.length() != 11)
        return false;
    if(id.at(3)!='U' && id.at(3)!='J' && id.at(3)!='Z')
        return false;
    for(int i = 0; i < 4; i++)
    {
        ch = id.at(i);
        if(!(ch >= 'A' && ch <= 'Z'))
            return false;
        result += mul * letterToInt(ch);
        mul *= 2;
    }
    for(int i = 4; i < 10; i++)
    {
        ch = id.at(i);
        if(!(ch >= '0' && ch <= '9'))
            return false;
        result += mul * (ch - '0');
        mul *= 2;
    }
    digitCheck = result % 11;
    if(digitCheck == 10)
        digitCheck = 0;
    return digitCheck == (id.at(10) - '0');
}

bool Container::checkDestination(){
    if(dest.length() != 5)
        return false;
    for(int i = 0; i < 5; i++) {
        char ch = dest.at(i);
        if (!(ch >= 'A' && ch <= 'Z'))
            return false;
    }
    return true;
}

void Container::printContainer() const {
    std::cout << "ID: " << this->id << ", " << "dest: " << this->dest << ", " <<"weight: " << this->weight << ", " << "blocked: " << this->blocked <<  endl;
}

int letterToInt(char ch)
{
    if(ch == 'A')
        return 10;
    if(ch >= 'B' && ch <= 'K')
        return 11 + (ch - 'A');
    if(ch >= 'L' && ch <= 'U')
        return 12 + (ch - 'A');
    if(ch >= 'V' && ch <= 'Z')
        return 13 + (ch - 'A');
    return 0;
}