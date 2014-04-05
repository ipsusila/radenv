#include "rivertransport.h"
#include "symbol.h"
#include "radcore.h"

RiverTransport::RiverTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool RiverTransport::allocateIoPorts()
{
    //allocate input/output
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Output);

    return true;
}

void RiverTransport::defineParameters()
{
    //define user inputs
    DataGroup dg1(QObject::tr("River conditions (site specific)"));
    dg1 << KData(&Srs19::RiverEstuaryWidth, 0)
        << KData(&Srs19::FlowDepth, 0)
        << KData(&Srs19::LowRiverFlowRate, 0)
        << KData(&Srs19::NetFreshwaterVelocity, 0)
        << KData(&Rad::CommentSymbol, QObject::tr("If U not known, value will be calculated with U=qr/BD."));
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Parameter estimation"));
    dg2 << KData(&Srs19::EstimateParameters, false)
        << KData(&Srs19::EstimatedRiverWidth, 0);
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Receptor position"));
    dg3 << KData(&Srs19::ReceptorOnOpposite, false);
    _userInputs << dg3;
}

void RiverTransport::estimateParameters()
{
    bool estimate = _userInputs.valueOf(Srs19::EstimateParameters).toBool();
    if (!estimate)
        return;

    qreal Bd = _userInputs.numericValueOf(Srs19::EstimatedRiverWidth);
    xTrace() << "Estimating parameter with Bd=" << Bd;

    //SRS-19 page 168
    //B = 10*qr^0.460
    //log B = log 10 + 0.460*log qr
    //(log B - 1) / 0.460 = log qr
    //qr = 10 ^ ()
    qreal qrd = qPow(10.0, (log10(Bd) - 1.0) / 0.460);
    qreal qr = qrd / 3;
    qreal B = 10.0 * qPow(qr, 0.460);
    qreal D = 0.163 * qPow(qr, 0.447);
    qreal U = qr / (B * D);

    //save parameters
    KDataGroupArray::iterator it = _userInputs.begin();
    KDataGroupArray::iterator end = _userInputs.end();
    while(it != end) {
        DataList::iterator iit = it->items.begin();
        DataList::iterator iend = it->items.end();
        while (iit != iend) {
            if (iit->symbol() == Srs19::RiverEstuaryWidth)
                iit->setValue(B);
            else if (iit->symbol() == Srs19::LowRiverFlowRate)
                iit->setValue(qr);
            else if (iit->symbol() == Srs19::NetFreshwaterVelocity)
                iit->setValue(U);
            else if (iit->symbol() == Srs19::FlowDepth)
                iit->setValue(D);
            iit++;
        }
        it++;
    }
}
void RiverTransport::calcualteConcentration(qreal x, qreal qr, qreal U, KDataArray * calcResult, qreal pr)
{
    KData qiW = _inpPorts.data(Srs19::WaterDischargeRate);
    DataItemArray cwList;
    DataItemArray ctList;
    for(int k = 0; k < qiW.count(); k++) {
        const KDataItem & qi = qiW.at(k);
        const KRadionuclide & rn = KStorage::storage()->radionuclide(qi.name());
        qreal l = rn.halfLife().decayConstant();

        // maximum concentration
        qreal ct = (qi.numericValue() * qExp(-(l*x)/U))/qr;

        //equation 12, page 35.
        qreal cw = pr * ct;

        //add to result
        ctList << KDataItem(qi.name(), ct, KData::Radionuclide);
        cwList << KDataItem(qi.name(), cw, KData::Radionuclide);
    }

    if (!cwList.isEmpty()) {
        (*calcResult) << KData(&Srs19::OppositeBankMaxConcentration, ctList);
        (*calcResult) << KData(&Srs19::TotalConcentrationInWater, cwList);
    }
}

bool RiverTransport::calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult)
{
    Q_UNUSED(ci);

    //estimate parameter
    estimateParameters();

    //user input parameters
    qreal x = loc.distance();
    qreal B = _userInputs.numericValueOf(Srs19::RiverEstuaryWidth);
    qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
    qreal D = _userInputs.numericValueOf(Srs19::FlowDepth);
    qreal U = _userInputs.numericValueOf(Srs19::NetFreshwaterVelocity);
    if (U <= 0.0) {
        U = qr / (B * D);
        _userInputs.replace(KData(&Srs19::NetFreshwaterVelocity, U));
    }
    qreal Lz = 7 * D;

    //add longitudinal distance
    (*calcResult) << KData(&Srs19::LongitudinalDistance, x);
    (*calcResult) << KData(&Srs19::CompleteMixingDistance, Lz);

    bool isOpposite = _userInputs.valueOf(Srs19::ReceptorOnOpposite).toBool();
    if (isOpposite) {
        calcualteConcentration(x, qr, U, calcResult);
    }
    else if (x > Lz) {
        qreal A = (1.5 * D * x) / (B * B);
        qreal pr = KMath::pr(A);

        //must be added to result
        (*calcResult) << KData(&Srs19::PartialMixingIndex, A);
        (*calcResult) << KData(&Srs19::MixingCorrectionFactor, pr);

        calcualteConcentration(x, qr, U, calcResult, pr);
    }
    else {
        //should be calculated at release modul
        //CO already calculated.
        //const KData& C0 = inpQi.find(Srs19::EffluentRadionuclideConcentration);
        KData C0 = _inpPorts.data(Srs19::EffluentRadionuclideConcentration);
        if (C0.isValid()) {
            (*calcResult) << KData(&Srs19::TotalConcentrationInWater, C0);
        }
        else {
            xWarning() << *this << QObject::tr("Concentration in water C0 is not provided by discharge source.");
        }
    }
    return true;
}

bool RiverTransport::doVerification(int * oerr, int * owarn)
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

    KData daF = _inpPorts.data(Srs19::LiquidFlowRate);
    if (!daF.isValid() || daF.numericValue() <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::LiquidFlowRate);
        err++;
    }

    bool estimate = _userInputs.valueOf(Srs19::EstimateParameters).toBool();
    if (estimate) {
        qreal Bd = _userInputs.numericValueOf(Srs19::EstimatedRiverWidth);
        if (Bd <= 0) {
            KOutputProxy::errorNotSpecified(this, Srs19::EstimatedRiverWidth);
            err++;
        }
    }
    else {
        //ceck all parameter
        qreal B = _userInputs.numericValueOf(Srs19::RiverEstuaryWidth);
        qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
        qreal U = _userInputs.numericValueOf(Srs19::NetFreshwaterVelocity);
        qreal D = _userInputs.numericValueOf(Srs19::FlowDepth);

        if (B <= 0) {
            KOutputProxy::errorNotSpecified(this, Srs19::RiverEstuaryWidth);
            err++;
        }

        if (qr <= 0) {
            KOutputProxy::errorNotSpecified(this, Srs19::LowRiverFlowRate);
            err++;
        }

        if (U <= 0) {
            //KOutputProxy::errorNotSpecified(this, Srs19::NetFreshwaterVelocity);
            //KOutputProxy::warningNotProperlyDefined(this, Srs19::NetFreshwaterVelocity);
            //warn++;
            KOutputProxy::infoUseDefaultValue(this, Srs19::NetFreshwaterVelocity);
        }

        if (D <= 0) {
            KOutputProxy::errorNotSpecified(this, Srs19::FlowDepth);
            err++;
        }
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool RiverTransport::verify(int * oerr, int * owarn)
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

bool RiverTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool RiverTransport::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}

