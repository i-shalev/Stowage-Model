//
// Created by itay on 21/03/2020.
//
#include <iostream>
#include <vector>

#ifndef STOWAGE_MODEL_SHIPROUTE_H
#define STOWAGE_MODEL_SHIPROUTE_H

//using namespace std;
class ShipRoute {
    private:
        std::vector<std::string>* dstList;
        std::vector<std::string>* errors;

    public:
        explicit ShipRoute(const std::vector<std::string> *_dstList);
        ShipRoute();
        ShipRoute(const ShipRoute &route);
        ~ShipRoute();
        void addDstAtEnd(const std::string& dst);
        void deleteFirst();
        std::string getHead() const;
        void printList() const;
        bool willVisit(const std::string& dest) const;
        int getRouteLength() const;
        std::vector<std::string>* getDstList();
};
bool checkDestination(std::string dest);


#endif //STOWAGE_MODEL_SHIPROUTE_H
