#ifndef VEGETATION_H
#define VEGETATION_H

#include "foodchain.h"
#include "transferfactor.h"

class Vegetation : public FoodChain
{
public:
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);

   // IUserInput * createUserInputWidget(QWidget *parent);

protected:
    TransferFactor _fvValues;

    Vegetation(IModelFactory * fact, const KModelInfo& inf);
    bool allocateIoPorts();
    bool calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calResult);
    void calculate(const KDataItem& d1, const KDataItem& d2, qreal Fv, qreal a, qreal p,
                   qreal lw, qreal ls, qreal te, qreal tb, qreal th, KDataArray * calcResult);

    virtual TransferFactor::ValueType vegetationType() const = 0;
};

class Crop : public Vegetation
{
public:
    enum {SerialId = 14};

    Crop(IModelFactory * fact, const KModelInfo& inf);
protected:
    TransferFactor::ValueType vegetationType() const;
    void defineParameters();
};

class Forage : public Vegetation
{
public:
    enum {SerialId = 15};

    Forage(IModelFactory * fact, const KModelInfo& inf);
protected:
    TransferFactor::ValueType vegetationType() const;
    void defineParameters();
};

#endif // VEGETATION_H
