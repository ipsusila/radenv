#include "sedimenteffect.h"
#include "quantity.h"
#include "radcore.h"
#include "coastaltransport.h"

SedimentEffect::SedimentEffect(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf), _kdValues(fact)
{
}

QString SedimentEffect::displayText() const
{
    return QObject::tr("Sediment");
}

void SedimentEffect::defineParameters()
{
    //define user inputs
    KDataGroupArray * ui = userInputs();
    DataGroup dg1(QObject::tr("Sediment parameters"));
    dg1 << KData(&Srs19::EffShoreAccumulationTime, 3.15e+7)
        << KData(&Srs19::SuspendedSedimentConcentration, defaultSuspendedSedimentConcentration())
        << KData(&Srs19::LongCommentQuantity, "If value of suspended sediment concentration not known, "
                 "left it to 0. The value of 1x10^-2 kg/m3 for coastal water, 5x10^-2 kg/m3 for others "
                 "will be used for calculation.");
    ui->append(dg1);

    DataGroup dg2(QObject::tr("Distribution coefficient"));
    dg2 << KData(&Srs19::UseDefaultValue, true)
        << KData(&Srs19::SedimentDistributionCoeff, KData::RadionuclideArray, QVariant());
    ui->append(dg2);

    //parameter control
    KQuantityControl qc(&Srs19::UseDefaultValue, false);
    qc.append(&Srs19::SedimentDistributionCoeff);
    ui->addQuantityControl(qc);
}

bool SedimentEffect::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Input);
    _outPorts << new KPort(this, &Srs19::ConcentrationInFilteredWater, KPort::Output)
              << new KPort(this, &Srs19::ConcentrationInSuspendedSediment, KPort::Output)
              << new KPort(this, &Srs19::ConcentrationInBottomSediment, KPort::Output)
              << new KPort(this, &Srs19::ConcentrationInShoreSediment, KPort::Output);

    return true;
}

bool SedimentEffect::needLocation() const
{
    return false;
}

QRectF SedimentEffect::modelRect() const
{
    return QRectF(-55, -50, 110, 100);
}

bool SedimentEffect::calculate(const KCalculationInfo &ci, const KLocation & loc, KDataArray * calResult)
{
    Q_UNUSED(ci);
    Q_UNUSED(loc);

    //Kd values for specific Radionuclide
    //add to input
    KDataGroupArray * ui = userInputs();
    qreal Ss = ui->numericValueOf(Srs19::SuspendedSedimentConcentration);
    qreal Te = ui->numericValueOf(Srs19::EffShoreAccumulationTime);
    bool useDefaultKd = ui->valueOf(Srs19::UseDefaultValue).toBool();

    //check for default value
    if (Ss <= 0) {
        Ss = defaultSuspendedSedimentConcentration();
        ui->replace(KData(&Srs19::SuspendedSedimentConcentration, Ss));
    }
    if (Te <= 0) {
        Te = 3.15e+7;
        ui->replace(KData(&Srs19::EffShoreAccumulationTime, Te));
    }

    //get cwtot
    //use concentration in shoreline for beach/large lake model
    KData Cw = _inpPorts.data(Srs19::TotalConcentrationInWater);
    if (!Cw.isValid()) {
        Cw = _inpPorts.data(Srs19::ConcentrationInShoreline);
    }

    if (useDefaultKd) {
        DataItemArray kdItems;
        for(int k = 0; k < Cw.count(); k++) {
            const KDataItem & cwItem = Cw.at(k);
            qreal Kd = _kdValues.value(cwItem.name(), waterType());
            calculate(cwItem, Ss, Te, Kd, calResult);
            kdItems << KDataItem(cwItem.name(), Kd, KData::Radionuclide);
        }
        ui->replace(KData(&Srs19::SedimentDistributionCoeff, kdItems));
    }
    else {
        const KData& KdList = ui->find(Srs19::SedimentDistributionCoeff);
        for(int k = 0; k < Cw.count(); k++) {
            const KDataItem & cwItem = Cw.at(k);
            qreal Kd = KdList.numericValue(cwItem.name());
            calculate(cwItem, Ss, Te, Kd, calResult);
        }
    }

    return true;
}

void SedimentEffect::calculate(const KDataItem & cwItem, qreal Ss, qreal Te, qreal Kd, KDataArray * result)
{
    const KRadionuclide & rn = KStorage::storage()->radionuclide(cwItem.name());
    qreal l = rn.halfLife().decayConstant();
    qreal Cwtot = cwItem.numericValue();
    qreal lTe = (1-qExp(-l*Te)) / (l * Te);

    qreal Cws = Cwtot / (1 + 0.001 * Kd * Ss);
    qreal Csw = 0.001 * Kd * Cws;
    qreal Csb = 0.1 * Csw * lTe;
    qreal Css = 60 * Csb;

    QString nuc = rn.nuclide();
    result->appendOrMerge(&Srs19::ConcentrationInFilteredWater, nuc, Cws, KData::Radionuclide);
    result->appendOrMerge(&Srs19::ConcentrationInSuspendedSediment, nuc, Csw, KData::Radionuclide);
    result->appendOrMerge(&Srs19::ConcentrationInBottomSediment, nuc, Csb, KData::Radionuclide);
    result->appendOrMerge(&Srs19::ConcentrationInShoreSediment, nuc, Css, KData::Radionuclide);
}

bool SedimentEffect::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this);
        err ++;
    }

    KDataGroupArray * ui = userInputs();
    qreal Te = ui->numericValueOf(Srs19::EffShoreAccumulationTime);
    if (Te <= 0) {
        KOutputProxy::infoUseDefaultValue(this, Srs19::EffShoreAccumulationTime);
    }

    qreal Ss = ui->numericValueOf(Srs19::SuspendedSedimentConcentration);
    if (Ss <= 0) {
        KOutputProxy::infoUseDefaultValue(this, Srs19::SuspendedSedimentConcentration);
    }

    //load default kd from databases
    //TODO
    //check default flag/no
    bool useDefaultKd = ui->valueOf(Srs19::UseDefaultValue).toBool();
    if (useDefaultKd) {
        if (!_kdValues.load(true)) {
            KOutputProxy::errorLoadFailed(this, Srs19::SedimentDistributionCoeff);
            err ++;
        }
    }
    else {
        const KData & Kd = ui->find(Srs19::SedimentDistributionCoeff);
        if (Kd.isEmpty()) {
            KOutputProxy::errorNotSpecified(this, Srs19::SedimentDistributionCoeff);
            err++;
        }
    }


    KOutputProxy::infoVerificationResult(this, err, warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

FreshwaterSediment::FreshwaterSediment(IModelFactory * fact, const KModelInfo& inf)
    : SedimentEffect(fact, inf)
{
}

qreal FreshwaterSediment::defaultSuspendedSedimentConcentration() const
{
    return 5e-2;
}
int FreshwaterSediment::waterType() const
{
    return DistributionCoefficient::Freshwater;
}
CoastalSediment::CoastalSediment(IModelFactory * fact, const KModelInfo& inf)
    : SedimentEffect(fact, inf)
{
}

qreal CoastalSediment::defaultSuspendedSedimentConcentration() const
{
    return 1e-2;
}
int CoastalSediment::waterType() const
{
    return DistributionCoefficient::SaltWater;
}

