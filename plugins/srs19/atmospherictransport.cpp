#include "atmospherictransport.h"
#include "quantity.h"
#include "radcore.h"

/////////////////////////////////////////////////////////////////////////////////////////
AtmosphericTransport::AtmosphericTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}
/**
 * @brief Create output ports
 * @return
 */
bool AtmosphericTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::AtmosphericDischargeRate, KPort::Input);

    _outPorts << new KPort(this, &Srs19::ConcentrationInAir, KPort::Output)
              << new KPort(this, &Srs19::DailyDepositionRate, KPort::Output);

    return true;
}

void AtmosphericTransport::defineParameters()
{
    KDataGroupArray * ui = userInputs();
    DataGroup dg1(QObject::tr("Wind parameters"));
    dg1 << KData(&Srs19::FractionOfWind, KData::Numeric | KData::Mandatory, 0.25)
        << KData(&Srs19::GeometricMeanOfWindSpeed, KData::Numeric | KData::Mandatory, 2);
    ui->append(dg1);

    DataGroup dg2(QObject::tr("Ground deposition"));
    dg2 << KData(&Srs19::DryDepositionCoeff, KData::Numeric | KData::Mandatory, 500)
        << KData(&Srs19::WetDepositionCoeff, KData::Numeric | KData::Mandatory, 500);
    ui->append(dg2);

    DataGroup dg3(QObject::tr("Building parameter"));
    dg3 << KData(&Srs19::BuildingHeight, KData::Numeric | KData::ConditionalMandatory, 0)
        << KData(&Srs19::CrossSectionalArea, KData::Numeric | KData::ConditionalMandatory, 0);
    ui->append(dg3);

    DataGroup dg4(QObject::tr("Cavity zone"));
    dg4 << KData(&Srs19::BuildingWidth, 0)
        << KData(&Srs19::OnSameBuilding, false);
    ui->append(dg4);

    //carbon and tritium
    DataGroup dg5(QObject::tr("Atmosphere characteristics"));
    dg5 << KData(&Srs19::AbsoluteAirHumidity)
        << KData(&Srs19::AirborneCarbonConcentration);
    ui->append(dg5);

    //parameter control
    KQuantityControl qc(&Srs19::OnSameBuilding, false);
    qc.append(&Srs19::BuildingWidth);
    ui->addQuantityControl(qc);
}

bool AtmosphericTransport::verify(int * oerr, int * owarn)
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

    KData daH = _inpPorts.data(Srs19::ReleaseHeight);
    if (!daH.isValid() || daH.numericValue() <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReleaseHeight);
        err++;
    }

    KDataGroupArray * ui = userInputs();
    const KData & daPp = ui->find(Srs19::FractionOfWind);
    if (!daPp.isValid() || daPp.numericValue() <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::FractionOfWind);
        err ++;
    }

    const KData & daUa = ui->find(Srs19::GeometricMeanOfWindSpeed);
    if (!daUa.isValid() || daUa.numericValue() <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::GeometricMeanOfWindSpeed);
        err ++;
    }

    //check for user input parameters
    qreal H, x, HB;
    x = this->location().distance();
    H = daH.numericValue();

    //get building height
    const KData & daHB = ui->find(Srs19::BuildingHeight);
    if (!daHB.isValid()) {
        KOutputProxy::errorNotSpecified(this, Srs19::BuildingHeight);
        err ++;
    }
    else {
        HB = daHB.numericValue();
        if (H <= 2.5 * HB) {
            const KData & daAB = ui->find(Srs19::CrossSectionalArea);
            if (!daAB.isValid()) {
                KOutputProxy::errorNotSpecified(this, Srs19::CrossSectionalArea);
                err++;
            }

            qreal sqAB = qSqrt(daAB.numericValue());
            if (x <= 2.5 * sqAB) {
                //get flow rate and stack diameter
                bool sameBuilding = ui->valueOf(Srs19::OnSameBuilding).toBool();
                if (sameBuilding) {
                    //KData daStack = daInput.find(Srs19::Diameter);
                    KData daStack = _inpPorts.data(Srs19::Diameter);
                    if (!daStack.isValid() || daStack.numericValue() <= 0) {
                        KOutputProxy::errorNotSpecified(this, Srs19::Diameter);
                        err++;
                    }

                    qreal sd = daStack.numericValue();
                    if (x <= 3 * sd) {
                        //KData daFlow = daInput.find(Srs19::AirFlowRate);
                        KData daFlow = _inpPorts.data(Srs19::AirFlowRate);
                        if (!daFlow.isValid() || daFlow.numericValue() <= 0) {
                            KOutputProxy::errorNotSpecified(this, Srs19::AirFlowRate);
                            err++;
                        }
                    }
                }
            }
        }
    }

    const KData & vfd = ui->find(Srs19::DryDepositionCoeff);
    if (!vfd.isValid() || vfd.numericValue() <= 0) {
        KOutputProxy::warningNotProperlyDefined(this, Srs19::DryDepositionCoeff);
        warn ++;
    }
    const KData & vfw = ui->find(Srs19::WetDepositionCoeff);
    if (!vfw.isValid() || vfw.numericValue() <= 0) {
        KOutputProxy::warningNotProperlyDefined(this, Srs19::WetDepositionCoeff);
        warn ++;
    }

    KOutputProxy::infoVerificationResult(this, err, warn);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

