#include "animaltransport.h"
#include "radcore.h"
#include "quantity.h"

AnimalTransport::AnimalTransport(IModelFactory * fact, const KModelInfo& inf)
    : FoodChain(fact, inf), _Fmf(fact)
{
}


/**
 * @brief Create output ports
 * @return
 */
bool AnimalTransport::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::ConcentrationInAnimalFeed, KPort::Input)
              << new KPort(this, &Srs19::ConcentrationInWater, KPort::Input);

    _outPorts << new KPort(this, outQuantity(), KPort::Output);

    return true;
}

bool AnimalTransport::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    bool useDefault = _userInputs.valueOf(Rad::UseDefaultValue).toBool();
    KData Cai = _inpPorts.data(0, Srs19::ConcentrationInAnimalFeed);
    KData Cwi = _inpPorts.data(1, Srs19::ConcentrationInWater);
    qreal Qw = _userInputs.numericValueOf(Srs19::WaterConsumption);
    qreal Qmf = _userInputs.numericValueOf(*feedingAmountQuantity());
    qreal Tmf = _userInputs.numericValueOf(*consumptionDelayQuantity());

    if (useDefault) {
        DataItemArray FmItems;
        for (int k = 0; k < Cai.count(); k++) {
            const KDataItem & CaItem = Cai.at(k);
            qreal cwi = Cwi.numericValue(CaItem.name());
            FvValue Fm = _Fmf.value(CaItem.name());
            qreal fmf;
            if (feedingAmountQuantity() == &Srs19::MilkAnimalFeed) {
                fmf = Fm.milk;
            }
            else {
                fmf = Fm.meat;
            }
            FmItems << KDataItem(CaItem.name(), fmf, KData::Radionuclide);
            calculate(CaItem, cwi, fmf, Qmf, Qw, Tmf, calcResult);
        }
        _userInputs.replace(KData(uptakeRateQuantity(), FmItems));
    }
    else {
        const KData & Fm = _userInputs.find(*uptakeRateQuantity());
        for (int k = 0; k < Cai.count(); k++) {
            const KDataItem & CaItem = Cai.at(k);
            qreal cwi = Cwi.numericValue(CaItem.name());
            qreal fmf = Fm.numericValue(CaItem.name());
            calculate(CaItem, cwi, fmf, Qmf, Qw, Tmf, calcResult);
        }
    }

    return true;
}

void AnimalTransport::calculate(const KDataItem & CaItem, qreal cwi,
                                qreal Fmf, qreal Qmf, qreal Qw, qreal Tmf, KDataArray * result)
{
    const KRadionuclide & rn = KStorage::storage()->radionuclide(CaItem.name());
    qreal l = rn.halfLife().decayConstant(KHalfLife::Day);
    qreal Cmf = Fmf * (CaItem.numericValue() * Qmf + cwi * Qw) * qExp(-l*Tmf);
    result->appendOrMerge(this->outQuantity(), CaItem.name(), Cmf, KData::Radionuclide);
}

bool AnimalTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }
    if (!_inpPorts.isConnected(1)) {
        KOutputProxy::warningMessage(this, _inpPorts.at(1)->quantity()->symbol + QObject::tr(" not connected."));
        warn++;
        //KOutputProxy::war
        //err ++;
    }

    qreal Qmf = _userInputs.numericValueOf(*feedingAmountQuantity());
    if (Qmf <= 0) {
        KOutputProxy::errorNotSpecified(this, *feedingAmountQuantity());
        err++;
    }

    qreal Tmf = _userInputs.numericValueOf(*consumptionDelayQuantity());
    if (Tmf < 0) {
        KOutputProxy::errorNotSpecified(this, *consumptionDelayQuantity());
        err ++;
    }

    qreal qw = _userInputs.numericValueOf(Srs19::WaterConsumption);
    if (qw < 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::WaterConsumption);
        err++;
    }

    //uptake rate
    bool useDefault = _userInputs.valueOf(Rad::UseDefaultValue).toBool();
    if (useDefault) {
        if (!_Fmf.load(true)) {
            KOutputProxy::errorLoadFailed(this, *uptakeRateQuantity());
            err ++;
        }
    }
    else {
        const KData & fmf = _userInputs.find(*uptakeRateQuantity());
        if (fmf.isEmpty()) {
            KOutputProxy::errorNotSpecified(this, *uptakeRateQuantity());
            err++;
        }
    }

    //TODO other verification

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

