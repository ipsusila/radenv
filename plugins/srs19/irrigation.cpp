#include "irrigation.h"
#include "quantity.h"
#include "radcore.h"

Irrigation::Irrigation(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}
bool Irrigation::needLocation() const
{
    return false;
}

/**
 * @brief Create output ports
 * @return
 */
bool Irrigation::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::TotalConcentrationInWater, KPort::Input);
    _outPorts << new KPort(this, &Srs19::AverageDepositionRate, KPort::Output);

    return true;
}
void Irrigation::defineParameters()
{
    //define user inputs
    DataGroup dg1(QObject::tr("Irrigation parameters"));
    dg1 << KData(&Srs19::WaterDailyIrrigation)
        << KData(&Srs19::IrrigationDays)
        << KData(&Srs19::AnnualIrrigationPeriod);
    userInputs()->append(dg1);
}

bool Irrigation::calculate(const KCalculationInfo& ci, const KLocation & loc, KDataArray * calcResult)
{
    Q_UNUSED(loc);
    Q_UNUSED(ci);

    KDataGroupArray * ui = userInputs();
    KData inpCw = _inpPorts.data(Srs19::TotalConcentrationInWater);
    qreal wd = ui->numericValueOf(Srs19::WaterDailyIrrigation);
    int Nd = ui->valueOf(Srs19::IrrigationDays).toInt();
    int Td = ui->valueOf(Srs19::AnnualIrrigationPeriod).toInt();
    qreal Iw = wd * Nd / (Td * 1000);   //1000 is conversion from Litre to m^3
    qreal Iwa = Iw * Td / 365;

    calcResult->append(KData(&Srs19::IrrigationRate, Iw));
    calcResult->append(KData(&Srs19::AnnualIrrigationRate, Iwa));

    for (int k = 0; k < inpCw.count(); k++) {
        const KDataItem & item = inpCw.at(k);
        qreal cw = item.numericValue();
        QString nuc = item.name();

        calcResult->appendOrMerge(&Srs19::AverageDepositionRate, nuc, Iw * cw, KData::Radionuclide);
        calcResult->appendOrMerge(&Srs19::AnnualAverageDepositionRate, nuc, Iwa * cw, KData::Radionuclide);
    }

    return true;
}

bool Irrigation::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this, _inpPorts.first());
        err ++;
    }

    KDataGroupArray * ui = userInputs();
    qreal wd = ui->numericValueOf(Srs19::WaterDailyIrrigation);
    if (wd <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::WaterDailyIrrigation);
        err ++;
    }

    int nd = ui->valueOf(Srs19::AnnualIrrigationPeriod).toInt();
    if (nd <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::AnnualIrrigationPeriod);
        err ++;
    }

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}