qreal AtmosphericTransport::calcModifiedConcentration(
        const QString& nuc, qreal x, qreal ua, KRadionuclide * radinuclide)
{
    //reduction factor for short lived radionuclide
    //eq. 9, page. 26
    qreal f = 1.0;
    const KRadionuclide & nuclide = factory()->storage()->radionuclide(nuc);
    //if (nuclide.isVeryShortLived()) {
    bool isShortLived = _inpPorts.data(Srs19::IsShortLiveNuclide).value().toBool();
    if (isShortLived) {
        qreal l = nuclide.halfLife().decayConstant();
        f = qExp((-l * x)/ua);
    }
    *radinuclide = nuclide;
    return f;
}

qreal AtmosphericTransport::calcSigmaZ(qreal x, qreal H, KDataArray * calcResult)
{
    qreal sz, E, G;

    //SRS-19 page. 19
    if (H <= 45) {
        sz = (0.06 * x) / qSqrt(1 + (0.0015*x));
    }
    else if (H <= 80) {
        E = 0.215;
        G = 0.885;
        sz = E * qPow(x, G);

        (*calcResult) << KData(&Srs19::SigmaFitParameterE, E)
                  << KData(&Srs19::SigmaFitParameterG, G);
    }
    else {
        E = 0.265;
        G = 0.818;
        sz = E * qPow(x, G);

        (*calcResult) << KData(&Srs19::SigmaFitParameterE, E)
                  << KData(&Srs19::SigmaFitParameterG, G);
    }
    (*calcResult) << KData(&Srs19::VerticalDiffusionParameter, sz);

    return sz;
}

qreal AtmosphericTransport::calcF(qreal x, qreal H, KDataArray * calcResult)
{
    //vertical diffusion parameter
    qreal sz = calcSigmaZ(x, H, calcResult);

    //SRS-19, page. 18, equation. 3
    qreal F = (12.0 * qExp(-(H*H)/(2*sz*sz))) / (x*sz*qSqrt(2*M_PI*M_PI*M_PI));
    (*calcResult) << KData(&Srs19::HeightGaussianDiffusionFactor, F);

    return F;
}
qreal AtmosphericTransport::calcB(qreal x, qreal H, qreal AB, KDataArray * calcResult)
{
    //eq. 6, page. 21
    qreal sz = calcSigmaZ(x, H, calcResult);
    qreal Sz = qSqrt((sz*sz) + (AB/M_PI));
    (*calcResult) << KData(&Srs19::CorrectedVerticalDiffusionParameter, Sz);

    //eq. 5, page. 21
    qreal B = 12.0 / (x * Sz * qSqrt(2 * M_PI * M_PI * M_PI));
    (*calcResult) << KData(&Srs19::LeeGaussianDiffusionFactor, B);

    return B;
}

