#include <vector>
#include "Container.h"

#ifndef STOWAGE_MODEL_FLOOR_H
#define STOWAGE_MODEL_FLOOR_H

//using namespace std;
class Floor{
    private:
        int index;
        int length;
        int width;
        std::vector<std::vector<Container*>>* floorMap;

    public:
        Floor(int index, int length, int width);
        ~Floor();
        Floor(const Floor& f) = delete; // disable copy constr'. need deep copy
        Container* getContainerAtPosition(int i, int j) const;
        void setContainerAtPosition(int i, int j, Container* value);


};
#endif //STOWAGE_MODEL_FLOOR_H
