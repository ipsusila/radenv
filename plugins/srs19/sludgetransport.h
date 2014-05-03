#ifndef SLUDGETRANSPORT_H
#define SLUDGETRANSPORT_H

#include "transport.h"

class SludgeTransport : public Transport
{
public:
    enum {SerialId = 22};

    SludgeTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool needLocation() const;
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // SLUDGETRANSPORT_H
