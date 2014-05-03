#ifndef SMALLLAKETRANSPORT_H
#define SMALLLAKETRANSPORT_H

#include "transport.h"

class SmallLakeTransport : public Transport
{
public:
    enum {SerialId = 8};

    SmallLakeTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool needLocation() const;
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // SMALLLAKETRANSPORT_H
