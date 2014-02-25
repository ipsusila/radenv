#include "dialogatmospherictransport.h"
#include "ui_dialogatmospherictransport.h"
#include "symbol.h"

DialogAtmosphericTransport::DialogAtmosphericTransport(const KDataArray& dataList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAtmosphericTransport)
{
    ui->setupUi(this);
    setDataList(dataList);
}

DialogAtmosphericTransport::~DialogAtmosphericTransport()
{
    delete ui;
}

KDataArray DialogAtmosphericTransport::dataList() const
{
    KDataArray list;
    list.append(KData(&Srs19::FractionOfWind, ui->numWindFraction->value()));
    list.append(KData(&Srs19::GeometricMeanOfWindSpeed, ui->numWindSpeed->value()));
    list.append(KData(&Srs19::BuildingHeight, ui->numBuildingHeight->value()));
    list.append(KData(&Srs19::CrossSectionalArea, ui->numBuildingArea->value()));
    list.append(KData(&Srs19::DryDepositionCoeff, ui->numDryCoeff->value()));
    list.append(KData(&Srs19::WetDepositionCoeff, ui->numWetCoeff->value()));

    list.append(KData(&Srs19::SigmaFitParameterE, ui->numFitE->value()));
    list.append(KData(&Srs19::SigmaFitParameterG, ui->numFitG->value()));
    list.append(KData(&Srs19::AirConcentrationIncrease, ui->numB0->value()));
    list.append(KData(&Srs19::ConstantValue1Metre, ui->numK->value()));

    return list;
}
void DialogAtmosphericTransport::setDataList(const KDataArray& d)
{
    //set discharge period, height, flowrate and diameter
    KData xd = d.find(Srs19::FractionOfWind.symbol);
    if (xd.isValid())
        ui->numWindFraction->setValue(xd.numericValue());

    xd = d.find(Srs19::GeometricMeanOfWindSpeed.symbol);
    if (xd.isValid())
        ui->numWindSpeed->setValue(xd.numericValue());

    xd = d.find(Srs19::BuildingHeight.symbol);
    if (xd.isValid())
        ui->numBuildingHeight->setValue(xd.numericValue());

    xd = d.find(Srs19::CrossSectionalArea.symbol);
    if (xd.isValid())
        ui->numBuildingArea->setValue(xd.numericValue());

    xd = d.find(Srs19::DryDepositionCoeff.symbol);
    if (xd.isValid())
        ui->numDryCoeff->setValue(xd.numericValue());

    xd = d.find(Srs19::WetDepositionCoeff.symbol);
    if (xd.isValid())
        ui->numWetCoeff->setValue(xd.numericValue());

    xd = d.find(Srs19::SigmaFitParameterE.symbol);
    if (xd.isValid())
        ui->numFitE->setValue(xd.numericValue());

    xd = d.find(Srs19::SigmaFitParameterG.symbol);
    if (xd.isValid())
        ui->numFitG->setValue(xd.numericValue());

    xd = d.find(Srs19::AirConcentrationIncrease.symbol);
    if (xd.isValid())
        ui->numB0->setValue(xd.numericValue());

    xd = d.find(Srs19::ConstantValue1Metre.symbol);
    if (xd.isValid())
        ui->numK->setValue(xd.numericValue());
}


