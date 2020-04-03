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

void readShipPlan(vector<vector<vector<int>>>& allBlocks, const string& path);
void getSizesShipPlan(const string& path, int& numFloors, int& length, int& width);

#endif //STOWAGE_MODEL_FILES_H
