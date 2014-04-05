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
    virtual void refresh();

    virtual const KPortList & outputs() const;
    virtual const KPortList & inputs() const;
    virtual QString displayText() const;
    virtual KDataGroupArray * userInputs();
    virtual bool calculate(const KCalculationInfo& ci);

protected:
    KPortList        _outPorts;
    KPortList        _inpPorts;
    KDataGroupArray _userInputs;

    KDataGroupArray & toUserInputs(const KDataArray & da);
    virtual KData modelData(const Quantity & sym) const;
    virtual bool calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calResult) = 0;

private:
    KDataArray      _dataList;
};

#endif // TRANSPORT_H
