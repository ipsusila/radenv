#include "coastaltransport.h"
#include "quantity.h"
#include "radcore.h"

CoastalTransport::CoastalTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool CoastalTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInShoreline, KPort::Output)
              << new KPort(this, &Srs19::ConcentrationInWaterFish, KPort::Output);

    return true;
}
void CoastalTransport::defineParameters()
{
    //allocate parameters for user inputs
    DataGroup dg1(QObject::tr("Coastal/Lake parameters"));
    dg1 << KData(&Srs19::ReleaseToBeachDistance, 0)
        << KData(&Srs19::WaterDepth, 0)
        << KData(&Srs19::CoastalCurrent, 0.1);
    _userInputs << dg1;
}

bool CoastalTransport::calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult)
{
    //user input parameters
    qreal x = loc.distance(ci);
    qreal y0 = _userInputs.numericValueOf(Srs19::ReleaseToBeachDistance);
    qreal D = _userInputs.numericValueOf(Srs19::WaterDepth);
    qreal U = _userInputs.numericValueOf(Srs19::CoastalCurrent);

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
bool CoastalTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;
    int result = doVerification(&err, &warn);

    //check counditions
    qreal D = _userInputs.numericValueOf(Srs19::WaterDepth);
    qreal x = this->location().distance();
    if ((7*D) >= x) {
        KOutputProxy::warningMessage(this, QObject::tr("Model condition is not satisfied (condition: 7D < x)"));
        warn ++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return result;
}

bool CoastalTransport::doVerification(int *oerr, int *owarn)
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

    qreal y0 = _userInputs.numericValueOf(Srs19::ReleaseToBeachDistance);
    if (y0 <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReleaseToBeachDistance);
        err ++;
    }

    qreal D = _userInputs.numericValueOf(Srs19::WaterDepth);
    if (D <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::WaterDepth);
        err ++;
    }

    qreal U = _userInputs.numericValueOf(Srs19::CoastalCurrent);
    if (U <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::CoastalCurrent);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool CoastalTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool CoastalTransport::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

GenericCoastalTransport::GenericCoastalTransport(IModelFactory * fact, const KModelInfo& inf)
    : CoastalTransport(fact, inf)
{
}
bool GenericCoastalTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Output);

    return true;
}
void GenericCoastalTransport::defineParameters()
{
    //allocate parameters for user inputs
    DataGroup dg1(QObject::tr("Coastal parameters"));
    dg1 << KData(&Srs19::ReleaseToBeachDistance, 0)
        << KData(&Srs19::DistanceFromShoreline, 0)
        << KData(&Srs19::WaterDepth, 0)
        << KData(&Srs19::CoastalCurrent, 0.1)
        << KData(&Srs19::LateralDispersionCoeff, 0)
        << KData(&Srs19::UseOkuboDispersionExp, true);
    _userInputs << dg1;
}

bool GenericCoastalTransport::calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult)
{
    //user input parameters
    qreal x = loc.distance(ci);
    qreal y0 = _userInputs.numericValueOf(Srs19::ReleaseToBeachDistance);
    qreal y = _userInputs.numericValueOf(Srs19::DistanceFromShoreline);
    qreal D = _userInputs.numericValueOf(Srs19::WaterDepth);
    qreal U = _userInputs.numericValueOf(Srs19::CoastalCurrent);
    qreal Ey = _userInputs.numericValueOf(Srs19::LateralDispersionCoeff);
    bool okuboEq = _userInputs.valueOf(Srs19::UseOkuboDispersionExp).toBool();

    if (okuboEq) {
        Ey = 3.44e-7 * qPow(x/U, 1.34);
        _userInputs.replace(KData(&Srs19::LateralDispersionCoeff, Ey));
        xTrace() << *this << "Estimating Ey: " << Ey;
    }

    //add salt water flag
    (*calcResult) << KData(&Srs19::IsSaltWater, true);

    qreal cw;
    //const KData & qiW = inpQi.find(Srs19::WaterDischargeRate);
    KData qiW = _inpPorts.data(Srs19::WaterDischargeRate);
    DataItemArray cwList;
    if (Ey > 0.0) {
        (*calcResult) << KData(&Rad::CommentQuantity, QObject::tr("Using eq. VI-46, page 183."));
        for(int k = 0; k < qiW.count(); k++) {
            const KDataItem & qi = qiW.at(k);
            const KRadionuclide & rn = KStorage::storage()->radionuclide(qi.name());
            qreal l = rn.halfLife().decayConstant();

            //equation VI-46, page 183
            cw = (qi.numericValue() * qExp(((-U*(y-y0)*(y-y0)) / (4 * Ey * x)) - (l*x/U))) /
                (D * qSqrt(M_PI * U * Ey * x));

            //add to result
            cwList << KDataItem(qi.name(), cw, KData::Radionuclide);
        }
    }
    else {
        (*calcResult) << KData(&Rad::CommentQuantity, QObject::tr("Ey not available, using eq. VI-52, page 184."));
        for(int k = 0; k < qiW.count(); k++) {
            const KDataItem & qi = qiW.at(k);
            const KRadionuclide & rn = KStorage::storage()->radionuclide(qi.name());
            qreal l = rn.halfLife().decayConstant();

            //equation VI-52, page 184
            cw = (962 * qPow(U, 0.17) * qi.numericValue() * qExp(((-7.28e+5*qPow(U,2.34)*(y-y0)*(y-y0)) / qPow(x,2.34)) - (l*x/U))) /
                (D * qPow(x, 1.17));

            //add to result
            cwList << KDataItem(qi.name(), cw, KData::Radionuclide);
        }
    }

    if (!cwList.isEmpty()) {
        (*calcResult) << KData(&Srs19::TotalConcentrationInWater, cwList);
    }

    return true;
}
bool GenericCoastalTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;
    int result = doVerification(&err, &warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return result;
}

bool GenericCoastalTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool GenericCoastalTransport::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
