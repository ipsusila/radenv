#include "transport.h"
#include "kport.h"

Transport::Transport(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KDataGroupArray * Transport::userInputs()
{
    return &_userInputs;
}
KDataGroupArray & Transport::toUserInputs(const KDataArray & da)
{
    _userInputs = KDataGroupArray(QObject::tr("User inputs"), da);
    return _userInputs;
}

KData Transport::modelData(const Quantity &sym) const
{
    KData d = _dataList.find(sym);
    if (d.isValid())
        return d;
    return _userInputs.find(sym);
}

KDataArray Transport::result() const
{
    return _dataList;
}
bool Transport::calculate(const KCalculationInfo &ci)
{
    //clear results
    _dataList.clear();

    if (needLocation()) {
        //check for distance
        KLocationPort * lp = locationPort();
        if (lp == 0)
            return false;

        KLocation loc = lp->location();
        _dataList.setLocation(loc);
        return calculate(ci, loc, &_dataList);
    }
    else {
        //get location from port data
        KLocation loc = _inpPorts.firstValidLocation();
        _dataList.setLocation(loc);
        return calculate(ci, loc, &_dataList);
    }
}

void Transport::refresh()
{
    KLocationPort * lp = locationPort();
    if (lp)
        lp->refresh();
}

const KPortList & Transport::inputs() const
{
    return _inpPorts;
}

const KPortList & Transport::outputs() const
{
    return _outPorts;
}
QString Transport::displayText() const
{
    return _dataList.displayText();
}
