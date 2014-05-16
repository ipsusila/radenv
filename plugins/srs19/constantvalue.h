#ifndef CONSTANTVALUE_H
#define CONSTANTVALUE_H

#include "imodel.h"

class ConstantValue : public IModel
{
public:
    enum { SerialId = 12 };

    ConstantValue(IModelFactory * fact, const KModelInfo& inf);
    KDataArray result() const;
    bool calculate(const KCalculationInfo& ci);
    bool needLocation() const;
    QRectF modelRect() const;

    const KPortList & outputs() const;
    QString displayText() const;

    bool verify(int * err = 0, int * warn = 0);
    virtual QDataStream & serialize(QDataStream &stream) const;
    virtual QDataStream & deserialize(QDataStream &stream);
    virtual void copyDataTo(IModel * model) const;

protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
    KData modelData(const Quantity & qty) const;

    KPortList        _outPorts;
    KDataArray      _dataList;
};

#endif // CONSTANTVALUE_H
