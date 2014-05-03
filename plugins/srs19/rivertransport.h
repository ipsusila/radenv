#ifndef RIVERTRANSPORT_H
#define RIVERTRANSPORT_H

#include "transport.h"

class RiverTransport : public Transport
{
public:
    enum {SerialId = 5};
    RiverTransport(IModelFactory * fact, const KModelInfo& inf);

    virtual bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    virtual void defineParameters();

    bool doVerification(int * oerr, int * owarn);
    void estimateParameters();
    void calcualteConcentration(qreal x, qreal qr, qreal U, KDataArray * calcResult, qreal Pr = 1.0);
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // RIVERTRANSPORT_H
