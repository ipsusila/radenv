#include "doseestimation.h"
#include "quantity.h"
#include "radcore.h"
#include "aquaticfoodtransport.h"

///////////////////////////////////////////////////////////////////////////////
DoseEstimation::DoseEstimation(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf), _extDf(fact), _inhDf(fact), _ingDf(fact)
{
}

bool DoseEstimation::needLocation() const
{
    return true;
}

KData DoseEstimation::modelData(const Quantity &qty) const
{
    KData d = _dataList.find(qty);
    if (d.isValid())
        return d;
    return constUserInputs().find(qty);
}
bool DoseEstimation::allocateIoPorts()
{
    _inpPorts << new KPort(this, &Srs19::Concentration, KPort::Input);
    return true;
}

void DoseEstimation::defineParameters()
{
    DataGroup dg1(Inhalation, QObject::tr("Inhalation"));
    dg1 << KData(&Srs19::InhalationRate, inhalationRate());

    DataGroup dg2(InhalationSludge, QObject::tr("Sewage Sludge"));
    dg2 << KData(&Srs19::ExposedFractionTime, 0.228)
        << KData(&Srs19::DustLoadingFactor, 1e-7);

    KDataGroupArray * ui = userInputs();
    (*ui) << dg1
        << DataGroup(Ingestion, QObject::tr("Ingestion"))
        << DataGroup(Immersion, QObject::tr("Immersion"))
        << DataGroup(ExternalGround, QObject::tr("Ground Deposition"))
        << DataGroup(ExternalSediment, QObject::tr("Sediment Exposure"))
        << dg2;

}

KDataArray DoseEstimation::result() const
{
    return _dataList;
}
void DoseEstimation::addExposedFraction(const IModel * model, int type)
{
    //Immersion/SkinImmersion/
    KDataGroupArray * ui = userInputs();
    KData * da = ui->findPtr(Srs19::ExposedFractionTime, type);
    if (da == 0) {
        //new data
        KData di(&Srs19::ExposedFractionTime, model->tagName(), exposedFraction(type), KData::NumericArray);
        ui->appendOrReplace(di, type);
    }
    else {
        da->append(model->tagName(), exposedFraction(type), KData::Numeric);
    }
}
void DoseEstimation::addConsumptionRate(const IModel * model, const Quantity & qty)
{
    int type = 0;
    if (qty == Srs19::ConcentrationInAquaticFood) {
        switch(model->info().serialId()) {
        case FreshwaterFishTransport::SerialId:
            type = AquaticFoodTransport::FreshwaterFish;
            break;
        case MarineFishTransport::SerialId:
            type = AquaticFoodTransport::MarineFish;
            break;
        case MarineShellfishTransport::SerialId:
            type = AquaticFoodTransport::MarineShellfish;
            break;
        }
    }

    KDataGroupArray * ui = userInputs();
    qreal Hp = consumptionRate(qty, type);
    KData * da = ui->findPtr(Srs19::ConsumptionRate, Ingestion);
    if (da == 0) {
        //new data
        KData di(&Srs19::ConsumptionRate, model->tagName(), Hp, KData::NumericArray);
        ui->appendOrReplace(di, Ingestion);
    }
    else {
        da->append(model->tagName(), Hp, KData::Numeric);
    }
}

void DoseEstimation::addQuantity(KPort * port, KConnector * con)
{
    KPort * other = con->oppositePort(port);
    IModel * model = other->model();
    const Quantity * qty = other->quantity();

    if (qty == &Srs19::ConcentrationInAir) {
        //Immersion
        addExposedFraction(model, Immersion);
    }
    else if (qty == &Srs19::ConcentrationInSurfaceSoil) {
        addExposedFraction(model, ExternalGround);
    }
    else if (qty == &Srs19::ConcentrationInShoreSediment) {
        addExposedFraction(model, ExternalSediment);
    }
    else if (qty == &Srs19::ConcentrationInVegetation ||
             qty == &Srs19::ConcentrationInMilk ||
             qty == &Srs19::ConcentrationInMeat ||
             qty == &Srs19::TotalConcentrationInWater ||
             qty == &Srs19::ConcentrationInShoreline ||
             qty == &Srs19::ConcentrationInAquaticFood)
    {
        addConsumptionRate(model, *qty);
    }
}

