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

bool getSizesShipPlan(const string &path, int &numFloors, int &length, int &width, int &numLines);
bool readShipPlan(vector<vector<int>>& blocks, const string& path);
void readShipPorts(vector<string>& ports, const string& path, int numFloors);


#endif //STOWAGE_MODEL_FILES_H
