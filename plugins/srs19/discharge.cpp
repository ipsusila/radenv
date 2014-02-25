#include<QtDebug>
#include<QtCore/qmath.h>
#include "discharge.h"
#include "kport.h"
#include "koutput.h"

#include "dialogatmosphericdischarge.h"
#include "dialogwaterdischarge.h"
#include "dialogsewagedischarge.h"
#include "symbol.h"

///////////////////////////////////////////////////////////////////////////////
Discharge::Discharge(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KDataArray Discharge::result()
{
    return _dataList;
}
bool Discharge::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);
    return !_dataList.isEmpty();
}

void Discharge::refresh()
{
    KLocationPort * lp = locationPort();
    if (lp)
        lp->refresh();
}

bool Discharge::isSource() const
{
    return true;
}

const PortList & Discharge::outputs() const
{
    return _outPorts;
}
QString Discharge::displayText() const
{
    return _dataList.displayText();
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
void AirDischarge::promptParameters()
{
    DialogAtmosphericDischarge dlg(_dataList);
    if (dlg.exec() == QDialog::Accepted) {
        _dataList = dlg.dataList();
        setToolTip(displayText());
        xInfo() << *this << QObject::tr(" parameters is assigned.")
                << KOutput::EndLine << _dataList.displayText();
    }
}
bool AirDischarge::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;
    //mandatory parameter
    if (_dataList.isEmpty()) {
        xError() << *this << QObject::tr("Atmospheric discharge parameter is not defined.");
        err ++;
    }

    //release height
    KData xd = _dataList.find(Srs19::ReleaseHeight);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Parameter [Release height] not set.");
        err ++;
    }

    xd = _dataList.find(Srs19::DischargePeriod);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Parameter [Discharge period] not set.");
        err ++;
    }

    xd = _dataList.find(Srs19::AtmosphericDischargeRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Radionuclides and/or Discharge rate value not set properly.");
        err ++;
    }

    //optional parameter
    xd = _dataList.find(Srs19::AirFlowRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xWarning() << *this << QObject::tr("Parameter [Air flow rate] not set.");
        warn ++;
    }

    xd = _dataList.find(Srs19::Diameter);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xWarning() << *this << QObject::tr("Parameter [Stack diameter] is not set.");
        warn ++;
    }

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        xWarning() << *this << QObject::tr("Receptor location is not specified. Please double-click the port to specify location.");
        warn ++;
    }

    xInfo() << tagName() << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool AirDischarge::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool AirDischarge::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
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
void WaterDischarge::promptParameters()
{
    DialogWaterDischarge dlg(_dataList);
    if (dlg.exec() == QDialog::Accepted) {
        _dataList = dlg.dataList();
        setToolTip(displayText());
        xInfo() << *this << QObject::tr(" parameters is assigned.")
                << KOutput::EndLine << _dataList.displayText();
    }
}
bool WaterDischarge::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;
    //mandatory parameter
    if (_dataList.isEmpty()) {
        xError() << *this << QObject::tr("Surface water discharge parameter is not defined.");
        err ++;
    }

    //discharge period
    KData xd = _dataList.find(Srs19::DischargePeriod);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Parameter [Discharge period] not set.");
        err ++;
    }

    xd = _dataList.find(Srs19::WaterDischargeRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Radionuclides and/or Discharge rate value not set properly.");
        err ++;
    }

    xd = _dataList.find(Srs19::LiquidFlowRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xWarning() << *this << QObject::tr("Parameter [Liquid flow rate] not set.");
        err ++;
    }

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        xWarning() << *this << QObject::tr("Receptor location is not specified. Please double-click the port to specify location.");
        warn ++;
    }

    xInfo() << tagName() << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);

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
    if (!daQi.isValid() || !daF.isValid()) {
        xWarning() << *this << QObject::tr("Discharge rate and/or flow rate not set");
        return false;
    }

    QVector<KDataItem> C0Array;
    qreal F = daF.numericValue();
    for (int k = 0; k < daQi.count(); k++) {
        const KDataItem & di = daQi.at(k);
        qreal C0 = di.numericValue() / F;

        C0Array << KDataItem(di.name(), C0);
    }
    //replace existing
    _dataList.addOrReplace(KData(&Srs19::EffluentRadionuclideConcentration, C0Array));

    return true;
}


bool WaterDischarge::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool WaterDischarge::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
}

SewageDischarge::SewageDischarge(IModelFactory * fact, const KModelInfo& inf)
    : Discharge(fact, inf)
{
}
bool SewageDischarge::allocateIoPorts()
{
    _outPorts << new KPort(this, &Srs19::AverageConcentrationInSewage, KPort::Output);
    return true;
}
void SewageDischarge::promptParameters()
{
    DialogSewageDischarge dlg(_dataList);
    if (dlg.exec() == QDialog::Accepted) {
        _dataList = dlg.dataList();
        setToolTip(displayText());
        xInfo() << *this << QObject::tr(" parameters is assigned.")
                << KOutput::EndLine << _dataList.displayText();
    }
}
bool SewageDischarge::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;
    //mandatory parameter
    if (_dataList.isEmpty()) {
        xError() << *this << QObject::tr("Sewage sludge discharge parameter is not defined.");
        err ++;
    }

    //discharge period
    KData xd = _dataList.find(Srs19::DischargePeriod);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Parameter [Discharge period] not set.");
        err ++;
    }

    xd = _dataList.find(Srs19::SewageDischargeRate);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xError() << *this << QObject::tr("Radionuclides and/or Discharge rate value not set properly.");
        err ++;
    }

    xd = _dataList.find(Srs19::AnnualSludgeProduction);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xWarning() << *this << QObject::tr("Annual sludge production not set.");
        err ++;
    }

    xd = _dataList.find(Srs19::NumOfServedPerson);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        xWarning() << *this << QObject::tr("Number of served person in the facility not set.");
        err ++;
    }

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        xWarning() << *this << QObject::tr("Receptor location is not specified. Please double-click the port to specify location.");
        warn ++;
    }

    xInfo() << tagName() << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool SewageDischarge::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool SewageDischarge::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
}
