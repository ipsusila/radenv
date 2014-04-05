#ifndef ATMOSPHERICTRANSPORT_H
#define ATMOSPHERICTRANSPORT_H

#include "transport.h"

class AtmosphericTransport : public Transport
{
public:
    enum { SerialId = 4 };

    AtmosphericTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    void defineParameters();

    qreal calcSigmaZ(qreal x, qreal H, KDataArray * calcResult);
    qreal calcF(qreal x, qreal H, KDataArray * calcResult);
    qreal calcB(qreal x, qreal H, qreal AB, KDataArray * calcResult);
    qreal calcModifiedConcentration(const QString& nuc, qreal x, qreal ua, KRadionuclide * radinuclide);
    void calcDisplacementZone(qreal x, qreal H, KDataArray * calcResult);
    void calcWakeZone(qreal x, qreal H, qreal AB, KDataArray * calcResult);
    void calcCavityZone(qreal x, qreal HB, bool onSameBuilding, KDataArray * calcResult);

    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // ATMOSPHERICTRANSPORT_H
