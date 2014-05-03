#include "aquaticfoodtransport.h"
#include "quantity.h"
#include "radcore.h"

AquaticFoodTransport::AquaticFoodTransport(IModelFactory * fact, const KModelInfo& inf)
    : FoodChain(fact, inf), bpValues(fact)
{
}

/**
 * @brief Create output ports
 * @return
 */
bool AquaticFoodTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInAquaticFood, KPort::Output);

    return true;
}
void AquaticFoodTransport::defineParameters()
{
    QString name;
    switch(fishType()) {
    case FreshwaterFish:
        name = QObject::tr("Freshwater fish");
        break;
    case MarineFish:
        name = QObject::tr("Marine fish");
        break;
    case MarineShellfish:
        name = QObject::tr("Marine shellfish");
        break;
    default:
        name = "Fish";
        break;
    }

    KDataGroupArray * dga = userInputs();
    //define user inputs
    DataGroup dg1(QObject::tr("Fish"));
    dg1 << KData(&Srs19::NameQuantity, name);
    dga->append(dg1);

    DataGroup dg2(QObject::tr("Aquatic food parameters"));
    dg2 << KData(&Srs19::UseDefaultValue, true)
        << KData(&Srs19::BioaccumulationFactor, KData::RadionuclideArray, QVariant());
    dga->append(dg2);

    //parameter control
    KQuantityControl qc(&Srs19::UseDefaultValue, false);
    qc.append(&Srs19::BioaccumulationFactor);
    dga->addQuantityControl(qc);
}

bool AquaticFoodTransport::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    KDataGroupArray * ui = userInputs();
    bool useDefault = ui->find(Srs19::UseDefaultValue).value().toBool();
    KData daCw = _inpPorts.data(Srs19::TotalConcentrationInWater);
    DataItemArray cafList;
    if (useDefault) {
        DataItemArray bpList;
        for (int k = 0; k < daCw.count(); k++) {
            const KDataItem & cw = daCw.at(k);
            qreal Bp = bpValues.value(cw.name(), fishType());
            qreal Caf = cw.numericValue() * Bp / 1000;
            cafList << KDataItem(cw.name(), Caf, KData::Radionuclide);
            bpList << KDataItem(cw.name(), Bp, KData::Radionuclide);
        }
        if (!bpList.isEmpty()) {
            ui->replace(KData(&Srs19::BioaccumulationFactor, bpList));
        }
    }
    else {
        const KData& daBp = ui->find(Srs19::BioaccumulationFactor);
        for(int k = 0; k < daCw.count(); k++) {
            const KDataItem & cw = daCw.at(k);
            qreal Bp = daBp.numericValue(cw.name());
            qreal Caf = cw.numericValue() * Bp / 1000;
            cafList << KDataItem(cw.name(), Caf, KData::Radionuclide);
        }
    }

    if (!cafList.isEmpty()) {
        (*calcResult) << KData(&Srs19::ConcentrationInAquaticFood, cafList);
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

    KDataGroupArray * ui = userInputs();
    bool useDefault = ui->valueOf(Srs19::UseDefaultValue).toBool();
    if (useDefault) {
        if (!bpValues.load(true)) {
            KOutputProxy::errorLoadFailed(this, Srs19::BioaccumulationFactor);
            err ++;
        }
    }
    else {
        const KData& Bp = ui->find(Srs19::BioaccumulationFactor);
        if (Bp.isEmpty()) {
            KOutputProxy::errorNotSpecified(this, Srs19::BioaccumulationFactor);
            err++;
        }
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

FreshwaterFishTransport::FreshwaterFishTransport(IModelFactory * fact, const KModelInfo& inf)
    : AquaticFoodTransport(fact, inf)
{
}
int FreshwaterFishTransport::fishType() const
{
    return AquaticFoodTransport::FreshwaterFish;
}

MarineFishTransport::MarineFishTransport(IModelFactory * fact, const KModelInfo& inf)
    : AquaticFoodTransport(fact, inf)
{
}
int MarineFishTransport::fishType() const
{
    return AquaticFoodTransport::MarineFish;
}

MarineShellfishTransport::MarineShellfishTransport(IModelFactory * fact, const KModelInfo& inf)
    : AquaticFoodTransport(fact, inf)
{
}
int MarineShellfishTransport::fishType() const
{
    return AquaticFoodTransport::MarineShellfish;
}



