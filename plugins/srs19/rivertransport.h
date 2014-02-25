#ifndef RIVERTRANSPORT_H
#define RIVERTRANSPORT_H

#include "transport.h"

class RiverTransport : public Transport
{
public:
    enum {SerialId = 5};
    RiverTransport(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();

    void estimateParameters();
    void calcualteConcentration(qreal x, qreal qr, qreal U, const KDataArray& inpQi, qreal Pr = 1.0);
};

#endif // RIVERTRANSPORT_H
