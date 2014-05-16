#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "imodel.h"
#include "klocation.h"

class Transport : public IModel
{
protected:
    Transport(IModelFactory * fact, const KModelInfo& inf);
public:
    virtual KDataArray result() const;
    virtual const KPortList & outputs() const;
    virtual const KPortList & inputs() const;
    virtual QString displayText() const;
    virtual bool calculate(const KCalculationInfo& ci);
    virtual void copyDataTo(IModel *model) const;

protected:
    KPortList        _outPorts;
    KPortList        _inpPorts;

    virtual KData modelData(const Quantity & qty) const;
    virtual bool calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calResult) = 0;

private:
    KDataArray      _dataList;
};

#endif // TRANSPORT_H
