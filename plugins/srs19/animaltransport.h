#ifndef ANIMALTRANSPORT_H
#define ANIMALTRANSPORT_H

#include "foodchain.h"
#include "transferfactor.h"

class AnimalTransport : public FoodChain
{
public:
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    TransferFactor _Fmf;

    bool isSmallAnimal() const;
    virtual const Quantity * feedingAmountSymbol() const = 0;
    virtual const Quantity * consumptionDelaySymbol() const = 0;
    virtual const Quantity * outSymbol() const = 0;
    virtual const Quantity * uptakeRateSymbol() const = 0;
    bool allocateIoPorts();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
    void calculate(const KDataItem & CaItem, qreal cwi,  qreal Fmf, qreal Qmf, qreal Qw, qreal Tmf, KDataArray * result);

    AnimalTransport(IModelFactory * fact, const KModelInfo& inf);
};

class MilkTransport : public AnimalTransport
{
public:
    enum {SerialId = 18};

    MilkTransport(IModelFactory * fact, const KModelInfo& inf);
protected:

    void defineParameters();
    virtual const Quantity * feedingAmountSymbol() const;
    virtual const Quantity * consumptionDelaySymbol() const;
    virtual const Quantity * outSymbol() const;
    virtual const Quantity * uptakeRateSymbol() const;
};

class MeatTransport : public AnimalTransport
{
public:
    enum {SerialId = 19};

    MeatTransport(IModelFactory * fact, const KModelInfo& inf);
protected:

    void defineParameters();
    virtual const Quantity * feedingAmountSymbol() const;
    virtual const Quantity * consumptionDelaySymbol() const;
    virtual const Quantity * outSymbol() const;
    virtual const Quantity * uptakeRateSymbol() const;
};

#endif // ANIMALTRANSPORT_H