void DoseEstimation::removeQuantity(KPort * port, KConnector * con)
{
    qDebug() << "Dose connectoin removed";
    KPort * other = con->oppositePort(port);
    IModel * model = other->model();
    const Quantity * qty = other->quantity();

    KDataGroupArray * ui = userInputs();
    if (qty == &Srs19::ConcentrationInAir) {
        //Immersion
        KData * da = ui->findPtr(Srs19::ExposedFractionTime, Immersion);
        if (da) {
            da->remove(model->tagName());
            if (da->isEmpty())
                ui->removeQuantity(Srs19::ExposedFractionTime, Immersion);
        }
    }
    else if (qty == &Srs19::ConcentrationInSurfaceSoil) {
        KData * da = ui->findPtr(Srs19::ExposedFractionTime, ExternalGround);
        if (da) {
            da->remove(model->tagName());
            if (da->isEmpty())
                ui->removeQuantity(Srs19::ExposedFractionTime, ExternalGround);
        }
    }
    else if (qty == &Srs19::ConcentrationInShoreSediment) {
        KData * da = ui->findPtr(Srs19::ExposedFractionTime, ExternalSediment);
        if (da) {
            da->remove(model->tagName());
            if (da->isEmpty())
                ui->removeQuantity(Srs19::ExposedFractionTime, ExternalSediment);
        }

    }
    else if (qty == &Srs19::ConcentrationInVegetation ||
             qty == &Srs19::ConcentrationInMilk ||
             qty == &Srs19::ConcentrationInMeat ||
             qty == &Srs19::TotalConcentrationInWater ||
             qty == &Srs19::ConcentrationInShoreline ||
             qty == &Srs19::ConcentrationInAquaticFood)
    {
        KData * da = ui->findPtr(Srs19::ConsumptionRate, Ingestion);
        if (da) {
            da->remove(model->tagName());
            if (da->isEmpty())
                ui->removeQuantity(Srs19::ConsumptionRate, Ingestion);
        }
    }
}

void DoseEstimation::connectionModified(KPort * port, KConnector * con, bool connected)
{
    if (connected)
        addQuantity(port, con);
    else
        removeQuantity(port, con);
}

bool DoseEstimation::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    KLocationPort * lp = locationPort();
    if (lp == 0 || !lp->hasLocation()) {
        KOutputProxy::errorReceptorNotSpecified(this);
        err ++;
    }

    //check wether input port is connected or not
    if (!_inpPorts.isConnected()) {
        KOutputProxy::errorPortNotConnected(this);
        err ++;
    }

    _extDf.load(true);
    _inhDf.load(true);
    _ingDf.load(true);

    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;
    KOutputProxy::infoVerificationResult(this, err, warn);

    return err == 0;
}

void DoseEstimation::calcImmersionDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses, daDf;
    KDataGroupArray * ui = userInputs();
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInAir) {
            IModel * model = p->model();
            KData daCa = model->modelData(*qty);
            for (int k = 0; k < daCa.count(); k++) {
                const KDataItem & ca = daCa.at(k);
                QString nuc = ca.name();
                KData *daOf = ui->findPtr(Srs19::ExposedFractionTime, Immersion);
                if (daOf == 0)
                    continue;
                qreal Of = daOf->numericValue(model->tagName());
                qreal Df = _extDf.value(nuc, ExternalDoseCoefficient::ImmersionDose);
                qreal dose = ca.numericValue() * Df * Of;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //df
                daDf[nuc] = Df;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::ImmersionDoseCoeff, daDf, KData::Radionuclide));
        _dataList.appendOrReplace(KData(&Srs19::ImmersionDoseRate, doses, KData::Radionuclide));
    }


    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::ImmersionDoseRate, daItems));
    }
    */
}

