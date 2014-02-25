#ifndef SMALLLAKETRANSPORT_H
#define SMALLLAKETRANSPORT_H

#include "transport.h"

class SmallLakeTransport : public Transport
{
public:
    enum {SerialId = 8};

    SmallLakeTransport(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

#endif // SMALLLAKETRANSPORT_H
