#include "aquaticfoodtransport.h"
#include "radcore.h"
#include "symbol.h"

AquaticFoodTransport::AquaticFoodTransport(IModelFactory * fact, const KModelInfo& inf)
    : FoodChain(fact, inf)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool AquaticFoodTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input)
              << new KPort(this, &Srs19::DailyDepositionRate, KPort::Input);

    _outPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Output);

    return true;
}
void AquaticFoodTransport::defineParameters()
{
    //define user inputs
    DataGroup dg1(QObject::tr("River conditions"));
    dg1 << KData(&Srs19::LowRiverFlowRate, 0)
        << KData(&Srs19::EstimateParameters, false)
        << KData(&Srs19::EstimatedRiverWidth, 0);
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Lake parameters"));
    dg2 << KData(&Srs19::LakeSurfaceArea, 0)
        << KData(&Srs19::LakeVolume, 0);

    _userInputs << dg2;
}

bool AquaticFoodTransport::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    //estimate qr
    bool estimate = _userInputs.valueOf(Srs19::EstimateParameters).toBool();
    if (estimate) {
        qreal Bd = _userInputs.numericValueOf(Srs19::EstimatedRiverWidth);
        xTrace() << "Estimating parameter with Bd=" << Bd;

        //SRS-19 page 168
        //B = 10*qr^0.460
        //log B = log 10 + 0.460*log qr
        //(log B - 1) / 0.460 = log qr
        //qr = 10 ^ (...)
        qreal qrd = qPow(10.0, (log10(Bd) - 1.0) / 0.460);
        qreal qr = qrd / 3;
        _userInputs.replace(KData(&Srs19::LowRiverFlowRate, qr));
    }

    qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
    qreal Al = _userInputs.numericValueOf(Srs19::LakeSurfaceArea);
    qreal V = _userInputs.numericValueOf(Srs19::LakeVolume);
    qreal di = _inpPorts.data(1, Srs19::DailyDepositionRate).numericValue();
    qreal T = _inpPorts.data(Srs19::DischargePeriod).numericValue();
    qreal t = T * 365 * 24 * 60 * 60;

    qreal cw;
    KData qiW = _inpPorts.data(0, Srs19::WaterDischargeRate);
    DataItemArray cwList;
    DataItemArray qiaList;
    for(int k = 0; k < qiW.count(); k++) {
        const KDataItem & qi = qiW.at(k);
        const KRadionuclide & rn = KStorage::storage()->radionuclide(qi.name());
        qreal l = rn.halfLife().decayConstant();

        qreal qia = qi.numericValue() + ((3 * di * Al) / 86400);
        qiaList << KDataItem(qi.name(), qia, KData::Radionuclide);

        qreal c = qr/V + l;
        calcResult->appendOrMerge(&Srs19::LambdaConstant, qi.name(), c, KData::Radionuclide);
        if (c <= 1e-8) {
            //equation 23, page 49
            cw = (qia / (qr + l * V)) * (1.0 - qExp(-t * c));
        }
        else {
            //equation 23, page 49
            cw = qia / (qr + l * V);
        }

        //add to result
        cwList << KDataItem(qi.name(), cw, KData::Radionuclide);
    }

    if (!qiaList.isEmpty()) {
        (*calcResult) << KData(&Srs19::CombinedReleaseRate, qiaList);
    }

    if (!cwList.isEmpty()) {
        (*calcResult) << KData(&Srs19::TotalConcentrationInWater, cwList);
    }

    return true;
}

bool AquaticFoodTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }
    if (!_inpPorts.isConnected(1)) {
        KOutputProxy::warningMessage(this, _inpPorts.at(1)->symbol()->symbol + QObject::tr(" not connected."));
        warn++;
        //KOutputProxy::errorPortNotConnected(this, _inpPorts.at(1));
        //err ++;
    }

    //get qr
    bool estimate = _userInputs.valueOf(Srs19::EstimateParameters).toBool();
    if (estimate) {
        qreal Bd = _userInputs.numericValueOf(Srs19::EstimatedRiverWidth);
        if (Bd <= 0) {
            KOutputProxy::errorNotSpecified(this, Srs19::EstimatedRiverWidth);
            err ++;
        }
    }
    else {
        qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
        if (qr <= 0) {
            KOutputProxy::errorNotSpecified(this, Srs19::LowRiverFlowRate);
            err ++;
        }
    }

    qreal Al = _userInputs.numericValueOf(Srs19::LakeSurfaceArea);
    if (Al <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::LakeSurfaceArea);
        err ++;
    }

    qreal V = _userInputs.numericValueOf(Srs19::LakeVolume);
    if (V <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::LakeVolume);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

bool AquaticFoodTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool AquaticFoodTransport::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}


