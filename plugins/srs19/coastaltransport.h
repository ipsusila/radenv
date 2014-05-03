#ifndef COASTALTRANSPORT_H
#define COASTALTRANSPORT_H

#include "transport.h"

class CoastalTransport : public Transport
{
public:
    enum { SerialId = 7};

    CoastalTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);

    bool doVerification(int * oerr, int * owarn);
};

class GenericCoastalTransport : public CoastalTransport
{
public:
    enum {SerialId = 10};

    GenericCoastalTransport(IModelFactory * fact, const KModelInfo& inf);
    bool verify(int * err = 0, int * warn = 0);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult);
};

#endif // COASTALTRANSPORT_H
