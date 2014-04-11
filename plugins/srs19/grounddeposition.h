#ifndef GROUNDDEPOSITION_H
#define GROUNDDEPOSITION_H

#include "transport.h"

class GroundDeposition : public Transport
{
public:
    enum {SerialId = 23};

    GroundDeposition(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool needLocation() const;
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // GROUNDDEPOSITION_H
