#ifndef DISCHARGE_H
#define DISCHARGE_H

#include "imodel.h"

class Discharge : public IModel
{
protected:
    Discharge(IModelFactory * fact, const KModelInfo& inf);
public:
    KDataArray result();
    bool calculate(const KCalculationInfo& ci);
    void refresh();
    bool isSource() const;

    const PortList & outputs() const;
    QString displayText() const;

protected:
    PortList        _outPorts;
    KDataArray       _dataList;
};

class AirDischarge : public Discharge
{
public:
    enum { SerialId = 1 };

    AirDischarge(IModelFactory * fact, const KModelInfo& inf);

    void promptParameters();
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

class WaterDischarge : public Discharge
{
public:
    enum { SerialId = 2 };

    WaterDischarge(IModelFactory * fact, const KModelInfo& inf);

    bool calculate(const KCalculationInfo& ci);
    void promptParameters();
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

class SewageDischarge : public Discharge
{
public:
    enum { SerialId = 3};

    SewageDischarge(IModelFactory * fact, const KModelInfo& inf);
    void promptParameters();
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * i);
protected:
    bool allocateIoPorts();
};

#endif // DISCHARGE_H
