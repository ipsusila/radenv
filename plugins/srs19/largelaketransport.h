#ifndef LARGELAKETRANSPORT_H
#define LARGELAKETRANSPORT_H

#include "transport.h"

class LargeLakeTransport : public Transport
{
public:
    enum {SerialId = 9};

    LargeLakeTransport(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

#endif // LARGELAKETRANSPORT_H
