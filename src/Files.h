//
// Created by itay on 29/03/2020.
//
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include "Container.h"

#ifndef STOWAGE_MODEL_FILES_H
#define STOWAGE_MODEL_FILES_H

using namespace std;

class Files {

};

bool getSizesShipPlan(const string &path, int &numFloors, int &length, int &width, int &numLines);
bool readShipPlan(vector<vector<int>>& blocks, const string& path);
bool getNumberOfNonEmtpyLines(const string &path, int &numLines);
bool readShipPorts(vector<string>& ports, const string& path);
bool readPortContainers(vector<Container*>& containers, const string& path);

#endif //STOWAGE_MODEL_FILES_H
