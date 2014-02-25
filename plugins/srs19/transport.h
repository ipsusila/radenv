#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "imodel.h"

class Transport : public IModel
{
protected:
    Transport(IModelFactory * fact, const KModelInfo& inf);
public:
    KDataArray result();
    void refresh();

    const PortList & outputs() const;
    const PortList & inputs() const;
    QString displayText() const;
    KDataGroupArray * userInputs();

protected:
    PortList        _outPorts;
    PortList        _inpPorts;
    KDataArray      _dataList;
    KDataGroupArray _userInputs;
};

#endif // TRANSPORT_H
