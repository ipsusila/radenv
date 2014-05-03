#ifndef STOREDFORAGE_H
#define STOREDFORAGE_H

#include "foodchain.h"

class StoredForage : public FoodChain
{
public:
    enum {SerialId = 16};

    StoredForage(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // STOREDFORAGE_H
