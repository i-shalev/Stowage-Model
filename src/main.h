//
// Created by itay on 04/04/2020.
//

#ifndef STOWAGE_MODEL_MAIN_H
#define STOWAGE_MODEL_MAIN_H

using namespace std;
class main {

};

int main();
int simulate(const string &pathToDir);
bool handleNameOfFile (const string& fileName, string& portName, int & indexNumber);
map<string, int>* createMapOfPortAndNumberOfVisits(vector<string>* portList);
void findMissingPortFiles(map<string, int> *mapPortVisits, vector<string> *portVector, const string &path);
ShipPlan* createShipPlan(const string& pathToShipPlan);
ShipRoute *createShipRoute(const string &pathToShipPorts);
Ship* createShip(const string &pathToDir);

#endif //STOWAGE_MODEL_MAIN_H
