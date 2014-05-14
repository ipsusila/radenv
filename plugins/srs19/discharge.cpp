#include "discharge.h"
#include "quantity.h"
#include "radcore.h"

#include "widgetatmosphericdischarge.h"
#include "widgetwaterdischarge.h"

///////////////////////////////////////////////////////////////////////////////
Discharge::Discharge(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KData Discharge::modelData(const Quantity &qty) const
{
    KData d = _dataList.find(qty);
    if (d.isValid())
        return d;
    return constUserInputs().find(qty);
}

KDataArray Discharge::result() const
{
    return _dataList;
}
bool Discharge::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);
    return !_dataList.isEmpty();
}

const KPortList & Discharge::outputs() const
{
    return _outPorts;
}
QString Discharge::displayText() const
{
    return "";
}
QDataStream & Discharge::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    IModel::serialize(stream);
    _dataList.serialize(stream);
    return stream;
}
QDataStream & Discharge::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    IModel::deserialize(stream);
    _dataList.deserialize(stream);
    return stream;
}

/////////////////////////////////////////////////////////////////////////////////////////
AirDischarge::AirDischarge(IModelFactory * fact, const KModelInfo& inf)
    : Discharge(fact, inf)
{
}
/**
 * @brief Create output ports
 * @return
 */
bool AirDischarge::allocateIoPorts()
{
    _outPorts << new KPort(this, &Srs19::AtmosphericDischargeRate, KPort::Output);
    return true;
}
IUserInput * AirDischarge::createUserInputWidget(QWidget *parent)
{
    WidgetAtmosphericDischarge * w =
            new WidgetAtmosphericDischarge(factory()->storage(), &_dataList, parent);
    return w;
}

bool AirDischarge::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //release height
    KData xd = _dataList.find(Srs19::ReleaseHeight);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReleaseHeight);
        err ++;
    }

    xd = _dataList.find(Srs19::DischargePeriod);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::DischargePeriod);
        err ++;
    }

    xd = _dataList.find(Srs19::AtmosphericDischargeRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::AtmosphericDischargeRate);
        err ++;
    }

    //optional parameter
    xd = _dataList.find(Srs19::AirFlowRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::warningNotProperlyDefined(this, Srs19::AirFlowRate);
        warn ++;
    }

    xd = _dataList.find(Srs19::Diameter);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::warningNotProperlyDefined(this, Srs19::Diameter);
        warn ++;
    }

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        KOutputProxy::warningLocationNotSpecified(this);
        warn ++;
    }
    else {
        //assign location
        _dataList.setLocation(this->location());
    }

    KOutputProxy::infoVerificationResult(this, err, warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool AirDischarge::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);
    return !_dataList.isEmpty();
}

WaterDischarge::WaterDischarge(IModelFactory * fact, const KModelInfo& inf)
    : Discharge(fact, inf)
{
}
/**
 * @brief Create output ports
 * @return
 */
bool WaterDischarge::allocateIoPorts()
{
    _outPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Output);
    return true;
}
IUserInput * WaterDischarge::createUserInputWidget(QWidget *parent)
{
    WidgetWaterDischarge * w =
            new WidgetWaterDischarge(factory()->storage(), &_dataList, parent);
    return w;
}
bool WaterDischarge::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //discharge period
    KData xd = _dataList.find(Srs19::DischargePeriod);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::DischargePeriod);
        err ++;
    }

    xd = _dataList.find(Srs19::WaterDischargeRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::WaterDischargeRate);
        err ++;
    }

    xd = _dataList.find(Srs19::LiquidFlowRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::LiquidFlowRate);
        err ++;
    }

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        KOutputProxy::warningLocationNotSpecified(this);
        warn ++;
    }
    else {
        //assign location
        _dataList.setLocation(this->location());
    }

    KOutputProxy::infoVerificationResult(this, err, warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}
bool WaterDischarge::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);

    const KData & daQi = _dataList.find(Srs19::WaterDischargeRate);
    const KData & daF = _dataList.find(Srs19::LiquidFlowRate);
    if (!daQi.isValid()) {
        KOutputProxy::errorNotSpecified(this, Srs19::WaterDischargeRate);
        return false;
    }
    if (!daF.isValid()) {
        KOutputProxy::errorNotSpecified(this, Srs19::LiquidFlowRate);
        return false;
    }

    DataItemArray C0Array;
    qreal F = daF.numericValue();
    for (int k = 0; k < daQi.count(); k++) {
        const KDataItem & di = daQi.at(k);
        qreal C0 = di.numericValue() / F;

        C0Array << KDataItem(di.name(), C0, KData::Radionuclide);
    }
    //replace existing
    _dataList.appendOrReplace(KData(&Srs19::EffluentRadionuclideConcentration, C0Array));

    return true;
}
