//
// Created by zivco on 10/05/2020.
//

#ifndef STOWAGE_MODEL_ALGORITHMREGISTRAR_H
#define STOWAGE_MODEL_ALGORITHMREGISTRAR_H

#include <string>
#include <vector>
#include <list>
#include <functional>
#include "AlgorithmRegistration.h"

class AlgorithmRegistrar {
    std::list<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    void registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
        algorithmFactories.push_back(algorithmFactory);
    }
public:
    friend class AlgorithmRegistration;
    std::list<std::unique_ptr<AbstractAlgorithm>> getAlgorithms()const {
        std::list<std::unique_ptr<AbstractAlgorithm>> algorithms;
        for(auto algorithmFactoryFunc : algorithmFactories) {
            algorithms.push_back(algorithmFactoryFunc());
        }
        return algorithms;
    }
    size_t size()const {
        return algorithmFactories.size();
    }
    static AlgorithmRegistrar& getInstance() {
        static AlgorithmRegistrar instance;
        return instance;
    }
};


#endif //STOWAGE_MODEL_ALGORITHMREGISTRAR_H
