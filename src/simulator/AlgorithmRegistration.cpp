//
// Created by zivco on 10/05/2020.
//

#include "../common/AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerFactory(algorithmFactory);
}