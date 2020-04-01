//
// Created by itay on 29/03/2020.
//
#include <vector>
#include <iostream>

#ifndef STOWAGE_MODEL_FILES_H
#define STOWAGE_MODEL_FILES_H

using namespace std;

class Files {

};

vector<vector<vector<int>>> readShipPlan(const string& path, int& numFloors, int& length, int& width);

#endif //STOWAGE_MODEL_FILES_H
