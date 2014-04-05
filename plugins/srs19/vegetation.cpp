#include "vegetation.h"
#include "widgetvegetation.h"
#include "symbol.h"
#include "radcore.h"

Vegetation::Vegetation(IModelFactory * fact, const KModelInfo& inf)
    : FoodChain(fact, inf), _fvValues(fact)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool Vegetation::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::DailyDepositionRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInVegetation, KPort::Output);

    return true;
}
/*
IUserInput * Vegetation::createUserInputWidget(QWidget *parent)
{
    WidgetVegetation * w =
            new WidgetVegetation(&_userInputs, parent);
    return w;

    return 0;
}
*/

bool Vegetation::calculate(const KCalculationInfo &ci, const KLocation & loc, KDataArray * calResult)
{
    Q_UNUSED(ci);
    Q_UNUSED(loc);

    KData di2;
    KData di1 = _inpPorts.data(Srs19::DailyDepositionRate);
    if (di1.isValid()) {
        di2 = di1;
    }
    else {
        di1 = _inpPorts.data(Srs19::AverageDepositionRate);
        di2 = _inpPorts.data(Srs19::AnnualAverageDepositionRate);
    }

    if (di1.count() != di2.count())  {
        xTrace() << *this << "Deposition rate 1 and 2 do not have equal count.";
        return false;
    }

    qreal a = _userInputs.numericValueOf(Srs19::MassInterceptionFactor);
    qreal lw = _userInputs.numericValueOf(Srs19::ReductionOnSurface);
    qreal te = _userInputs.numericValueOf(Srs19::CropExposedPeriod);
    qreal ls = _userInputs.numericValueOf(Srs19::ReductionOnSoil);
    qreal p = _userInputs.numericValueOf(Srs19::SurfaceSoilDensity);
    qreal tb = _inpPorts.data(Srs19::DischargePeriod).numericValue() * 365;
    qreal th = _userInputs.numericValueOf(Srs19::IntervalAfterHarvest);
    bool useDefaultFv = _userInputs.valueOf(Rad::UseDefaultValue).toBool();

    if (useDefaultFv) {
        DataItemArray fvItems;
        for(int k = 0; k < di1.count(); k++) {
            const KDataItem & d1 = di1.at(k);
            const KDataItem & d2 = di2.at(d1.name());
            qreal Fv = _fvValues.value(d1.name(), vegetationType());
            fvItems << KDataItem(d1.name(), Fv, KData::Radionuclide);
            calculate(d1, d2, Fv, a, p, lw, ls, te, tb, th, calResult);
        }

        _userInputs.replace(KData(&Srs19::UptakeConcentrationFactor, fvItems));
    }
    else {
        //uptake concentration factor
        KData FvUser = _userInputs.find(Srs19::UptakeConcentrationFactor);
        for(int k = 0; k < di1.count(); k++) {
            const KDataItem & d1 = di1.at(k);
            const KDataItem & d2 = di2.at(d1.name());
            qreal Fv = FvUser.numericValue(d1.name());
            calculate(d1, d2, Fv, a, p, lw, ls, te, tb, th, calResult);
        }
    }

    return true;
}

void Vegetation::calculate(const KDataItem& d1, const KDataItem& d2, qreal Fv, qreal a, qreal p,
                           qreal lw, qreal ls, qreal te, qreal tb, qreal th, KDataArray * calcResult)
{
    QString nuc = d1.name();
    const KRadionuclide & rn = KStorage::storage()->radionuclide(nuc);
    qreal l = rn.halfLife().decayConstant(KHalfLife::Day);
    qreal lEv = l + lw;
    qreal lEs = l + ls;

    qreal Cvi1 = d1.numericValue() * a * (1 - qExp(-lEv * te)) / lEv;
    qreal Cs = d2.numericValue() * (1 - qExp(-lEs * tb)) / (p * lEs);
    qreal Cvi2 = Fv * Cs;

    //total vegetation concentration
    qreal Cvi = (Cvi1 + Cvi2) * qExp(-l * th);

    //add to result
    calcResult->appendOrMerge(&Srs19::ConsDueDirectContamination, nuc, Cvi1, KData::Radionuclide);
    calcResult->appendOrMerge(&Srs19::ConcentrationInDrySoil, nuc, Cs, KData::Radionuclide);
    calcResult->appendOrMerge(&Srs19::ConsDueIndirectProcess, nuc, Cvi2, KData::Radionuclide);
    calcResult->appendOrMerge(&Srs19::ConcentrationInVegetation, nuc, Cvi, KData::Radionuclide);
}