void DoseEstimation::calcIngestionDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses, daDf;
    KDataGroupArray * ui = userInputs();
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInVegetation ||
                     qty == &Srs19::ConcentrationInMilk ||
                     qty == &Srs19::ConcentrationInMeat ||
                     qty == &Srs19::TotalConcentrationInWater ||
                     qty == &Srs19::ConcentrationInShoreline ||
                     qty == &Srs19::ConcentrationInAquaticFood)
        {
            IModel * model = p->model();
            KData daCp = model->modelData(*qty);
            for (int k = 0; k < daCp.count(); k++) {
                const KDataItem & cp = daCp.at(k);
                QString nuc = cp.name();
                KData *daHp = ui->findPtr(Srs19::ConsumptionRate, Ingestion);
                if (daHp == 0)
                    continue;
                qreal Hp = daHp->numericValue(model->tagName());
                qreal Df = _ingDf.value(nuc, ingestionCoeffIndex());
                qreal dose = cp.numericValue() * Df * Hp;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //df
                daDf[nuc] = Df;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::IngestionDoseCoeff, daDf, KData::Radionuclide));
        _dataList.appendOrReplace(KData(&Srs19::IngestionDoseRate, doses, KData::Radionuclide));
    }

    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::IngestionDoseRate, daItems));
    }
    */
}

void DoseEstimation::calcInhalationDose(QMap<QString, qreal> * totalDoses)
{
    KDataGroupArray * ui = userInputs();
    qreal Rinh = ui->numericValueOf(Srs19::InhalationRate, Inhalation);
    QMap<QString, qreal> doses, daDf;
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInAir) {
            IModel * model = p->model();
            KData daCa = model->modelData(*qty);
            for (int k = 0; k < daCa.count(); k++) {
                const KDataItem & ca = daCa.at(k);
                QString nuc = ca.name();
                qreal Df = _inhDf.value(nuc, inhalationCoeffIndex());
                qreal dose = ca.numericValue() * Df * Rinh;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //df
                daDf[nuc] = Df;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::InhalationDoseCoeff, daDf, KData::Radionuclide));
        _dataList.appendOrReplace(KData(&Srs19::InhalationDoseRate, doses, KData::Radionuclide));
    }

    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::InhalationDoseRate, daItems));
    }
    */
}
void DoseEstimation::calcGroundDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses, daDf;
    KDataGroupArray * ui = userInputs();
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInSurfaceSoil) {
            IModel * model = p->model();
            KData daCa = model->modelData(*qty);
            for (int k = 0; k < daCa.count(); k++) {
                const KDataItem & ca = daCa.at(k);
                QString nuc = ca.name();
                KData *daOf = ui->findPtr(Srs19::ExposedFractionTime, ExternalGround);
                if (daOf == 0)
                    continue;
                qreal Of = daOf->numericValue(model->tagName());
                qreal Df = _extDf.value(nuc, ExternalDoseCoefficient::SurfaceDeposit);
                qreal dose = ca.numericValue() * Df * Of;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //Df
                daDf[nuc] = Df;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if(!doses.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::GroundExposureDoseCoeff, daDf, KData::Radionuclide));
        _dataList.appendOrReplace(KData(&Srs19::GroundExposureDoseRate, doses, KData::Radionuclide));
    }

    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::GroundExposureDoseRate, daItems));
    }
    */
}

void DoseEstimation::calcSedimentDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses;
    KDataGroupArray * ui = userInputs();
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInShoreSediment) {
            IModel * model = p->model();
            KData daCa = model->modelData(*qty);
            for (int k = 0; k < daCa.count(); k++) {
                const KDataItem & ca = daCa.at(k);
                QString nuc = ca.name();
                KData *daOf = ui->findPtr(Srs19::ExposedFractionTime, ExternalSediment);
                if (daOf == 0)
                    continue;
                qreal Of = daOf->numericValue(model->tagName());
                qreal Df = _extDf.value(nuc, ExternalDoseCoefficient::SurfaceDeposit);
                qreal dose = ca.numericValue() * Df * Of;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty())
        _dataList.appendOrReplace(KData(&Srs19::SedimentDoseRate, doses, KData::Radionuclide));

    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::SedimentDoseRate, daItems));
    }
    */
}

