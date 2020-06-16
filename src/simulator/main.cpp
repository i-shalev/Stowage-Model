#include <algorithm>
#include "main.h"


#define PATH_TO_EMPTY_FILE R"(.\empty.empty_file)"

int main(int argc, char **argv){
    std::map<std::string, std::string> args;
    std::vector<std::string> errors;
    int numThreads;
    bool errorInCreateArgs = false;
    if(createArgs(args, argc, argv)){
        errorInCreateArgs = true;
        errors.emplace_back("ERROR: travel_path not provided!");
    }
//    printArgs(args);
    try{
        numThreads = stoi(args["-num_threads"]);
    } catch (const std::exception& e) {
        numThreads = 1;
        errors.emplace_back("ERROR: num_threads that provided is not a number!");
    }
    int res = folderIsExistOrCanBeBuilt(args["-output"]);
    if(res == 1){
        errors.emplace_back("Warning: output path that provided is non existent folder but we successfully create it.");
    } else if (res == 2) {
        errors.emplace_back("Warning: output path that provided is not a valid path, the output files will be under the folder you run the program.");
        args["-output"] = "./";
    }
    if(!errorInCreateArgs and !isFolderExist(args["-travel_path"])){
        errorInCreateArgs = true;
        errors.emplace_back("ERROR: travel_path that provided is not a valid path.");
    }
    if(errorInCreateArgs){
        writeErrorsToFile(args["-output"] + "/errors/" + "general_errors.errors", args["-output"] + "/errors/", &errors);
        return EXIT_FAILURE;
    }
    if(!isFolderExist(args["-algorithm_path"])){
        errors.emplace_back("Warning: algorithm_path that provided is not a valid path. so no algorithms runs.");
    } else {
      if(numThreads == 1) {
        runAllAlgo(args["-algorithm_path"], args["-travel_path"], args["-output"]);
      } else {
        // here we call the threads function.
        runThreads(numThreads, args["-algorithm_path"], args["-travel_path"], args["-output"]);
      }
    }
    writeErrorsToFile(args["-output"] + "/errors/" + "general_errors.errors", args["-output"] + "/errors/", &errors);
    return EXIT_SUCCESS;
}

int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv){
    for(int i = 1; i < argc-1; i += 2){
        args[argv[i]] = argv[i+1];
    }
    if(args["-algorithm_path"].empty()){
        args["-algorithm_path"] = "./";
    }
    if(args["-num_threads"].empty()){
        args["-num_threads"] = "1";
    }
    if(args["-output"].empty()){
        args["-output"] = "./";
    }
    if(args["-travel_path"].empty()){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void printArgs(std::map<std::string, std::string>& args){
    std::cout << "travel_path: " << args["-travel_path"] << std::endl;
    std::cout << "num_threads: " << args["-num_threads"] << std::endl;
    std::cout << "algorithm_path: " << args["-algorithm_path"] << std::endl;
    std::cout << "output: " << args["-output"] << std::endl;
}



