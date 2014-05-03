#include "transport.h"
#include "kport.h"

Transport::Transport(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KData Transport::modelData(const Quantity &qty) const
{
    KData d = _dataList.find(qty);
    if (d.isValid())
        return d;
    return constUserInputs().find(qty);
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

        KLocation loc = this->location();
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
    return "";
}