void AtmosphericTransport::calcDisplacementZone(qreal x, qreal H, KDataArray * calcResult)
{
    KDataGroupArray * ui = userInputs();
    qreal Pp = ui->numericValueOf(Srs19::FractionOfWind);
    qreal ua = ui->numericValueOf(Srs19::GeometricMeanOfWindSpeed);
    qreal Vd = ui->numericValueOf(Srs19::DryDepositionCoeff);
    qreal Vw = ui->numericValueOf(Srs19::WetDepositionCoeff);
    qreal F = calcF(x, H, calcResult);
    KData qiA = _inpPorts.data(Srs19::AtmosphericDischargeRate);

    qreal di;
    DataItemArray diList;

    qreal f;
    DataItemArray fList;

    //storing nuclide
    KRadionuclide nuclide;

    //calculate concentration
    //use SRS-19, Eq. 2 (page 18)
    DataItemArray caList;
    for(int k = 0; k < qiA.count(); k++) {
        const KDataItem & qi = qiA.at(k);
        qreal ca = (Pp * F * qi.numericValue()) / ua;

        //modified ca?
        f = calcModifiedConcentration(qi.name(), x, ua, &nuclide);
        if (f < 1.0) {
            ca *= f;
            fList << KDataItem(qi.name(), f, KData::Radionuclide);
        }

        //add to result
        caList << KDataItem(qi.name(), ca, KData::Radionuclide);

        //ground deposition rate
        if (nuclide.isDepositedInGround()) {
            di = (Vd + Vw) * ca;
            diList << KDataItem(qi.name(), di, KData::Radionuclide);
        }
    }

    //reduction factor
    if (!fList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ReductionFactor, fList);
    }

    //concentration
    if (!caList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ConcentrationInAir, caList);
    }

    //ground deposition rate
    if (!diList.isEmpty()) {
        (*calcResult) << KData(&Srs19::DailyDepositionRate, diList);
    }
}

void AtmosphericTransport::calcWakeZone(qreal x, qreal H, qreal AB, KDataArray * calcResult)
{
    KDataGroupArray * ui = userInputs();
    qreal Pp = ui->numericValueOf(Srs19::FractionOfWind);
    qreal ua = ui->numericValueOf(Srs19::GeometricMeanOfWindSpeed);
    qreal Vd = ui->numericValueOf(Srs19::DryDepositionCoeff);
    qreal Vw = ui->numericValueOf(Srs19::WetDepositionCoeff);
    qreal B = calcB(x, H, AB, calcResult);
    KData  qiA = _inpPorts.data(Srs19::AtmosphericDischargeRate);

    qreal di;
    DataItemArray diList;

    qreal f;
    DataItemArray fList;

    //storing nuclide
    KRadionuclide nuclide;

    //calculate concentration
    //use SRS-19, Eq. 2 (page 18)
    DataItemArray caList;
    for(int k = 0; k < qiA.count(); k++) {
        const KDataItem & qi = qiA.at(k);
        qreal ca = (Pp * B * qi.numericValue()) / ua;

        //modified ca?
        f = calcModifiedConcentration(qi.name(), x, ua, &nuclide);
        if (f < 1.0) {
            ca *= f;
            fList << KDataItem(qi.name(), f, KData::Radionuclide);
        }

        //add to result
        caList << KDataItem(qi.name(), ca, KData::Radionuclide);

        //ground deposition rate
        if (nuclide.isDepositedInGround()) {
            di = (Vd + Vw) * ca;
            diList << KDataItem(qi.name(), di, KData::Radionuclide);
        }
    }

    //reduction factor
    if (!fList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ReductionFactor, fList);
    }

    //concentration
    if (!caList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ConcentrationInAir, caList);
    }

    //ground deposition rate
    if (!diList.isEmpty()) {
        (*calcResult) << KData(&Srs19::DailyDepositionRate, diList);
    }
}

