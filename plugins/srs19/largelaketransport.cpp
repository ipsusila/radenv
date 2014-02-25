#include "largelaketransport.h"
#include "kport.h"
#include "koutput.h"
#include "symbol.h"

LargeLakeTransport::LargeLakeTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}
/**
 * @brief Create output ports
 * @return
 */
bool LargeLakeTransport::allocateIoPorts()
{
    KPort * port = new KPort(this, &Srs19::AtmosphericDischargeRate, KPort::Input);
    _inpPorts.append(port);

    port = new KPort(this, &Srs19::ConcentrationInAir, KPort::Output);
    _outPorts.append(port);

    return true;
}
bool LargeLakeTransport::calculate(const KCalculationInfo& ci)
{
    return 0;
}
bool LargeLakeTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    /*
    //mandatory parameter
    if (_dataList.isEmpty()) {
        xError() << *this << QObject::tr("Atmospheric discharge parameter is not defined.");
        err ++;
    }

    //release height
    XData xd = _dataList.find(Srs19::ReleaseHeight);
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

    XLocationPort * lp = locationPort();
    if (lp == 0 || !lp->location().isValid()) {
        xWarning() << *this << QObject::tr("Receptor location is not specified. Please double-click the port to specify location.");
        warn ++;
    }

    xInfo() << tagName() << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    */

    return err == 0;
}

bool LargeLakeTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool LargeLakeTransport::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
}

