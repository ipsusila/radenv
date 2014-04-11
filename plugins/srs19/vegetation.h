#ifndef VEGETATION_H
#define VEGETATION_H

#include "foodchain.h"
#include "defaultvalues.h"

class Vegetation : public FoodChain
{
public:
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);

protected:
    TransferFactor _fvValues;

    Vegetation(IModelFactory * fact, const KModelInfo& inf);
    bool allocateIoPorts();
    bool calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calResult);
    qreal calculate(const KDataItem& d1, const KDataItem& d2, qreal Fv, qreal a, qreal p,
                   qreal lw, bool defLs, qreal te, qreal tb, qreal th, KDataArray * calcResult);

    virtual int vegetationType() const = 0;
};

class Crop : public Vegetation
{
public:
    enum {SerialId = 14};

    Crop(IModelFactory * fact, const KModelInfo& inf);
protected:
    int vegetationType() const;
    void defineParameters();
};

class Forage : public Vegetation
{
public:
    enum {SerialId = 15};

    Forage(IModelFactory * fact, const KModelInfo& inf);
protected:
    int vegetationType() const;
    void defineParameters();
};

#endif // VEGETATION_H
