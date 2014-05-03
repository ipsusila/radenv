#ifndef LARGELAKETRANSPORT_H
#define LARGELAKETRANSPORT_H

#include "transport.h"

class LargeLakeTransport : public Transport
{
public:
    enum {SerialId = 9};

    LargeLakeTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // LARGELAKETRANSPORT_H
