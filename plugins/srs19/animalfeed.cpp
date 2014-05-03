#include "animalfeed.h"
#include "radcore.h"
#include "quantity.h"

AnimalFeed::AnimalFeed(IModelFactory * fact, const KModelInfo& inf)
    : FoodChain(fact, inf)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool AnimalFeed::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::ConcentrationInVegetation, KPort::Input)
              << new KPort(this, &Srs19::ConcentrationInStoredAnimalFeed, KPort::Input);

    _outPorts << new KPort(this, &Srs19::ConcentrationInAnimalFeed, KPort::Output);

    return true;
}
void AnimalFeed::defineParameters()
{
    //define user inputs
    KDataGroupArray * dga = userInputs();
    DataGroup dg1(QObject::tr("Vegetation"));
    dg1 << KData(&Srs19::NameQuantity, "Animal feed");
    dga->append(dg1);

    DataGroup dg2(QObject::tr("Feeding parameters"));
    dg2 << KData(&Srs19::AnnualPastureFraction, 0.7);
    dga->append(dg2);
}

bool AnimalFeed::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    qreal fp = userInputs()->numericValueOf(Srs19::AnnualPastureFraction);
    KData Cvi = _inpPorts.at(0)->data(Srs19::ConcentrationInVegetation);
    KData Cpi = _inpPorts.at(1)->data(Srs19::ConcentrationInStoredAnimalFeed);

    if (Cvi.isValid()) {
        for(int k = 0; k < Cvi.count(); k++) {
            const KDataItem & CviItem = Cvi.at(k);
            qreal vCvi = CviItem.numericValue();
            qreal vCpi = Cpi.numericValue(CviItem.name());

            qreal Cai = fp * vCvi + (1-fp)*vCpi;
            calcResult->appendOrMerge(&Srs19::ConcentrationInAnimalFeed,
                            CviItem.name(), Cai, KData::Radionuclide);
        }
    }
    else if (Cpi.isValid()) {
        for(int k = 0; k < Cpi.count(); k++) {
            const KDataItem & CpiItem = Cvi.at(k);
            qreal vCvi = Cvi.numericValue(CpiItem.name());
            qreal vCpi = CpiItem.numericValue();

            qreal Cai = fp * vCvi + (1-fp)*vCpi;
            calcResult->appendOrMerge(&Srs19::ConcentrationInAnimalFeed,
                            CpiItem.name(), Cai, KData::Radionuclide);
        }
    }

    return true;
}

bool AnimalFeed::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isAnyConnected()) {
        KOutputProxy::errorPortNotConnected(this, 0);
        err ++;
    }

    qreal fp = userInputs()->numericValueOf(Srs19::AnnualPastureFraction);
    if (fp < 0 || fp > 1) {
        KOutputProxy::errorNotSpecified(this, Srs19::AnnualPastureFraction);
        err++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}


