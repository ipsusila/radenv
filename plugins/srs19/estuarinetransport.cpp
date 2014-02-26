#include "estuarinetransport.h"
#include "kport.h"
#include "koutput.h"
#include "symbol.h"

EstuarineTransport::EstuarineTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
    DataGroup dg1(QObject::tr("River conditions"));
    dg1 << KData(&Srs19::RiverEstuaryWidth, 0)
        << KData(&Srs19::FlowDepth, 0)
        << KData(&Srs19::LowRiverFlowRate, 0);
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Parameter estimation"));
    dg2 << KData(&Srs19::EstimateParameters, false)
        << KData(&Srs19::TidalRiverWidth, 0);
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Estuary conditions"));
    dg3 << KData(&Srs19::TidalPeriod, 0)
        << KData(&Srs19::MaximumEbbVelocity, 0.5)
        << KData(&Srs19::MaximumFloodVelocity, 0.5);
    _userInputs << dg3;

    DataGroup dg4(QObject::tr("Receptor position"));
    dg4 << KData(&Srs19::ReceptorOnOpposite, false)
        << KData(&Srs19::ReceptorAtUpstream, false);
    _userInputs << dg4;
}

/**
 * @brief Create output ports
 * @return
 */
bool EstuarineTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Output);

    return true;
}
bool EstuarineTransport::calculate(const KCalculationInfo& ci)
{
    return true;
}
bool EstuarineTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        xError() << *this << QObject::tr("Receptor location is not specified. Please double-click the port to specify location.");
        err ++;
    }

    //check wether input port is connected or not
    if (!_inpPorts.first()->isConnected()) {
        xError() << *this << QObject::tr("Input port is not connected.");
        err ++;
    }

    KDataArray daInput = this->_inpPorts.first()->result();
    KData daF = daInput.find(Srs19::LiquidFlowRate);
    if (!daF.isValid() || daF.numericValue() <= 0) {
        xError() << *this << QObject::tr("Liquid flow rate not specified.");
        err++;
    }

    bool estimate = _userInputs.valueOf(Srs19::EstimateParameters).toBool();
    if (estimate) {
        qreal Bd = _userInputs.numericValueOf(Srs19::TidalRiverWidth);
        if (Bd <= 0) {
            xError() << *this << QObject::tr("River width for parameter estimation not specified.");
            err++;
        }
    }
    else {
        //ceck all parameter
        /*
        qreal B = _userInputs.numericValueOf(Srs19::RiverEstuaryWidth);
        qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
        qreal U = _userInputs.numericValueOf(Srs19::NetFreshwaterVelocity);
        qreal D = _userInputs.numericValueOf(Srs19::FlowDepth);

        if (B <= 0) {
            xError() << *this << QObject::tr("River width not specified.");
            err++;
        }

        if (qr <= 0) {
            xError() << *this << QObject::tr("River flow rate(qr) not specified.");
            err++;
        }

        if (U <= 0) {
            xError() << *this << QObject::tr("Net fresh water velocity not specified.");
            err++;
        }

        if (D <= 0) {
            xError() << *this << QObject::tr("Flow depth not specified.");
            err++;
        }
        */
    }

    xInfo() << tagName() << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool EstuarineTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool EstuarineTransport::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
}

