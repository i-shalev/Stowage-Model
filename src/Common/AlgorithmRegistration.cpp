//
// Created by zivco on 10/05/2020.
//

#include "AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory);
}