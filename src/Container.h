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

    public:
        Container(int _weight, const string _dest, const string _id);

        int getWeight();

        string getDest();

        string getId();

        void setWeight(int weight);

        void setId(string id);
        void setDest(string dest);

        bool checkId(string id);
};

int letterToInt(char ch);


#endif //STOWAGE_MODEL_CONTAINER_H
