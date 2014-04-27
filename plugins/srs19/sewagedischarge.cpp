#include "sewagedischarge.h"
#include "quantity.h"
#include "radcore.h"

SewageDischarge::SewageDischarge(IModelFactory * fact, const KModelInfo& inf)
    : Discharge(fact, inf)
{
}
bool SewageDischarge::allocateIoPorts()
{
    _outPorts << new KPort(this, &Srs19::ConcentrationInWetSewage, KPort::Output);
    return true;
}
void SewageDischarge::defineParameters()
{
    //discharge period
    //define user inputs
    DataGroup dg1(QObject::tr("Sewage parameters"));
    dg1 << KData(&Srs19::DischargePeriod, 30)
        << KData(&Srs19::PersonSludgeProduction, 20)
        << KData(&Srs19::NumOfServedPerson, 100)
        << KData(&Srs19::SolidMaterialConcentration, 5);
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Sewage parameters"));
    dg2 << KData(&Srs19::SewageDischargeRate, KData::RadionuclideArray, QVariant());
    _userInputs << dg2;
}

/*
IUserInput * SewageDischarge::createUserInputWidget(QWidget *parent)
{
    WidgetSewageDischarge * w =
            new WidgetSewageDischarge(&_dataList, parent);
    return w;
}
*/
bool SewageDischarge::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //discharge period
    KData xd = _userInputs.find(Srs19::DischargePeriod);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::DischargePeriod);
        err ++;
    }

    xd = _userInputs.find(Srs19::SewageDischargeRate);
    if (!xd.isValid() || xd.count() == 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::SewageDischargeRate);
        err ++;
    }

    xd = _userInputs.find(Srs19::PersonSludgeProduction);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::PersonSludgeProduction);
        err ++;
    }

    xd = _userInputs.find(Srs19::NumOfServedPerson);
    if (!xd.isValid() || xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::NumOfServedPerson);
        err ++;
    }

    xd = _userInputs.find(Srs19::SolidMaterialConcentration);
    if (xd.numericValue() <= 0.0) {
        KOutputProxy::errorNotSpecified(this, Srs19::SolidMaterialConcentration);
        err++;
    }

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        KOutputProxy::warningLocationNotSpecified(this);
        warn ++;
    }
    else {
        //assign location
        _dataList.setLocation(lp->location());
    }

    KOutputProxy::infoVerificationResult(this, err, warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool SewageDischarge::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);

    qreal Sp = _userInputs.find(Srs19::PersonSludgeProduction).numericValue();
    qint32 Np = _userInputs.find(Srs19::NumOfServedPerson).value().toInt();
    qreal ps = _userInputs.numericValueOf(Srs19::SolidMaterialConcentration) / 100.0;

    //concentration
    const KData & daQi = _userInputs.find(Srs19::SewageDischargeRate);
    if (!daQi.isValid()) {
        KOutputProxy::errorNotSpecified(this, Srs19::SewageDischargeRate);
        return false;
    }

    qreal Ssl = Sp * Np;    //sludge production in wet weight
    _dataList.appendOrReplace(KData(&Srs19::AnnualSludgeProduction, Ssl));

    DataItemArray itemsDry, itemsWet;
    for(int k = 0; k < daQi.count(); k++) {
        const KDataItem & qi = daQi.at(k);
        qreal Csl = qi.numericValue() / Ssl;
        itemsDry << KDataItem(qi.name(), Csl, KData::Radionuclide);
        itemsWet << KDataItem(qi.name(), Csl * ps, KData::Radionuclide);
    }

    if (!itemsDry.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::ConcentrationInDrySewage, itemsDry));
    }
    if (!itemsWet.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::ConcentrationInWetSewage, itemsWet));
    }

    return true;
}


bool SewageDischarge::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool SewageDischarge::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}

