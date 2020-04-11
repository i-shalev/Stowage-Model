//
// Created by itay on 21/03/2020.
//
#include <iostream>
#ifndef STOWAGE_MODEL_CONTAINER_H
#define STOWAGE_MODEL_CONTAINER_H

using namespace std;
class Container {
    private:
        const string dest;
        const string id;
        int weight;
        bool valid;
        bool blocked;

    public:
        Container(int _weight, string  _dest, string  _id, bool _blocked = false);
//        ~Container();
        int getWeight() const ;
        string getDest() const ;
        string getId() const;
        bool getValid() const;
        bool getBlocked() const;
        void setWeight(const int _weight);
        void printContainer() const;


    public: //only for tests reason, need to be private
        bool checkId();
        bool checkDestination();
};

int letterToInt(char ch);


#endif //STOWAGE_MODEL_CONTAINER_H
