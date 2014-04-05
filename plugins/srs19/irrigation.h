#ifndef IRRIGATION_H
#define IRRIGATION_H

#include "transport.h"

class Irrigation : public Transport
{
public:
    enum {SerialId = 13};

    Irrigation(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    void defineParameters();
    bool needLocation() const;
    bool calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult);
};

#endif // IRRIGATION_H
