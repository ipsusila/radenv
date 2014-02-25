#include <QtCore/qmath.h>
#include "atmospherictransport.h"
#include "kport.h"
#include "koutput.h"
#include "symbol.h"
#include "dialogatmospherictransport.h"
#include "kgroup.h"
#include "kstorage.h"
#include "kradionuclide.h"

/////////////////////////////////////////////////////////////////////////////////////////
AtmosphericTransport::AtmosphericTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
    DataGroup dg1(QObject::tr("Dispersion parameters"));
    dg1 << KData(&Srs19::FractionOfWind, 0.25)
        << KData(&Srs19::GeometricMeanOfWindSpeed, 2)
        << KData(&Srs19::BuildingHeight, 0)
        << KData(&Srs19::CrossSectionalArea, 0)
        << KData(&Srs19::DryDepositionCoeff, 500)
        << KData(&Srs19::WetDepositionCoeff, 500);
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Cavity zone"));
    dg2 << KData(&Srs19::BuildingWidth, 0)
        << KData(&Srs19::OnSameBuilding, false);
    _userInputs << dg2;

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

bool AtmosphericTransport::verify(int * oerr, int * owarn)
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
    KData daH = daInput.find(Srs19::ReleaseHeight);
    if (!daH.isValid() || daH.numericValue() <= 0) {
        xError() << *this << QObject::tr("Release height not specified.");
        err++;
    }

    KData daPp = _userInputs.find(Srs19::FractionOfWind);
    if (!daPp.isValid() || daPp.numericValue() <= 0) {
        xError() << *this << QObject::tr("Parameter [wind fraction] not set properly.");
        err ++;
    }

    KData daUa = _userInputs.find(Srs19::GeometricMeanOfWindSpeed);
    if (!daUa.isValid() || daUa.numericValue() <= 0) {
        xError() << *this << QObject::tr("Parameter [geometric mean of wind speed] not set properly.");
        err ++;
    }

    //check for user input parameters
    qreal H, x, HB;
    x = lp->location().distance();
    H = daH.numericValue();

    //get building height
    KData daHB = _userInputs.find(Srs19::BuildingHeight);
    if (!daHB.isValid()) {
        xError() << *this << QObject::tr("Building height not specified.");
        err ++;
    }
    else {
        HB = daHB.numericValue();
        if (H <= 2.5 * HB) {
            KData daAB = _userInputs.find(Srs19::CrossSectionalArea);
            if (!daAB.isValid()) {
                xError() << *this << QObject::tr("Building's cross sectional area not specified.");
                err++;
            }

            qreal sqAB = qSqrt(daAB.numericValue());
            if (x <= 2.5 * sqAB) {
                //get flow rate and stack diameter
                bool sameBuilding = _userInputs.valueOf(Srs19::OnSameBuilding).toBool();
                if (sameBuilding) {
                    KData daStack = daInput.find(Srs19::Diameter);
                    if (!daStack.isValid() || daStack.numericValue() <= 0) {
                        xError() << *this << QObject::tr("Stack diameter not set properly for cavity zone calculation.");
                        err++;
                    }

                    qreal sd = daStack.numericValue();
                    if (x <= 3 * sd) {
                        KData daFlow = daInput.find(Srs19::AirFlowRate);
                        if (!daFlow.isValid() || daFlow.numericValue() <= 0) {
                            xError() << *this << QObject::tr("Air flow rate not set properly for cavity zone calculation.");
                            err++;
                        }
                    }
                }
            }
        }
    }

    KData vf = _userInputs.find(Srs19::DryDepositionCoeff);
    if (!vf.isValid() || vf.numericValue() <= 0) {
        xWarning() << *this << QObject::tr("Parameter [dry deposition coeff.] not set properly.");
        warn ++;
    }
    vf = _userInputs.find(Srs19::WetDepositionCoeff);
    if (!vf.isValid() || vf.numericValue() <= 0) {
        xWarning() << *this << QObject::tr("Parameter [wet deposition coeff.] not set properly.");
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

qreal AtmosphericTransport::calcModifiedConcentration(
        const QString& nuc, qreal x, qreal ua, KRadionuclide * radinuclide)
{
    //reduction factor for short lived radionuclide
    //eq. 9, page. 26
    qreal f = 1.0;
    const KRadionuclide & nuclide = KStorage::storage()->radionuclide(nuc);
    if (nuclide.isVeryShortLived()) {
        qreal l = nuclide.halfLife().decayConstant();
        f = qExp((-l * x)/ua);
    }
    *radinuclide = nuclide;
    return f;
}

qreal AtmosphericTransport::calcSigmaZ(qreal x, qreal H)
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

        _dataList << KData(&Srs19::SigmaFitParameterE, E)
                  << KData(&Srs19::SigmaFitParameterG, G);
    }
    else {
        E = 0.265;
        G = 0.818;
        sz = E * qPow(x, G);

        _dataList << KData(&Srs19::SigmaFitParameterE, E)
                  << KData(&Srs19::SigmaFitParameterG, G);
    }
    _dataList << KData(&Srs19::VerticalDiffusionParameter, sz);

    return sz;
}

