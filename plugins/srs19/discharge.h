#ifndef DISCHARGE_H
#define DISCHARGE_H

#include "imodel.h"

class Discharge : public IModel
{
protected:
    Discharge(IModelFactory * fact, const KModelInfo& inf);
public:
    KDataArray result() const;
    bool calculate(const KCalculationInfo& ci);
    void refresh();
    KDataGroupArray * userInputs();

    const KPortList & outputs() const;
    QString displayText() const;

protected:
    KPortList        _outPorts;
    KDataArray       _dataList;
    KDataGroupArray _userInputs;

    KData modelData(const Quantity & qty) const;
};

class AirDischarge : public Discharge
{
public:
    enum { SerialId = 1 };

    AirDischarge(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
};

class WaterDischarge : public Discharge
{
public:
    enum { SerialId = 2 };

    WaterDischarge(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
};

#endif // DISCHARGE_H