bool AnimalTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool AnimalTransport::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}

////Milk
MilkTransport::MilkTransport(IModelFactory *fact, const KModelInfo &inf)
    : AnimalTransport(fact, inf)
{
}
void MilkTransport::defineParameters()
{
    //define user inputs
    DataGroup dg1(QObject::tr("Animal"));
    dg1 << KData(&Rad::NameQuantity, "Milk animal");
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Feeding parameters"));
    dg2 << KData(&Srs19::MilkAnimalFeed, 16)
        << KData(&Srs19::WaterConsumption, 0.06)
        << KData(&Rad::LongCommentQuantity, QObject::tr("For small animal suach as sheep and goats, "
                                            "if specific value is unknown, 0.006 should be used."));
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Consumption parameter"));
    dg3 << KData(&Srs19::IntervalAfterMilking, 1);
    _userInputs << dg3;

    DataGroup dg4(QObject::tr("Transfer factor"));
    dg4 << KData(&Rad::UseDefaultValue, true)
        << KData(&Srs19::FractionIntakeAtMilking, KData::RadionuclideArray, QVariant());
    _userInputs << dg4;

    //parameter control
    KQuantityControl qc(&Rad::UseDefaultValue, false);
    qc.append(&Srs19::FractionIntakeAtMilking);
    _userInputs.addQuantityControl(qc);
}

const Quantity * MilkTransport::feedingAmountQuantity() const
{
    return &Srs19::MilkAnimalFeed;
}

const Quantity * MilkTransport::consumptionDelayQuantity() const
{
    return &Srs19::IntervalAfterMilking;
}
const Quantity * MilkTransport::outQuantity() const
{
    return &Srs19::ConcentrationInMilk;
}
const Quantity * MilkTransport::uptakeRateQuantity() const
{
    return &Srs19::FractionIntakeAtMilking;
}

////Meat
MeatTransport::MeatTransport(IModelFactory *fact, const KModelInfo &inf)
    : AnimalTransport(fact, inf)
{

}

void MeatTransport::defineParameters()
{
    //define user inputs
    DataGroup dg1(QObject::tr("Animal"));
    dg1 << KData(&Rad::NameQuantity, "Meat animal");
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Feeding parameters"));
    dg2 << KData(&Srs19::MeatAnimalFeed, 12)
        << KData(&Srs19::WaterConsumption, 0.04)
        << KData(&Rad::LongCommentQuantity, QObject::tr("For small animal suach as sheep and goats, "
                                        "if specific value is unknown, 0.004 should be used."));
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Consumption parameter"));
    dg3 << KData(&Srs19::IntervalAfterSlaughter, 20);
    _userInputs << dg3;

    DataGroup dg4(QObject::tr("Transfer factor"));
    dg4 << KData(&Rad::UseDefaultValue, true)
        << KData(&Srs19::FractionIntakeAtSlaughter, KData::RadionuclideArray, QVariant());
    _userInputs << dg4;

    //parameter control
    KQuantityControl qc(&Rad::UseDefaultValue, false);
    qc.append(&Srs19::FractionIntakeAtSlaughter);
    _userInputs.addQuantityControl(qc);

}
const Quantity * MeatTransport::feedingAmountQuantity() const
{
    return &Srs19::MeatAnimalFeed;
}

const Quantity * MeatTransport::consumptionDelayQuantity() const
{
    return &Srs19::IntervalAfterSlaughter;
}

const Quantity * MeatTransport::outQuantity() const
{
    return &Srs19::ConcentrationInMeat;
}
const Quantity * MeatTransport::uptakeRateQuantity() const
{
    return &Srs19::FractionIntakeAtSlaughter;
}

