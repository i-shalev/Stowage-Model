
#include "main.h"

int main(int argc, char **argv){
    std::map<std::string, std::string> args;

    if(createArgs(args, argc, argv)){
        // fatal error
        return EXIT_FAILURE;
    }


    printArgs(args);

    auto* dirs = getDirsNamesFromRootDir(args["-travel_path"]);
    for(const auto& dir:*dirs) {
        std::cout << dir << std::endl;
//        writeToFile(pathToRootDir +  resultFileName, "," + dir);
    }
    auto* algoNames = getAlgoNames(args["-algorithm_path"]);
    for(const auto& algoName:*algoNames) {
        std::cout << algoName << std::endl;
//        writeToFile(pathToRootDir +  resultFileName, "," + dir);
    }

    free(algoNames);
    free(dirs);
    return EXIT_SUCCESS;
}

int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv){
    for(int i = 1; i < argc-1; i += 2){
        args[argv[i]] = argv[i+1];
    }

    if(args["-travel_path"].empty()){
        // fatal error
        return EXIT_FAILURE;
    }
    if(args["-algorithm_path"].empty()){
        args["-algorithm_path"] = "./";
    }
    if(args["-output"].empty()){
        args["-output"] = "./";
    }

    return EXIT_SUCCESS;
}

void printArgs(std::map<std::string, std::string>& args){
    std::cout << "travel_path: " << args["-travel_path"] << std::endl;
    std::cout << "algorithm_path: " << args["-algorithm_path"] << std::endl;
    std::cout << "output: " << args["-output"] << std::endl;
}