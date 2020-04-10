//
// Created by itay on 29/03/2020.
//
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include "Container.h"
#include "Port.h"

#ifndef STOWAGE_MODEL_FILES_H
#define STOWAGE_MODEL_FILES_H

using namespace std;

class Files {

};

bool getSizesShipPlan(const string &path, int &numFloors, int &length, int &width, int &numLines, vector<string>* errors);
bool readShipPlan(vector<vector<int>> &blocks, const string &path, vector<string> *errors);

bool readPortContainers(Port *&port, const string &path, vector<string> *errors);
bool readShipPorts(vector<string> &ports, const string &path, vector<string> *errors);
string removeLeadingAndTrailingWhitespaces(string line);
bool isCommentLine(string line);
void getCargoData(const char *path, vector<string>& res);
void stringToCharStar(char* chatStar, string str);
bool isFile(const char* path);
int isDirectory(const char *path);
vector<string>* getDirsFromRootDir(const string &pathToDir);
void writeToFile(const string& filename, const string& data);
void emptyFile(const string& filename);
vector<string>* getDirsNamesFromRootDir(const string &pathToDir);

#endif //STOWAGE_MODEL_FILES_H
