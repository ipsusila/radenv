#include "storedforage.h"
#include "radcore.h"
#include "quantity.h"

StoredForage::StoredForage(IModelFactory * fact, const KModelInfo& inf)
    : FoodChain(fact, inf)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool StoredForage::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::ConcentrationInVegetation, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInStoredAnimalFeed, KPort::Output);

    return true;
}
void StoredForage::defineParameters()
{
    //define user inputs
    KDataGroupArray * ui = userInputs();
    DataGroup dg1(QObject::tr("Vegetation"));
    dg1 << KData(&Srs19::NameQuantity, "Stored feed");
    ui->append(dg1);

    DataGroup dg2(QObject::tr("Storing parameter"));
    dg2 << KData(&Srs19::IntervalAfterHarvest, 90);
    ui->append(dg2);
}

bool StoredForage::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    KDataGroupArray * ui = userInputs();
    qreal th = ui->numericValueOf(Srs19::IntervalAfterHarvest);
    KData Cvi1 = _inpPorts.data(Srs19::ConsDueDirectContamination);
    KData Cvi2 = _inpPorts.data(Srs19::ConsDueIndirectProcess);

    if (!Cvi1.isValid() || !Cvi2.isValid() || Cvi1.count() != Cvi2.count()) {
        KOutputProxy::errorMessage(this, QObject::tr("Vegetation concentration due to direct "
            "and/or indirect contamination value Cv,i,1 and Cv,i,2 are not valid."));
        return false;
    }

    for(int k = 0; k < Cvi1.count(); k++) {
        const KDataItem & Cvi1Item = Cvi1.at(k);
        const KRadionuclide & rn = factory()->storage()->radionuclide(Cvi1Item.name());
        qreal l = rn.halfLife().decayConstant(KHalfLife::Day);
        qreal vCvi1 = Cvi1Item.numericValue();
        qreal vCvi2 = Cvi2.numericValue(rn.nuclide());

        qreal Cpi = (vCvi1 + vCvi2) * qExp(-l * th);
        calcResult->appendOrMerge(&Srs19::ConcentrationInStoredAnimalFeed,
                        rn.nuclide(), Cpi, KData::Radionuclide);
    }

    return true;
}

bool StoredForage::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }

    qreal th = userInputs()->numericValueOf(Srs19::IntervalAfterHarvest);
    if (th <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::IntervalAfterHarvest);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

