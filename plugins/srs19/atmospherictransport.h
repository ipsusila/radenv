#ifndef ATMOSPHERICTRANSPORT_H
#define ATMOSPHERICTRANSPORT_H

#include "transport.h"

class AtmosphericTransport : public Transport
{
public:
    enum { SerialId = 4 };

    AtmosphericTransport(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();

    qreal calcSigmaZ(qreal x, qreal H);
    qreal calcF(qreal x, qreal H);
    qreal calcB(qreal x, qreal H, qreal AB);
    qreal calcModifiedConcentration(const QString& nuc, qreal x, qreal ua, KRadionuclide * radinuclide);
    void calcDisplacementZone(qreal x, qreal H, const KDataArray& inpQi);
    void calcWakeZone(qreal x, qreal H, qreal AB, const KDataArray& inpQi);
    void calcCavityZone(qreal x, qreal HB, bool onSameBuilding, const KDataArray& inpQi);
};

#endif // ATMOSPHERICTRANSPORT_H
