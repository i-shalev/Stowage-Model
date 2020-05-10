//
// Created by zivco on 10/05/2020.
//

#include "../Common/AlgorithmRegistration.h"
#include "../Common/AlgorithmRegistrar.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory);
}