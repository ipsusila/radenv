#ifndef COASTALTRANSPORT_H
#define COASTALTRANSPORT_H

#include "transport.h"

class CoastalTransport : public Transport
{
public:
    enum { SerialId = 7};

    CoastalTransport(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

#endif // COASTALTRANSPORT_H