void DoseEstimation::calcExtSludgeDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses;
    KDataGroupArray * ui = userInputs();
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::SurfaceSludgeConcentration) {
            IModel * model = p->model();
            KData daCsl = model->modelData(*qty);
            for (int k = 0; k < daCsl.count(); k++) {
                const KDataItem & csl = daCsl.at(k);
                QString nuc = csl.name();
                KData *daOf = ui->findPtr(Srs19::ExposedFractionTime, InhalationSludge);
                if (daOf == 0)
                    continue;
                qreal Of = daOf->numericValue(model->tagName());
                qreal Df = _extDf.value(nuc, ExternalDoseCoefficient::SurfaceDeposit);
                qreal dose = csl.numericValue() * Df * Of;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty())
        _dataList.appendOrReplace(KData(&Srs19::SewageDoseRate, doses, KData::Radionuclide));

    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::SewageDoseRate, daItems));
    }
    */
}

void DoseEstimation::calcInhSludgeDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses;
    KDataGroupArray * ui = userInputs();
    qreal Rinh = ui->numericValueOf(Srs19::InhalationRate, Inhalation);
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInWetSewage) {
            IModel * model = p->model();
            KData daCsl = model->modelData(*qty);
            for (int k = 0; k < daCsl.count(); k++) {
                const KDataItem & csl = daCsl.at(k);
                QString nuc = csl.name();
                KData *daOf = ui->findPtr(Srs19::ExposedFractionTime, InhalationSludge);
                KData * daDL = ui->findPtr(Srs19::DustLoadingFactor, InhalationSludge);
                if (daOf == 0 || daDL == 0)
                    continue;

                qreal Of = daOf->numericValue(model->tagName());
                qreal Df = _inhDf.value(nuc, inhalationCoeffIndex());
                qreal DL = daDL->numericValue();
                qreal dose = csl.numericValue() * Rinh * Df * Of * DL;
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty())
        _dataList.appendOrReplace(KData(&Srs19::ResuspensionDoseRate, doses, KData::Radionuclide));

    /*
    DataItemArray daItems;
    QMap<QString, qreal>::const_iterator it = doses.constBegin();
    QMap<QString, qreal>::const_iterator end = doses.constEnd();
    while (it != end) {
        daItems << KDataItem(it.key(), it.value(), KData::Radionuclide);
        it++;
    }

    if (!daItems.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::ResuspensionDoseRate, daItems));
    }
    */
}
void DoseEstimation::calcSkinDose(QMap<QString, qreal> * totalDoses)
{
    QMap<QString, qreal> doses, daDf;
    KDataGroupArray * ui = userInputs();
    const KPortList & inpPorts = inputs().first()->connectedPorts();
    foreach(KPort * p, inpPorts) {
        const Quantity * qty = p->quantity();
        if (qty == &Srs19::ConcentrationInAir) {
            IModel * model = p->model();
            KData daCa = model->modelData(*qty);
            for (int k = 0; k < daCa.count(); k++) {
                const KDataItem & ca = daCa.at(k);
                QString nuc = ca.name();
                KData *daOf = ui->findPtr(Srs19::ExposedFractionTime, Immersion);
                if (daOf == 0)
                    continue;
                qreal Of = daOf->numericValue(model->tagName());
                qreal Df = _extDf.value(nuc, ExternalDoseCoefficient::SkinDose);
                qreal dose = ca.numericValue() * Df * Of;    //default (page 83)
                qreal oldDose = doses.value(nuc, 0);
                doses[nuc] = dose + oldDose;

                //Df
                daDf[nuc] = Df;

                //total dose
                oldDose = totalDoses->value(nuc, 0);
                totalDoses->insert(nuc, dose + oldDose);
            }
        }
    }

    if (!doses.isEmpty()) {
        _dataList.appendOrReplace(KData(&Srs19::SkinDoseCoeff, daDf, KData::Radionuclide));
        _dataList.appendOrReplace(KData(&Srs19::SkinDoseRate, doses, KData::Radionuclide));
    }
}

bool DoseEstimation::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);

    //clear results
    _dataList.clear();

    if (needLocation()) {
        //check for distance
        KLocationPort * lp = locationPort();
        if (lp == 0)
            return false;

        KLocation loc = this->location();
        _dataList.setLocation(loc);
    }

    //perform dose calculation
    QMap<QString, qreal> totalDoses;
    calcImmersionDose(&totalDoses);
    calcIngestionDose(&totalDoses);
    calcInhalationDose(&totalDoses);
    calcSedimentDose(&totalDoses);
    calcGroundDose(&totalDoses);
    calcExtSludgeDose(&totalDoses);
    calcInhSludgeDose(&totalDoses);

    //skin doses
    QMap<QString, qreal> skinDoses;
    calcSkinDose(&skinDoses);

    //save total doses
    if (!totalDoses.isEmpty())
        _dataList.appendOrReplace(KData(&Srs19::TotalDose, totalDoses, KData::Radionuclide));

    //return true if there are any result
    return !_dataList.isEmpty();
}

