
#include "main.h"

int main(int argc, char **argv){
    if(argc < 2) {
        std::cout << "ERROR: Please pass a root dir" << std::endl;
        return EXIT_FAILURE;
    }
//    std::cout << 1 << std::endl;
//    std::string pathToRootDir = argv[1];
//    std::string resultFileName = R"(/simulation.results)";
//    std::string errorFileName = R"(/simulation.errors)";
//    emptyFile(pathToRootDir +  resultFileName);
//    emptyFile(pathToRootDir +  errorFileName);
//    auto* dirs = getDirsNamesFromRootDir(pathToRootDir);
//    for(const auto& dir:*dirs) {
//        writeToFile(pathToRootDir +  resultFileName, "," + dir);
//        writeToFile(pathToRootDir +  errorFileName, "," + dir);
//    }
//    writeToFile(pathToRootDir +  resultFileName, ", Sum \n");
//    writeToFile(pathToRootDir +  errorFileName, "\n");
//    auto* algoRunnerNaive = new AlgoRunner(NaiveAlgoEnum, pathToRootDir);
//    algoRunnerNaive->startRun();
//
//    delete algoRunnerNaive;
    int x =  turnToTrueBit(0, 0);
    std::cout << x << std::endl;
    x =  turnToTrueBit(x, 1);
    x =  turnToTrueBit(x, 2);
    x =  turnToTrueBit(x, 3);
    x =  turnToTrueBit(x, 4);
    x =  turnToTrueBit(x, 5);
    x =  turnToTrueBit(x, 6);
    x =  turnToTrueBit(x, 7);
    x =  turnToTrueBit(x, 8);
    x =  turnToTrueBit(x, 9);
    x =  turnToTrueBit(x, 10);
    x =  turnToTrueBit(x, 11);
    x =  turnToTrueBit(x, 12);
    x =  turnToTrueBit(x, 13);
    x =  turnToTrueBit(x, 14);
    x =  turnToTrueBit(x, 15);
    x =  turnToTrueBit(x, 16);
    x =  turnToTrueBit(x, 17);
    x =  turnToTrueBit(x, 18);
    std::cout << x << std::endl;

    return EXIT_SUCCESS;
}