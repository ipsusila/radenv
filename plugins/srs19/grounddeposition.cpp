#include "grounddeposition.h"
#include "quantity.h"
#include "radcore.h"

GroundDeposition::GroundDeposition(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}
bool GroundDeposition::needLocation() const
{
    return false;
}

/**
 * @brief Create output ports
 * @return
 */
bool GroundDeposition::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::DailyDepositionRate, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInSurfaceSoil, KPort::Output);

    return true;
}
void GroundDeposition::defineParameters()
{
    DataGroup dg(QObject::tr("Soil reduction"));
    dg << KData(&Rad::UseDefaultValue, true)
       << KData(&Srs19::ReductionOnSoil, KData::RadionuclideArray, QVariant());
    _userInputs << dg;

    //parameter control
    KQuantityControl qc(&Rad::UseDefaultValue, false);
    qc.append(&Srs19::ReductionOnSoil);
    _userInputs.addQuantityControl(qc);
}

bool GroundDeposition::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(ci);
    Q_UNUSED(loc);

    KData di = _inpPorts.data(Srs19::DailyDepositionRate);
    if (!di.isValid()) {
        di = _inpPorts.data(Srs19::AnnualAverageDepositionRate);
    }

    qreal tb = _inpPorts.data(Srs19::DischargePeriod).numericValue() * 365;
    bool useDefaultLs = _userInputs.valueOf(Rad::UseDefaultValue).toBool();

    if (tb <= 0) {
        tb = Srs19::DischargePeriod.defaultValue * 365;  //30 years
        _userInputs.replace(KData(&Srs19::DischargePeriod, Srs19::DischargePeriod.defaultValue));
    }

    qreal ls;
    DataItemArray cgrItems;
    if (useDefaultLs) {
        DataItemArray lsItems;
        for(int k = 0; k < di.count(); k++) {
            const KDataItem & d = di.at(k);
            const KRadionuclide & rn = KStorage::storage()->radionuclide(d.name());
            qreal l = rn.halfLife().decayConstant(KHalfLife::Day);

            //get default ls value
            bool isAnion = _inpPorts.data(Srs19::IsAnion).value().toBool();
            if (isAnion) {
                ls = 0.0014;
            }
            else if (rn.element() == KElement::Sr || rn.element() == KElement::Cs)   {
                ls = 0.00014;
            }
            else {
                ls = 0;
            }
            lsItems << KDataItem(d.name(), ls, KData::Radionuclide);

            //calculate concentration
            qreal lEs = l + ls;
            qreal Cgr = (d.numericValue() * (1 - qExp(-lEs * tb))) / lEs;
            cgrItems << KDataItem(d.name(), Cgr, KData::Radionuclide);

        }

        //replace values
        _userInputs.replace(KData(&Srs19::ReductionOnSoil, lsItems));
    }
    else {
        for(int k = 0; k < di.count(); k++) {
            const KDataItem & d = di.at(k);
            const KRadionuclide & rn = KStorage::storage()->radionuclide(d.name());
            qreal l = rn.halfLife().decayConstant(KHalfLife::Day);
            qreal ls = _userInputs.find(Srs19::ReductionOnSoil).numericValue(d.name());

            //calculate concentration
            qreal lEs = l + ls;
            qreal Cgr = (d.numericValue() * (1 - qExp(-lEs * tb))) / lEs;
            cgrItems << KDataItem(d.name(), Cgr, KData::Radionuclide);

        }
    }

    if (!cgrItems.isEmpty()) {
        (*calcResult) << KData(&Srs19::ConcentrationInSurfaceSoil, cgrItems);
    }

    return true;
}

bool GroundDeposition::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }

    qreal ls = _userInputs.numericValueOf(Srs19::ReductionOnSoil);
    if (ls < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::ReductionOnSoil);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

bool GroundDeposition::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool GroundDeposition::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}


