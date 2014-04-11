#ifndef SEDIMENTEFFECT_H
#define SEDIMENTEFFECT_H

#include "transport.h"
#include "defaultvalues.h"

class SedimentEffect : public Transport
{
public:
    QString displayText() const;
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
    QRectF modelRect() const;

protected:
    DistributionCoefficient _kdValues;

    SedimentEffect(IModelFactory * fact, const KModelInfo& inf);
    virtual int waterType() const = 0;
    virtual qreal defaultSuspendedSedimentConcentration() const = 0;
    bool needLocation() const;
    void defineParameters();
    bool allocateIoPorts();
    bool calculate(const KCalculationInfo &ci, const KLocation & loc, KDataArray * calResult);
    void calculate(const KDataItem & cwItem, qreal Ss, qreal Te, qreal Kd, KDataArray * result);
};

class FreshwaterSediment : public SedimentEffect
{
public:
    enum {SerialId = 11};   //todo

    FreshwaterSediment(IModelFactory * fact, const KModelInfo& inf);

protected:
    qreal defaultSuspendedSedimentConcentration() const;
    int waterType() const;
};

class CoastalSediment : public SedimentEffect
{
public:
    enum {SerialId = 21};   //todo

    CoastalSediment(IModelFactory * fact, const KModelInfo& inf);
protected:
    qreal defaultSuspendedSedimentConcentration() const;
    int waterType() const;
};

#endif // SEDIMENTEFFECT_H
