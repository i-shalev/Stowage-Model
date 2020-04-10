
#include "main.h"

int main(){
    string pathToRootDir = R"(C:\Users\itay\Desktop\ex)";
    string resultFileName = R"(/simulation.results)";
    emptyFile(pathToRootDir +  resultFileName);
    auto* dirs = getDirsNamesFromRootDir(pathToRootDir);
    for(const auto& dir:*dirs) {
        writeToFile(pathToRootDir +  resultFileName, "," + dir);
    }
    writeToFile(pathToRootDir +  resultFileName, ", Sum \n");
    auto* algoRunner = new AlgoRunner(NaiveAlgoEnum, pathToRootDir);
    algoRunner->startRun();
    delete algoRunner;
}