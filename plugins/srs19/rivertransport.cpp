#include <QtCore/qmath.h>
#include "rivertransport.h"
#include "kport.h"
#include "koutput.h"
#include "symbol.h"
#include "kgroup.h"
#include "kdata.h"
#include "kmath.h"
#include "kstorage.h"
#include "kradionuclide.h"

RiverTransport::RiverTransport(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
    DataGroup dg1(QObject::tr("River conditions"));
    dg1 << KData(&Srs19::RiverEstuaryWidth, 0)
        << KData(&Srs19::LowRiverFlowRate, 0)
        << KData(&Srs19::NetFreshwaterVelocity, 0)
        << KData(&Srs19::FlowDepth, 0);
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Parameter estimation"));
    dg2 << KData(&Srs19::EstimateParameters, false)
        << KData(&Srs19::TidalRiverWidth, 0);
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Receptor position"));
    dg3 << KData(&Srs19::ReceptorOnOpposite, false);
    _userInputs << dg3;
}

/**
 * @brief Create output ports
 * @return
 */
bool RiverTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::WaterDischargeRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Output);

    return true;
}

void RiverTransport::estimateParameters()
{
    bool estimate = _userInputs.valueOf(Srs19::EstimateParameters).toBool();
    if (!estimate)
        return;

    qreal Bd = _userInputs.numericValueOf(Srs19::TidalRiverWidth);
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
void RiverTransport::calcualteConcentration(qreal x, qreal qr, qreal U, const KDataArray& inpQi, qreal pr)
{
    const KData & qiW = inpQi.find(Srs19::WaterDischargeRate);
    QVector<KDataItem> cwList;
    QVector<KDataItem> ctList;
    for(int k = 0; k < qiW.count(); k++) {
        const KDataItem & qi = qiW.at(k);
        const KRadionuclide & rn = KStorage::storage()->radionuclide(qi.name());
        qreal l = rn.halfLife().decayConstant();

        // maximum concentration
        qreal ct = (qi.numericValue() * qExp(-(l*x)/U))/qr;

        //equation 12, page 35.
        qreal cw = pr * ct;

        //add to result
        ctList << KDataItem(qi.name(), ct);
        cwList << KDataItem(qi.name(), cw);
    }

    if (!cwList.isEmpty()) {
        _dataList << KData(&Srs19::OppositeBankMaxConcentration, ctList);
        _dataList << KData(&Srs19::TotalConcentrationInWater, cwList);
    }
}

bool RiverTransport::calculate(const KCalculationInfo& ci)
{
    _dataList.clear();

    //check for distance
    KLocationPort * lp = locationPort();
    if (lp == 0)
        return false;

    //estimate parameter
    estimateParameters();

    //input parameter
    KDataArray inpQi = ci.input();

    //user input parameters
    qreal x = lp->location().distance();
    qreal B = _userInputs.numericValueOf(Srs19::RiverEstuaryWidth);
    qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
    qreal U = _userInputs.numericValueOf(Srs19::NetFreshwaterVelocity);
    qreal D = _userInputs.numericValueOf(Srs19::FlowDepth);
    qreal Lz = 7 * D;

    //add downind distance
    _dataList << KData(&Srs19::LongitudinalDistance, x);

    bool isOpposite = _userInputs.valueOf(Srs19::ReceptorOnOpposite).toBool();
    xTrace() << "Receptor is opposite? " << (int)isOpposite;

    if (isOpposite) {
        calcualteConcentration(x, qr, U, inpQi);
    }
    else if (x > Lz) {
        qreal A = (1.5 * D * x) / (B * B);
        qreal pr = KMath::pr(A);

        //must be added to result
        _dataList << KData(&Srs19::MixingCorrectionFactor, pr);

        calcualteConcentration(x, qr, U, inpQi, pr);
    }
    else {
        //should be calculated at release modul
        //CO already calculated.
        const KData& C0 = inpQi.find(Srs19::EffluentRadionuclideConcentration);
        if (C0.isValid()) {
            _dataList << KData(&Srs19::TotalConcentrationInWater, C0);
        }
        else {
            xWarning() << *this << QObject::tr("Concentration in water C0 is not provided by discharge source.");
        }
    }
    return true;
}

bool RiverTransport::verify(int * oerr, int * owarn)
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
    }

    xInfo() << tagName() << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool RiverTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool RiverTransport::save(QIODevice * i)
{
    Q_UNUSED(i);
    return true;
}

