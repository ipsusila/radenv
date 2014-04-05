#ifndef SEDIMENTEFFECT_H
#define SEDIMENTEFFECT_H

#include "transport.h"
#include "distributioncoefficient.h"

class SedimentEffect : public Transport
{
public:
    enum {SerialId = 11};

    SedimentEffect(IModelFactory * fact, const KModelInfo& inf);

    QString displayText() const;
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
    QRectF modelRect() const;

protected:
    DistributionCoefficient _kdValues;

    bool needLocation() const;
    void defineParameters();
    bool allocateIoPorts();
    bool calculate(const KCalculationInfo &ci, const KLocation & loc, KDataArray * calResult);
    void calculate(const KDataItem & cwItem, qreal Ss, qreal Te, qreal Kd, KDataArray * result);
};

#endif // SEDIMENTEFFECT_H
