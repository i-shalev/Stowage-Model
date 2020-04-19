
#include "main.h"

int main(int argc, char **argv){
    if(argc < 2) {
        std::cout << "ERROR: Please pass a root dir" << std::endl;
        return EXIT_FAILURE;
    }
    std::string pathToRootDir = argv[1];
    std::string resultFileName = R"(/simulation.results)";
    std::string errorFileName = R"(/simulation.errors)";
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
    auto* algoRunnerNaiveWithTrick = new AlgoRunner(NaiveAlgoWithTrickEnum, pathToRootDir);
    algoRunnerNaiveWithTrick->startRun();
    delete algoRunnerNaive;
    delete algoRunnerNaiveWithTrick;

    return EXIT_SUCCESS;
}