bool Vegetation::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }

    qreal a = _userInputs.numericValueOf(Srs19::MassInterceptionFactor);
    if (a < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::MassInterceptionFactor);
        err++;
    }
    qreal lw = _userInputs.numericValueOf(Srs19::ReductionOnSurface);
    if (lw < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReductionOnSurface);
        err++;
    }
    qreal te = _userInputs.numericValueOf(Srs19::CropExposedPeriod);
    if (te < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::CropExposedPeriod);
        err++;
    }
    qreal ls = _userInputs.numericValueOf(Srs19::ReductionOnSoil);
    if (ls < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReductionOnSoil);
        err++;
    }
    qreal p = _userInputs.numericValueOf(Srs19::SurfaceSoilDensity);
    if (p < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::SurfaceSoilDensity);
        err++;
    }

    qreal th = _userInputs.numericValueOf(Srs19::IntervalAfterHarvest);
    if (th < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::IntervalAfterHarvest);
        err++;
    }

    if (!_fvValues.load(true)) {
        KOutputProxy::errorLoadFailed(this, Srs19::UptakeConcentrationFactor);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

bool Vegetation::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool Vegetation::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}

//////crops
Crop::Crop(IModelFactory *fact, const KModelInfo &inf) : Vegetation(fact, inf)
{
}
TransferFactor::ValueType Crop::vegetationType() const
{
    return TransferFactor::Crops;
}
void Crop::defineParameters()
{

    //TODO: loss-rate
    //define user inputs
    //default values from page 64-66
    //Peat (lahan gambut)

    //define user inputs
    DataGroup dg1(QObject::tr("Vegetation"));
    dg1 << KData(&Rad::NameSymbol, "Crop");
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Transport parameter"));
    dg2 << KData(&Srs19::MassInterceptionFactor, 0.3)
        << KData(&Srs19::ReductionOnSurface, 0.05)
        << KData(&Srs19::CropExposedPeriod, 60)
        << KData(&Srs19::ReductionOnSoil, 0.0014)
        << KData(&Rad::CommentSymbol, QObject::tr("Note: value for anions. For Cs and Sr use 0.00014 d-1 instead, all other nuclides set to 0 d-1."))
        << KData(&Srs19::SurfaceSoilDensity, 260)
        << KData(&Rad::CommentSymbol, QObject::tr("Note: for peat soils use 100 kg/m2 instead."))
        << KData(&Srs19::IntervalAfterHarvest, 14);
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Transfer factor"));
    dg3 << KData(&Rad::UseDefaultValue, true)
        << KData(&Srs19::UptakeConcentrationFactor, KData::RadionuclideArray, QVariant());
    _userInputs << dg3;
}

//////Forage
Forage::Forage(IModelFactory *fact, const KModelInfo &inf) : Vegetation(fact, inf)
{

}

TransferFactor::ValueType Forage::vegetationType() const
{
    return TransferFactor::Forage;
}
void Forage::defineParameters()
{
    //define user inputs
    //default values from page 64-66
    //Peat (lahan gambut)
    //define user inputs
    DataGroup dg1(QObject::tr("Vegetation"));
    dg1 << KData(&Rad::NameSymbol, "Forage");
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Transport parameters"));
    dg2 << KData(&Srs19::MassInterceptionFactor, 3)
        << KData(&Srs19::ReductionOnSurface, 0.05)
        << KData(&Srs19::CropExposedPeriod, 30)
        << KData(&Srs19::ReductionOnSoil, 0.0014)
        << KData(&Rad::CommentSymbol, QObject::tr("Note: value for anions. For Cs and Sr use 0.00014 d-1 instead, all other nuclides set to 0 d-1."))
        << KData(&Srs19::SurfaceSoilDensity, 130)
        << KData(&Rad::CommentSymbol, QObject::tr("Note: for peat soils use 50 kg/m2 instead."))
        << KData(&Srs19::IntervalAfterHarvest, 0);

    DataGroup dg3(QObject::tr("Transfer factor"));
    dg3 << KData(&Rad::UseDefaultValue, true)
        << KData(&Srs19::UptakeConcentrationFactor, KData::RadionuclideArray, QVariant());
    _userInputs << dg3;
}

