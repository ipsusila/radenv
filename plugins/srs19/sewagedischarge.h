#ifndef SEWAGEDISCHARGE_H
#define SEWAGEDISCHARGE_H

#include "discharge.h"

class SewageDischarge : public Discharge
{
public:
    enum { SerialId = 3};

    SewageDischarge(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    //IUserInput * createUserInputWidget(QWidget *parent);
    void defineParameters();
};

#endif // SEWAGEDISCHARGE_H