qreal AtmosphericTransport::calcF(qreal x, qreal H)
{
    //vertical diffusion parameter
    qreal sz = calcSigmaZ(x, H);

    //SRS-19, page. 18, equation. 3
    qreal F = (12.0 * qExp(-(H*H)/(2*sz*sz))) / (x*sz*qSqrt(2*M_PI*M_PI*M_PI));
    _dataList << KData(&Srs19::HeightGaussianDiffusionFactor, F);

    return F;
}
qreal AtmosphericTransport::calcB(qreal x, qreal H, qreal AB)
{
    //eq. 6, page. 21
    qreal sz = calcSigmaZ(x, H);
    qreal Sz = qSqrt((sz*sz) + (AB/M_PI));
    _dataList << KData(&Srs19::CorrectedVerticalDiffusionParameter, Sz);

    //eq. 5, page. 21
    qreal B = 12.0 / (x * sz * qSqrt(2 * M_PI * M_PI * M_PI));
    _dataList << KData(&Srs19::LeeGaussianDiffusionFactor, B);

    return B;
}

void AtmosphericTransport::calcDisplacementZone(qreal x, qreal H, const KDataArray& inpQi)
{
    qreal Pp = _userInputs.numericValueOf(Srs19::FractionOfWind);
    qreal ua = _userInputs.numericValueOf(Srs19::GeometricMeanOfWindSpeed);
    qreal Vd = _userInputs.numericValueOf(Srs19::DryDepositionCoeff);
    qreal Vw = _userInputs.numericValueOf(Srs19::WetDepositionCoeff);
    qreal F = calcF(x, H);
    const KData & qiA = inpQi.find(Srs19::AtmosphericDischargeRate);

    qreal di;
    QVector<KDataItem> diList;

    qreal f;
    QVector<KDataItem> fList;

    //storing nuclide
    KRadionuclide nuclide;

    //calculate concentration
    //use SRS-19, Eq. 2 (page 18)
    QVector<KDataItem> caList;
    for(int k = 0; k < qiA.count(); k++) {
        const KDataItem & qi = qiA.at(k);
        qreal ca = (Pp * F * qi.numericValue()) / ua;

        //modified ca?
        f = calcModifiedConcentration(qi.name(), x, ua, &nuclide);
        if (f < 1.0) {
            ca *= f;
            fList << KDataItem(qi.name(), f);
        }

        //add to result
        caList << KDataItem(qi.name(), ca);

        //ground deposition rate
        if (!nuclide.isNobleGas()) {
            di = (Vd + Vw) * ca;
            diList << KDataItem(qi.name(), di);
        }
    }

    //reduction factor
    if (!fList.isEmpty()) {
        _dataList << KData(&Srs19::ReductionFactor, fList);
    }

    //concentration
    if (!caList.isEmpty()) {
        _dataList << KData(&Srs19::ConcentrationInAir, caList);
    }

    //ground deposition rate
    if (!diList.isEmpty()) {
        _dataList << KData(&Srs19::DailyDepositionRate, diList);
    }
}

void AtmosphericTransport::calcWakeZone(qreal x, qreal H, qreal AB, const KDataArray& inpQi)
{
    qreal Pp = _userInputs.numericValueOf(Srs19::FractionOfWind);
    qreal ua = _userInputs.numericValueOf(Srs19::GeometricMeanOfWindSpeed);
    qreal Vd = _userInputs.numericValueOf(Srs19::DryDepositionCoeff);
    qreal Vw = _userInputs.numericValueOf(Srs19::WetDepositionCoeff);
    qreal B = calcB(x, H, AB);
    const KData & qiA = inpQi.find(Srs19::AtmosphericDischargeRate);

    qreal di;
    QVector<KDataItem> diList;

    qreal f;
    QVector<KDataItem> fList;

    //storing nuclide
    KRadionuclide nuclide;

    //calculate concentration
    //use SRS-19, Eq. 2 (page 18)
    QVector<KDataItem> caList;
    for(int k = 0; k < qiA.count(); k++) {
        const KDataItem & qi = qiA.at(k);
        qreal ca = (Pp * B * qi.numericValue()) / ua;

        //modified ca?
        f = calcModifiedConcentration(qi.name(), x, ua, &nuclide);
        if (f < 1.0) {
            ca *= f;
            fList << KDataItem(qi.name(), f);
        }

        //add to result
        caList << KDataItem(qi.name(), ca);

        //ground deposition rate
        if (!nuclide.isNobleGas()) {
            di = (Vd + Vw) * ca;
            diList << KDataItem(qi.name(), di);
        }
    }

    //reduction factor
    if (!fList.isEmpty()) {
        _dataList << KData(&Srs19::ReductionFactor, fList);
    }

    //concentration
    if (!caList.isEmpty()) {
        _dataList << KData(&Srs19::ConcentrationInAir, caList);
    }

    //ground deposition rate
    if (!diList.isEmpty()) {
        _dataList << KData(&Srs19::DailyDepositionRate, diList);
    }
}

