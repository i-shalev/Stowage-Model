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
void createMapOfPortAndNumberOfVisits(vector<string> *portList, map<string, int> *mapPortVisits);

#endif //STOWAGE_MODEL_MAIN_H
