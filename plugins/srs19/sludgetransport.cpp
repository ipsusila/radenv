#include "sludgetransport.h"
#include "quantity.h"
#include "radcore.h"

SludgeTransport::SludgeTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}
bool SludgeTransport::needLocation() const
{
    return false;
}

/**
 * @brief Create output ports
 * @return
 */
bool SludgeTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::ConcentrationInWetSewage, KPort::Input);
    _outPorts << new KPort(this, &Srs19::SurfaceSludgeConcentration, KPort::Output);

    return true;
}
void SludgeTransport::defineParameters()
{
    //define user inputs
    DataGroup dg1(QObject::tr("Sludge parameters"));
    dg1 << KData(&Srs19::SewageSludgeDensity, 1000)
        << KData(&Srs19::SludgeContainerDepth, 1);
    userInputs()->append(dg1);
}

bool SludgeTransport::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    KDataGroupArray * ui = userInputs();
    qreal p = ui->numericValueOf(Srs19::SewageSludgeDensity);
    qreal d = ui->numericValueOf(Srs19::SludgeContainerDepth);
    //qreal ps = ui->numericValueOf(Srs19::SolidMaterialConcentration) / 100.0;

    KData daCsl = _inpPorts.data(0, Srs19::ConcentrationInWetSewage);
    DataItemArray cslList;
    for(int k = 0; k < daCsl.count(); k++) {
        const KDataItem & cslItem = daCsl.at(k);
        qreal vCsl = cslItem.numericValue() * p * d;
        cslList << KDataItem(cslItem.name(), vCsl, KData::Radionuclide);
    }

    if (!cslList.isEmpty()) {
        (*calcResult) << KData(&Srs19::SurfaceSludgeConcentration, cslList);
    }

    return true;
}

bool SludgeTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }

    KDataGroupArray * ui = userInputs();
    qreal p = ui->numericValueOf(Srs19::SewageSludgeDensity);
    if (p <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::SewageSludgeDensity);
        err ++;
    }

    qreal d = ui->numericValueOf(Srs19::SludgeContainerDepth);
    if (d <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::SludgeContainerDepth);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

