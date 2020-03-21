//
// Created by itay on 21/03/2020.
//
#include <iostream>
#include <vector>

#ifndef STOWAGE_MODEL_SHIPROUTE_H
#define STOWAGE_MODEL_SHIPROUTE_H

using namespace std;
class ShipRoute {
    private:
        vector<string> dstList;

    public:
        explicit ShipRoute(const vector<string>& _dstList);
        ShipRoute();

        void addDstAtEnd(const string& dst);
        void deleteFirst();
        string getHead();
        void printList();

};
bool checkDestination(string dest);


#endif //STOWAGE_MODEL_SHIPROUTE_H
