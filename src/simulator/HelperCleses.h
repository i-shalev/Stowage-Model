//
// Created by zivco on 29/05/2020.
//

#ifndef STOWAGE_MODEL_HELPERCLESES_H
#define STOWAGE_MODEL_HELPERCLESES_H

// helper classes for named arguments
template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};

class NumTasks: public Named<int> {
    using Named<int>::Named;
};

class IterationsPerTask: public Named<int> {
    using Named<int>::Named;
};

class NumThreads: public Named<int> {
    using Named<int>::Named;
};

#endif //STOWAGE_MODEL_HELPERCLESES_H
