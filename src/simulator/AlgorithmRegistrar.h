//
// Created by zivco on 10/05/2020.
//

#ifndef STOWAGE_MODEL_ALGORITHMREGISTRAR_H
#define STOWAGE_MODEL_ALGORITHMREGISTRAR_H

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include "../common/AlgorithmRegistration.h"

class AlgorithmRegistrar {

    struct DlCloser
    {
        void operator()(void *dlhandle) const noexcept;
    };

    // Algorithm factories signature type
    typedef std::function<std::unique_ptr<AbstractAlgorithm>()> AlgorithmFactory;
    typedef std::unique_ptr<void, DlCloser> DlHandler;

    // Dynamic-linker handles map (to keep algorithms code in memory)
    std::map<std::string, DlHandler> _handles;

    // Algorithm factories
    std::vector<AlgorithmFactory> _factories;

    // AlgorithmRegistration's internal interface to support register-on-load policy
    inline void registerFactory(AlgorithmFactory factory) { _factories.push_back(factory); }

public:
    friend class AlgorithmRegistration;

    // iteration over factories
    typedef decltype(_factories)::const_iterator const_iterator;

    // Default Constructor
    AlgorithmRegistrar() = default;
    ~AlgorithmRegistrar();

    // Prevent copy - this is singelton
    AlgorithmRegistrar(const AlgorithmRegistrar&) = delete;
    AlgorithmRegistrar& operator=(const AlgorithmRegistrar&) = delete;

    size_t size()const {
        return _factories.size();
    }

    static AlgorithmRegistrar& getInstance() {
        static AlgorithmRegistrar instance;
        return instance;
    }

    bool loadAlgorithmFromFile(const char *file_path, std::string& error);

    // Rreturns an iterator to the first algorithm factory
    inline const_iterator begin() const { return _factories.begin(); }

    // Returns an iterator that points to the end
    inline const_iterator end() const { return _factories.end(); }

    const_iterator at(int i) {
        int j = 0;
        for (auto algo_iter = begin(); algo_iter != end(); ++algo_iter) {
            if( j == i)
                return algo_iter;
            j++;
        }
        return begin();
    }
};


#endif //STOWAGE_MODEL_ALGORITHMREGISTRAR_H
