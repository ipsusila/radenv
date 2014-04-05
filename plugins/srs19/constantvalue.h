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
    void refresh();
    bool isSource() const;
    bool needLocation() const;
    QRectF modelRect() const;

    const KPortList & outputs() const;
    QString displayText() const;

    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);

protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
    KData modelData(const Quantity & sym) const;

    KPortList        _outPorts;
    KDataArray      _dataList;
};

#endif // CONSTANTVALUE_H
