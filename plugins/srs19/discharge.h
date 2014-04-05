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
    bool isSource() const;

    const KPortList & outputs() const;
    QString displayText() const;

protected:
    KPortList        _outPorts;
    KDataArray       _dataList;

    KData modelData(const Quantity & sym) const;
};

class AirDischarge : public Discharge
{
public:
    enum { SerialId = 1 };

    AirDischarge(IModelFactory * fact, const KModelInfo& inf);

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

class SewageDischarge : public Discharge
{
public:
    enum { SerialId = 3};

    SewageDischarge(IModelFactory * fact, const KModelInfo& inf);

    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
};

#endif // DISCHARGE_H
