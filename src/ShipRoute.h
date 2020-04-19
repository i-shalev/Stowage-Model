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
        vector<string>* dstList;
        vector<string>* errors;

    public:
        explicit ShipRoute(const vector<string>* _dstList, vector<string>* _errors);
        ShipRoute();
        ~ShipRoute();
        void addDstAtEnd(const string& dst);
        void deleteFirst();
        string getHead() const;
        void printList() const;
        bool willVisit(const string& dest) const;
        int getRouteLength() const;
        vector<string>* getDstList();
};
bool checkDestination(string dest);


#endif //STOWAGE_MODEL_SHIPROUTE_H