void AtmosphericTransport::calcCavityZone(qreal x, qreal HB, bool onSameBuilding, const KDataArray& inpQi)
{
    qreal Pp = _userInputs.numericValueOf(Srs19::FractionOfWind);
    qreal ua = _userInputs.numericValueOf(Srs19::GeometricMeanOfWindSpeed);
    qreal Vd = _userInputs.numericValueOf(Srs19::DryDepositionCoeff);
    qreal Vw = _userInputs.numericValueOf(Srs19::WetDepositionCoeff);

    qreal WB = _userInputs.numericValueOf(Srs19::BuildingWidth);
    const KData & qiA = inpQi.find(Srs19::AtmosphericDischargeRate);
    qreal d = inpQi.numericValueOf(Srs19::Diameter);
    qreal V = inpQi.numericValueOf(Srs19::AirFlowRate);

    //for eq. 8, select width or height
    HB = (WB > 0 && WB < HB) ? WB : HB;
    qreal K = 1;

    qreal di;
    QVector<KDataItem> diList;

    qreal f, B0 = 0;
    QVector<KDataItem> fList;

    //storing nuclide
    KRadionuclide nuclide;

    //calculate concentration
    //use SRS-19, Eq. 2 (page 18)
    QVector<KDataItem> caList;
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
                    _dataList << KData(&Srs19::AirConcentrationIncrease, B0);
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
            fList << KDataItem(qi.name(), f);
        }

        //add to result
        caList << KDataItem(qi.name(), ca);

        //ground deposition rate
        if (!nuclide.isNobleGas()) {
            di = (Vd + Vw) * ca;
            diList << KDataItem(qi.name(), di);
        }
    }

    //reduction factor
    if (!fList.isEmpty()) {
        _dataList << KData(&Srs19::ReductionFactor, fList);
    }

    //concentration
    if (!caList.isEmpty()) {
        _dataList << KData(&Srs19::ConcentrationInAir, caList);
    }

    //ground deposition rate
    if (!diList.isEmpty()) {
        _dataList << KData(&Srs19::DailyDepositionRate, diList);
    }
}

bool AtmosphericTransport::calculate(const KCalculationInfo& ci)
{
    //clear results
    _dataList.clear();

    //check for distance
    KLocationPort * lp = locationPort();
    if (lp == 0)
        return false;

    KDataArray inpQi = ci.input();  //input parameter

    //user input parameters
    qreal x = lp->location().distance();
    bool onSameBuilding = _userInputs.valueOf(Srs19::OnSameBuilding).toBool();
    qreal HB = _userInputs.numericValueOf(Srs19::BuildingHeight);
    qreal AB = _userInputs.numericValueOf(Srs19::CrossSectionalArea);
    qreal H = inpQi.numericValueOf(Srs19::ReleaseHeight);

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

    if (H > HB25) {
        //displacement zone
        _dataList << KData(&Rad::CommentSymbol, QObject::tr("Dispersion in the lee of an isolated point source, "
                                                            "H > 2.5Hb (Refer to Eq. 2, pp. 18)"))
                  << KData(&Srs19::DownwindDistance, x);
        calcDisplacementZone(x, H, inpQi);
    }
    else if (x > sqAB25) {
        //wake zone
        _dataList << KData(&Rad::CommentSymbol, QObject::tr("Dispersion in the lee of a building inside the wake zone, "
                                                            "H <= 2.5Hb and x > 2.5 sqrt(Ab) (Refer to Eq. 4, pp. 20)."))
                  << KData(&Srs19::DownwindDistance, x);
        calcWakeZone(x, H, AB, inpQi);
    }
    else {
        //cavity zone
        if (onSameBuilding) {
            //add comment
            _dataList << KData(&Rad::CommentSymbol, QObject::tr("Dispersion in the lee of a building inside the cavity zone, "
                                                                "H <= 2.5Hb and x <= sqrt(Ab), source and receptor on same "
                                                                "building surface (Refer to Eq. 7, pp. 24)"));
        }
        else {
            //add comment
            _dataList << KData(&Rad::CommentSymbol, QObject::tr("Dispersion in the lee of a building inside the cavity zone, "
                                                                "H <= 2.5Hb and x <= sqrt(Ab), source and receptor *not* on same "
                                                                "building surface (Refer to Eq. 8, pp. 24)"));
        }
        _dataList << KData(&Srs19::DownwindDistance, x);

        calcCavityZone(x, HB, onSameBuilding, inpQi);
    }

    return true;
}

bool AtmosphericTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool AtmosphericTransport::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
}
