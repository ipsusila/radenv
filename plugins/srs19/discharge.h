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

    const KPortList & outputs() const;
    QString displayText() const;
    virtual QDataStream & serialize(QDataStream &stream) const;
    virtual QDataStream & deserialize(QDataStream &stream);
    virtual void copyDataTo(IModel * model) const;

protected:
    KPortList        _outPorts;
    KDataArray       _dataList;

    KData modelData(const Quantity & qty) const;
};

class AirDischarge : public Discharge
{
public:
    enum { SerialId = 1 };

    AirDischarge(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
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
protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
};

#endif // DISCHARGE_H
