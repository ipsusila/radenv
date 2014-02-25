#ifndef ESTUARINETRANSPORT_H
#define ESTUARINETRANSPORT_H

#include "transport.h"

class EstuarineTransport : public Transport
{
public:
    enum {SerialId = 6};
    EstuarineTransport(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

#endif // ESTUARINETRANSPORT_H
