//
// Created by itay on 04/04/2020.
//

#ifndef STOWAGE_MODEL_MAIN_H
#define STOWAGE_MODEL_MAIN_H
#include <iostream>
#include "common/Files.h"
#include "common/Port.h"
#include "common/Crane.h"
#include "AlgoRunner.h"

//using namespace std;
class main {

};

int main(int argc, char **argv);
int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv);
void printArgs(std::map<std::string, std::string>& args);
#endif //STOWAGE_MODEL_MAIN_H
