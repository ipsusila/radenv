#include "transport.h"

Transport::Transport(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KDataGroupArray * Transport::userInputs()
{
    return &_userInputs;
}

KDataArray Transport::result()
{
    return _dataList;
}

void Transport::refresh()
{
    KLocationPort * lp = locationPort();
    if (lp)
        lp->refresh();
}

const PortList & Transport::inputs() const
{
    return _inpPorts;
}

const PortList & Transport::outputs() const
{
    return _outPorts;
}
QString Transport::displayText() const
{
    return _dataList.displayText();
}