void AtmosphericTransport::calcCavityZone(qreal x, qreal HB, bool onSameBuilding,
                                          KDataArray * calcResult)
{
    KDataGroupArray * ui = userInputs();
    qreal Pp = ui->numericValueOf(Srs19::FractionOfWind);
    qreal ua = ui->numericValueOf(Srs19::GeometricMeanOfWindSpeed);
    qreal Vd = ui->numericValueOf(Srs19::DryDepositionCoeff);
    qreal Vw = ui->numericValueOf(Srs19::WetDepositionCoeff);

    qreal WB = ui->numericValueOf(Srs19::BuildingWidth);
    KData qiA = _inpPorts.data(Srs19::AtmosphericDischargeRate);
    qreal d = _inpPorts.data(Srs19::Diameter).numericValue();
    qreal V = _inpPorts.data(Srs19::AirFlowRate).numericValue();

    //for eq. 8, select width or height
    HB = (WB > 0 && WB < HB) ? WB : HB;
    qreal K = 1;

    qreal di;
    DataItemArray diList;

    qreal f, B0 = 0;
    DataItemArray fList;

    //storing nuclide
    KRadionuclide nuclide;

    //calculate concentration
    //use SRS-19, Eq. 2 (page 18)
    DataItemArray caList;
    for(int k = 0; k < qiA.count(); k++) {
        const KDataItem & qi = qiA.at(k);
        qreal ca;
        if (onSameBuilding) {
            //page 24, 3.6.1 (a)

            if (x <= 3.0 * d) {
                //use eq.1
                ca = (Pp * qi.numericValue()) / V;
            }
            //page 24, 3.6.1 (b)
            else {
                //use eq. 7
                if (B0 == 0) {
                    B0 = 30;
                    (*calcResult) << KData(&Srs19::AirConcentrationIncrease, B0);
                }
                ca = (B0 * qi.numericValue()) / (ua * x * x);
            }
        }
        else {
            //use eq. 8
            ca = (Pp * qi.numericValue()) / (M_PI * ua * HB * K);
        }

        //modified ca?
        f = calcModifiedConcentration(qi.name(), x, ua, &nuclide);
        if (f < 1.0) {
            ca *= f;
            fList << KDataItem(qi.name(), f, KData::Radionuclide);
        }

        //add to result
        caList << KDataItem(qi.name(), ca, KData::Radionuclide);

        //ground deposition rate
        if (nuclide.isDepositedInGround()) {
            di = (Vd + Vw) * ca;
            diList << KDataItem(qi.name(), di, KData::Radionuclide);
        }
    }

    //reduction factor
    if (!fList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ReductionFactor, fList);
    }

    //concentration
    if (!caList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ConcentrationInAir, caList);
    }

    //ground deposition rate
    if (!diList.isEmpty()) {
        (*calcResult) << KData(&Srs19::DailyDepositionRate, diList);
    }
}

bool AtmosphericTransport::calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult)
{
    //user input parameters
    qreal x = loc.distance(ci);
    KDataGroupArray * ui = userInputs();
    bool onSameBuilding = ui->valueOf(Srs19::OnSameBuilding).toBool();
    qreal HB = ui->numericValueOf(Srs19::BuildingHeight);
    qreal AB = ui->numericValueOf(Srs19::CrossSectionalArea);
    qreal H = _inpPorts.data(Srs19::ReleaseHeight).numericValue();

    qreal HB25 = 2.5 * HB;
    qreal sqAB25 = 2.5 * qSqrt(AB);

    /*
    if (onSameBuilding || (H <= HB25 && x <= sqAB25)) {
        //cavity zone
        calcCavityZone(x, HB, onSameBuilding, inpQi);
    }
    else if (H <= HB25 && x > sqAB25) {
        //wake zone
        calcWakeZone(x, H, AB, inpQi);
    }
    else {
        //displacement zone
        calcDisplacementZone(x, H, inpQi);
    }
    */

    //check same building
    if (onSameBuilding && (H > HB25 || x > sqAB25)) {
        //warning
        xWarning() << *this << QObject::tr("Source and receptor are on the same building, but H > 2.5Hb or x > sqrt(Ab)");
    }

    if (H > HB25) {
        //displacement zone
        (*calcResult) << KData(&Srs19::CommentQuantity, QObject::tr("Dispersion in the lee of an isolated point source, "
                                                            "H > 2.5Hb (Refer to Eq. 2, pp. 18)"))
                  << KData(&Srs19::DownwindDistance, x);
        calcDisplacementZone(x, H, calcResult);
    }
    else if (x > sqAB25) {
        //wake zone
        (*calcResult) << KData(&Srs19::CommentQuantity, QObject::tr("Dispersion in the lee of a building inside the wake zone, "
                                                            "H <= 2.5Hb and x > 2.5 sqrt(Ab) (Refer to Eq. 4, pp. 20)."))
                  << KData(&Srs19::DownwindDistance, x);
        calcWakeZone(x, H, AB, calcResult);
    }
    else {
        //cavity zone
        if (onSameBuilding) {
            //add comment
            (*calcResult) << KData(&Srs19::CommentQuantity, QObject::tr("Dispersion in the lee of a building inside the cavity zone, "
                                                                "H <= 2.5Hb and x <= sqrt(Ab), source and receptor on same "
                                                                "building surface (Refer to Eq. 7, pp. 24)"));
        }
        else {
            //add comment
            (*calcResult) << KData(&Srs19::CommentQuantity, QObject::tr("Dispersion in the lee of a building inside the cavity zone, "
                                                                "H <= 2.5Hb and x <= sqrt(Ab), source and receptor *not* on same "
                                                                "building surface (Refer to Eq. 8, pp. 24)"));
        }
        (*calcResult) << KData(&Srs19::DownwindDistance, x);
        calcCavityZone(x, HB, onSameBuilding, calcResult);
    }

    return true;
}
