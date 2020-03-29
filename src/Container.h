//
// Created by itay on 21/03/2020.
//
#include <iostream>
#ifndef STOWAGE_MODEL_CONTAINER_H
#define STOWAGE_MODEL_CONTAINER_H

using namespace std;
class Container {
    private:
        int weight;
        const string dest;
        const string id;
        bool valid;
        bool blocked;

    public:
        Container(int _weight, const string _dest, const string _id, bool _blocked = false);

        int getWeight();
        string getDest();
        string getId();
        bool getValid();
        bool getBlocked(){return this->blocked;}
        void setWeight(int weight);


    public: //only for tests reason, need to be private
        bool checkId();
        bool checkDestination();
};

int letterToInt(char ch);


#endif //STOWAGE_MODEL_CONTAINER_H
