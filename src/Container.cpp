#include "Container.h"

Container::Container(int _weight, const string _dest, const string _id): dest(_dest), id(_id), weight(_weight){}

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

bool Container::checkId(string id)
{
    int result = 0;
    int mul = 1;
    int digitCheck;
    char ch;
    if(id.length() != 11)
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
    std::cout << digitCheck << std::endl;
    return digitCheck == (id.at(10) - '0');
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
