#include "largelaketransport.h"
#include "quantity.h"
#include "radcore.h"

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
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInShoreline, KPort::Output)
              << new KPort(this, &Srs19::ConcentrationInWaterFish, KPort::Output);
    return true;
}
void LargeLakeTransport::defineParameters()
{
    //allocate parameters for user inputs
    DataGroup dg1(QObject::tr("Coastal parameters"));
    dg1 << KData(&Srs19::ReleaseToBeachDistance, 0)
        << KData(&Srs19::WaterDepth, 0)
        << KData(&Srs19::LakeFlowVelocity, 0.1);
    userInputs()->append(dg1);
}
bool LargeLakeTransport::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    //user input parameters
    qreal x = loc.distance(ci);
    KDataGroupArray * ui = userInputs();
    qreal y0 = ui->numericValueOf(Srs19::ReleaseToBeachDistance);
    qreal D = ui->numericValueOf(Srs19::WaterDepth);
    qreal U = ui->numericValueOf(Srs19::LakeFlowVelocity);

    //const KData & qiW = inpQi.find(Srs19::WaterDischargeRate);
    KData qiW = _inpPorts.data(Srs19::WaterDischargeRate);
    DataItemArray cwfList;
    DataItemArray cwsList;
    for(int k = 0; k < qiW.count(); k++) {
        const KDataItem & qi = qiW.at(k);
        const KRadionuclide & rn = KStorage::storage()->radionuclide(qi.name());
        qreal l = rn.halfLife().decayConstant();

        // maximum concentration
        //equation 20, page 45
        qreal cwf = (962 * qi.numericValue() * qPow(U, 0.17) * qExp((-l*x)/U)) /
                (D * qPow(x, 1.17));

        //equation 21, page 45.
        qreal cws = cwf * qExp((-7.28e+5 * qPow(U, 2.34) * y0 * y0) / qPow(x, 2.34));

        //add to result
        cwfList << KDataItem(qi.name(), cwf, KData::Radionuclide);
        cwsList << KDataItem(qi.name(), cws, KData::Radionuclide);
    }

    if (!cwsList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ConcentrationInWaterFish, cwfList);
        (*calcResult) << KData(&Srs19::ConcentrationInShoreline, cwsList);
    }

    return true;
}
bool LargeLakeTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        KOutputProxy::errorReceptorNotSpecified(this);
        err ++;
    }

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this);
        err ++;
    }

    KDataGroupArray * ui = userInputs();
    qreal y0 = ui->numericValueOf(Srs19::ReleaseToBeachDistance);
    if (y0 <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReleaseToBeachDistance);
        err ++;
    }

    qreal D = ui->numericValueOf(Srs19::WaterDepth);
    if (D <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::WaterDepth);
        err ++;
    }

    qreal U = ui->numericValueOf(Srs19::LakeFlowVelocity);
    if (U <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::LakeFlowVelocity);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}


