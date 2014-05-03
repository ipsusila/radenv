#ifndef AQUATICFOODTRANSPORT_H
#define AQUATICFOODTRANSPORT_H

#include "foodchain.h"
#include "defaultvalues.h"

class AquaticFoodTransport : public FoodChain
{
public:
    enum {FreshwaterFish = 1, MarineFish = 2, MarineShellfish = 3};

    bool verify(int * err = 0, int * warn = 0);
protected:
    BioAccumulationFactor bpValues;

    AquaticFoodTransport(IModelFactory * fact, const KModelInfo& inf);

    virtual int fishType() const = 0;
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

class FreshwaterFishTransport : public AquaticFoodTransport
{
public:
    enum { SerialId = 24 };

    FreshwaterFishTransport(IModelFactory * fact, const KModelInfo& inf);

protected:
    int fishType() const;
};

class MarineFishTransport : public AquaticFoodTransport
{
public:
    enum { SerialId = 25 };

    MarineFishTransport(IModelFactory * fact, const KModelInfo& inf);

protected:
    int fishType() const;
};

class MarineShellfishTransport : public AquaticFoodTransport
{
public:
    enum { SerialId = 26 };

    MarineShellfishTransport(IModelFactory * fact, const KModelInfo& inf);

protected:
    int fishType() const;
};



#endif // AQUATICFOODTRANSPORT_H
