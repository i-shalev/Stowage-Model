
#include "main.h"

int main(){
    string pathToRootDir = R"(C:\Users\zivco\Desktop\ex)";
    string resultFileName = R"(/simulation.results)";
    string errorFileName = R"(/simulation.errors)";
    emptyFile(pathToRootDir +  resultFileName);
    emptyFile(pathToRootDir +  errorFileName);
    auto* dirs = getDirsNamesFromRootDir(pathToRootDir);
    for(const auto& dir:*dirs) {
        writeToFile(pathToRootDir +  resultFileName, "," + dir);
        writeToFile(pathToRootDir +  errorFileName, "," + dir);
    }
    writeToFile(pathToRootDir +  resultFileName, ", Sum \n");
    writeToFile(pathToRootDir +  errorFileName, "\n");
    auto* algoRunnerNaive = new AlgoRunner(NaiveAlgoEnum, pathToRootDir);
    algoRunnerNaive->startRun();
    auto* algoRunnerNaive1 = new AlgoRunner(NaiveAlgoEnum, pathToRootDir);
    algoRunnerNaive1->startRun();
    delete algoRunnerNaive;
    delete algoRunnerNaive1;
}