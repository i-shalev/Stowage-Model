
#include "main.h"

int main(int argc, char **argv){
    std::map<std::string, std::string> args;

    if(createArgs(args, argc, argv)){
        // fatal error
        return EXIT_FAILURE;
    }


//    printArgs(args);

    auto* dirs = getDirsNamesFromRootDir(args["-travel_path"]);
    for(const auto& dir:*dirs) {
        std::string shipPlanPath, shipRoutePath;
        auto* res = getFileNamesEndWith(args["-travel_path"] + "/" + dir, ".ship_plan");
        if(!res->empty()){
            shipPlanPath = res->at(0);
        } else {
            // ERROR : no shipPlan file!
            std::cout << "ERROR : no shipPlan file!" << std::endl;
        }
        delete(res);
        res = getFileNamesEndWith(args["-travel_path"] + "/" + dir, ".route");
        if(!res->empty()){
            shipRoutePath = res->at(0);
        } else {
            // ERROR : no shipRoute file!
            std::cout << "ERROR : no shipRoute file!" << std::endl;
        }
        delete(res);
    }

//    auto* algoNames = getFileNamesEndWith(args["-algorithm_path"], ".so");
//    for(const auto& algoName:*algoNames) {
//        std::cout << algoName << std::endl;
////        writeToFile(pathToRootDir +  resultFileName, "," + dir);
//    }

//    delete(algoNames);
    delete(dirs);
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