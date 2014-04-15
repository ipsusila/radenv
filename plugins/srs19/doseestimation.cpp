#include "doseestimation.h"

///////////////////////////////////////////////////////////////////////////////
DoseEstimation::DoseEstimation(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KData DoseEstimation::modelData(const Quantity &qty) const
{
    KData d = _dataList.find(qty);
    if (d.isValid())
        return d;
    return _userInputs.find(qty);
}

KDataGroupArray * DoseEstimation::userInputs()
{
    return &_userInputs;
}

KDataArray DoseEstimation::result() const
{
    return _dataList;
}
bool DoseEstimation::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);
    return !_dataList.isEmpty();
}
qreal DoseEstimation::calcDose(qreal c, qreal hd, qreal df) const
{
    return (c*hd*df);
}
qreal DoseEstimation::calcResDose(qreal c, qreal rinh, qreal dfinh, qreal of, qreal dl) const
{
    return (c*rinh*dfinh*of*dl);
}

void DoseEstimation::refresh()
{
    KLocationPort * lp = locationPort();
    if (lp)
        lp->refresh();
}

bool DoseEstimation::isSource() const
{
    return false;
}

const KPortList & DoseEstimation::inputs() const
{
    return _inpPorts;
}
QString DoseEstimation::displayText() const
{
    return "";
}
