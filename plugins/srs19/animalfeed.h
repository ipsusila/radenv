#ifndef ANIMALFEED_H
#define ANIMALFEED_H

#include "foodchain.h"

class AnimalFeed : public FoodChain
{
public:
    enum {SerialId = 17};

    AnimalFeed(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // ANIMALFEED_H
