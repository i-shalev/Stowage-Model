
#include "main.h"

std::string current_working_directory()
{
    char* cwd = _getcwd( 0, 0 ) ; // **** microsoft specific ****
    std::string working_directory(cwd) ;
    std::free(cwd) ;
    return working_directory ;
}

int main(int argc, char **argv){
    std::map<std::string, std::string> args;
    args["-travel_path"];
    args["-algorithm_path"];
    args["-output"];

    for(int i = 1; i < argc-1; i += 2){
        args[argv[i]] = argv[i+1];
    }

    if(args["-travel_path"].empty()){
        // fatal error
        return EXIT_FAILURE;
    }
    if(args["-algorithm_path"].empty()){
        args["-algorithm_path"] = ".\\";
    }
    if(args["-output"].empty()){
        args["-output"] = "";
    }
    std::cout << current_working_directory() << std::endl;
    std::ifstream fin;
    try{
        fin.open(  R"(./ShipPlan.csv)", std::ios::in);
        if(!fin.is_open()){
            throw std::exception();
        }
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
    }

    std::cout << "travel_path: " << args["-travel_path"] << std::endl;
    std::cout << "algorithm_path: " << args["-algorithm_path"] << std::endl;
    std::cout << "output: " << args["-output"] << std::endl;



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
//    auto* algoRunnerNaiveWithTrick = new AlgoRunner(NaiveAlgoWithTrickEnum, pathToRootDir);
//    algoRunnerNaiveWithTrick->startRun();
//    delete algoRunnerNaive;
//    delete algoRunnerNaiveWithTrick;


// tests for Files:

//    std::string pathToRootDir = R"(C:\Users\itay\Desktop\input\example1\Algo 2 better than 1)";
//
//    std::vector<std::string>* errors = {};
//    auto* blocks = new std::vector<std::vector<int>>(10-1);;
//    int num = 0;
//    bool results = getSizesShipPlan(pathToRootDir + "\\ShipPort.csv", num, num, num, num, errors);
//    auto results = readShipPlan(*blocks, pathToRootDir + "\\ShipPort.csv", errors);
//    std::cout << results << std::endl;
//    std::cout << "out: " << results << std::endl;
//    std::cout << results->at(0) << " " << results->at(1) << std::endl;
//
//
//    auto* ports = new std::vector<std::string>();
//    std::vector<bool>*  results = readShipPorts(*ports, pathToRootDir + "\\Ports.csv", errors);
//    std::cout << results->at(0) << " " << results->at(1) << " " << results->at(2) << " " << results->at(3) << std::endl;
//
//    Port *port = new Port("AAAAA", 0, errors);
//
//
//    std::vector<bool>*  results = readPortContainers(port, pathToRootDir + R"(\\AAAAA_0.cargo_data)", errors );
//    std::cout << results->at(0) << " " << results->at(1) << " " << results->at(2) << " " << results->at(3) << " " << results->at(4) << std::endl;

    return EXIT_SUCCESS;
}

void setPaths(std::map<std::string, std::string> args, std::string argName, std::string arg){
    std::cout << "argName: " << argName << std::endl;
    std::cout << "arg: " << arg << std::endl;
}