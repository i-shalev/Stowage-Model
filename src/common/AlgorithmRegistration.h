//
// Created by zivco on 08/05/2020.
//

#ifndef STOWAGE_MODEL_ALGORITHMREGISTRATION_H
#define STOWAGE_MODEL_ALGORITHMREGISTRATION_H


// AlgorithmRegistration.h

#pragma once

#include <functional>
#include <memory>
#include "AbstractAlgorithm.h"

class AlgorithmRegistration {
public:
    AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()>);
};

#define REGISTER_ALGORITHM(class_name) \
AlgorithmRegistration register_me_##class_name \
	([]{return std::make_unique<class_name>();} );



#endif //STOWAGE_MODEL_ALGORITHMREGISTRATION_H
