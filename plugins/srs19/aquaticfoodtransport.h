#ifndef AQUATICFOODTRANSPORT_H
#define AQUATICFOODTRANSPORT_H

#include "foodchain.h"

class AquaticFoodTransport : public FoodChain
{
public:
    enum {SerialId = 20};

    AquaticFoodTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // AQUATICFOODTRANSPORT_H