const KPortList & DoseEstimation::inputs() const
{
    return _inpPorts;
}

//infant
InfantDoseEstimation::InfantDoseEstimation(IModelFactory * fact, const KModelInfo & inf)
    : DoseEstimation(fact, inf)
{

}
QString InfantDoseEstimation::displayText() const
{
    return QObject::tr("Infant");
}

qreal InfantDoseEstimation::exposedFraction(int type) const
{
    switch(type) {
    case DoseEstimation::Immersion:
        return 1;
    case DoseEstimation::ExternalGround:
        return 1;
    case DoseEstimation::ExternalSediment:
        return 0.12;
    case DoseEstimation::ExternalIrrigation:
        return 0.06;
    }
    return 0;
}
qreal InfantDoseEstimation::consumptionRate(const Quantity & qty, int type) const
{
    if (qty == Srs19::ConcentrationInVegetation)
        return 150;
    else if (qty == Srs19::ConcentrationInMilk)
        return 300;
    else if (qty == Srs19::ConcentrationInMeat)
        return 40;
    else if (qty == Srs19::TotalConcentrationInWater)
        return 0.260;
    else if (qty == Srs19::ConcentrationInShoreline)
        return 0.260;
    else if (qty == Srs19::ConcentrationInAquaticFood)
    {
        switch(type) {
        case AquaticFoodTransport::FreshwaterFish:
            return 15;
        case AquaticFoodTransport::MarineFish:
            return 25;
        case AquaticFoodTransport::MarineShellfish:
            return 0;
        }
    }
    return 0;
}
qreal InfantDoseEstimation::inhalationRate() const
{
    return 1400;
}
int InfantDoseEstimation::inhalationCoeffIndex() const
{
    return InhalationDoseCoefficient::Infant;
}
int InfantDoseEstimation::ingestionCoeffIndex() const
{
    return IngestionDoseCoefficient::Infant;
}
//adult
AdultDoseEstimation::AdultDoseEstimation(IModelFactory * fact, const KModelInfo & inf)
    : DoseEstimation(fact, inf)
{

}
QString AdultDoseEstimation::displayText() const
{
    return QObject::tr("Adult");
}

qreal AdultDoseEstimation::exposedFraction(int type) const
{
    switch(type) {
    case DoseEstimation::Immersion:
        return 1;
    case DoseEstimation::ExternalGround:
        return 1;
    case DoseEstimation::ExternalSediment:
        return 0.18;
    case DoseEstimation::ExternalIrrigation:
        return 0.06;
    }
    return 0;
}
qreal AdultDoseEstimation::consumptionRate(const Quantity & qty, int type) const
{
    if (qty == Srs19::ConcentrationInVegetation)
        return 410;
    else if (qty == Srs19::ConcentrationInMilk)
        return 250;
    else if (qty == Srs19::ConcentrationInMeat)
        return 100;
    else if (qty == Srs19::TotalConcentrationInWater)
        return 0.600;
    else if (qty == Srs19::ConcentrationInShoreline)
        return 0.600;
    else if (qty == Srs19::ConcentrationInAquaticFood)
    {
        switch(type) {
        case AquaticFoodTransport::FreshwaterFish:
            return 30;
        case AquaticFoodTransport::MarineFish:
            return 50;
        case AquaticFoodTransport::MarineShellfish:
            return 15;
        }
    }
    return 0;
}
qreal AdultDoseEstimation::inhalationRate() const
{
    return 8400;
}
int AdultDoseEstimation::inhalationCoeffIndex() const
{
    return InhalationDoseCoefficient::Adult;
}
int AdultDoseEstimation::ingestionCoeffIndex() const
{
    return IngestionDoseCoefficient::Adult;
}
