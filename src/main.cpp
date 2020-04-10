
#include "main.h"

int main(){
    string pathToRootDir = R"(C:\Users\zivco\Desktop\ex)";
    string resultFileName = R"(/simulation.results)";
    emptyFile(pathToRootDir +  resultFileName);
    auto* dirs = getDirsNamesFromRootDir(pathToRootDir);
    for(const auto& dir:*dirs) {
        writeToFile(pathToRootDir +  resultFileName, "," + dir);
    }
    writeToFile(pathToRootDir +  resultFileName, ", Sum \n");
    auto* algoRunnerNaive = new AlgoRunner(NaiveAlgoEnum, pathToRootDir);
    algoRunnerNaive->startRun();
    delete algoRunnerNaive;
}