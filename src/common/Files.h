//
// Created by itay on 29/03/2020.
//
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include <cstring>
#include <dirent.h>
#include <memory>
#include <sys/stat.h>
#include "Container.h"
#include "Port.h"

#ifndef STOWAGE_MODEL_FILES_H
#define STOWAGE_MODEL_FILES_H

//using namespace std;

class Files {

};

// read the file and fill the data to the args.
bool getSizesShipPlan(const std::string &path, int &numFloors, int &length, int &width, int &numLines);

std::unique_ptr<std::vector<bool>> readShipPlanInFiles(std::vector<std::vector<int>> &blocks, const std::string &path,
        int numFloors, int length, int width);

bool isLegalPortName(std::string portName);

std::unique_ptr<std::vector<bool>> readPortContainers(Port *port, const std::string &path);

std::unique_ptr<std::vector<bool>> readShipPorts(std::vector<std::string> &ports, const std::string &path);

int getWeightIfLegal(std::string weight);

bool isLegalId(std::string id);

std::string removeLeadingAndTrailingWhitespaces(std::string line);

bool isCommentLine(std::string line);

void getCargoData(const char *path, std::vector<std::string>& res);
void stringToCharStar(char* chatStar, std::string str);
bool isFile(const char* path);
int isDirectory(const char *path);
//std::vector<std::string>* getDirsFromRootDir(const std::string &pathToDir);
void writeToFile(const std::string& filename, const std::string& data);
void
writeErrorsToFile(const std::string &filename, const std::string &folderPath, const std::vector<std::string> *errors);
void writeToSuccessFile(const std::string &filename, const std::vector<std::string>* results);
void emptyFile(const std::string& filename);
std::unique_ptr<std::vector<std::string>> getDirsNamesFromRootDir(const std::string &pathToDir);
std::unique_ptr<std::vector<std::string>> getFileNamesEndWith(const std::string &pathToDir, const std::string &endsWith);

#endif //STOWAGE_MODEL_FILES_H
