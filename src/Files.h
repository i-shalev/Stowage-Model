//
// Created by itay on 29/03/2020.
//
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include "Container.h"
#include "Port.h"

#ifndef STOWAGE_MODEL_FILES_H
#define STOWAGE_MODEL_FILES_H

using namespace std;

class Files {

};

bool getSizesShipPlan(const string &path, int &numFloors, int &length, int &width, int &numLines);
bool readShipPlan(vector<vector<int>>& blocks, const string& path);
bool getNumberOfNonEmtpyLines(const string &path, int &numLines);
bool readPortContainers(Port*& port, const string& path);
bool readShipPorts(vector<string>& ports, const string& path);
string removeLeadingAndTrailingWhitespaces(string line);
bool isCommentLine(string line);
void getCargoData(const char *path, vector<string>& res);
void stringToCharStar(char* chatStar, string str);
bool isFile(const char* path);

#endif //STOWAGE_MODEL_FILES_H
