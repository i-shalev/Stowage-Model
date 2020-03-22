#include <vector>
#include "Container.h"

#ifndef STOWAGE_MODEL_FLOOR_H
#define STOWAGE_MODEL_FLOOR_H

#endif //STOWAGE_MODEL_FLOOR_H

using namespace std;
class Floor{
    private:
        int index;
        int length;
        int width;
        Container*** floorMap = nullptr;

    public:
        Floor(int index, int length, int width);
        ~Floor();
        Floor(const Floor& f) = delete; // disable copy constr'. need deep copy
        Container* getContainerAtPosition(int i, int j);
        void setContainerAtPosition(int i, int j, Container* value);


};