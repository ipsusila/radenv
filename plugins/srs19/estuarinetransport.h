#ifndef ESTUARINETRANSPORT_H
#define ESTUARINETRANSPORT_H

#include "rivertransport.h"

class EstuarineTransport : public RiverTransport
{
public:
    enum {SerialId = 6};
    EstuarineTransport(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
protected:
    void defineParameters();

    bool calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult);
};

#endif // ESTUARINETRANSPORT_H
