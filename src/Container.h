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
        string dest;
        string id;

    public:
        Container(int weight, string dest, string id);

        int getWeight();

        string getDest();

        string getId();

        void setWeight(int weight);

        void setId(string id);
        void setDest(string dest);
};


#endif //STOWAGE_MODEL_CONTAINER_H
