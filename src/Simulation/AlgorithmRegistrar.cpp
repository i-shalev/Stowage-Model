//
// Created by itay on 15/05/2020.
//

#include "../Common/AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"
#include <dlfcn.h>

//void AlgorithmRegistrar::registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
//    _factories.push_back(algorithmFactory);
//}

AlgorithmRegistrar::~AlgorithmRegistrar(){
    // Remove all factories - before closing all handles!
    _factories.clear();
    _handles.clear();
}

void AlgorithmRegistrar::DlCloser::operator()(void *dlhandle) const noexcept
{
//    std::cout << "Closing..." << std::endl;
    dlclose(dlhandle);
//        (void)dlhandle;
//    std::cout << "Finished Closing..." << std::endl;
}

bool AlgorithmRegistrar::loadAlgorithmFromFile(const char *file_path, std::string& error)
{
    size_t lastLength = size();
    DlHandler algo_handle(dlopen(file_path, RTLD_LAZY));
    if(!algo_handle){
        const char *dlopen_error = dlerror();
        error = dlopen_error ? dlopen_error : "ERROR : can't load algorithm!";
        return false;
    } else {
        if(lastLength == size()){
            std::vector<std::string> errors;
            error = "ERROR : the algorithm didn't registered!";
            return false;
        } else {
            _handles[file_path] = std::move(algo_handle);
            return true;
        }
    }
}