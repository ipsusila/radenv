#include "dialogatmosphericdischarge.h"
#include "ui_dialogatmosphericdischarge.h"
#include "dischargeitemdelegate.h"
#include "symbol.h"

DialogAtmosphericDischarge::DialogAtmosphericDischarge(const KDataArray& d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAtmosphericDischarge)
{
    ui->setupUi(this);
    ui->tblDiscarges->setVariable(&Srs19::AtmosphericDischargeRate);
    setDataList(d);
}

DialogAtmosphericDischarge::~DialogAtmosphericDischarge()
{
    delete ui;
}

KDataArray DialogAtmosphericDischarge::dataList() const
{
    KDataArray list;
    list.append(KData(&Srs19::DischargePeriod, ui->numDischargePeriod->value()));
    list.append(KData(&Srs19::ReleaseHeight, ui->numReleaseHeight->value()));
    list.append(KData(&Srs19::AirFlowRate, ui->numFlowrate->value()));
    list.append(KData(&Srs19::Diameter, ui->numDiameter->value()));

    KData xd = ui->tblDiscarges->data();
    if (xd.isValid())
        list.append(xd);

    return list;
}
void DialogAtmosphericDischarge::setDataList(const KDataArray& d)
{
    ui->tblDiscarges->setData(d);

    //set discharge period, height, flowrate and diameter
    KData xd = d.find(Srs19::DischargePeriod.symbol);
    if (xd.isValid())
        ui->numDischargePeriod->setValue(xd.numericValue());

    xd = d.find(Srs19::ReleaseHeight.symbol);
    if (xd.isValid())
        ui->numReleaseHeight->setValue(xd.numericValue());

    xd = d.find(Srs19::AirFlowRate.symbol);
    if (xd.isValid())
        ui->numFlowrate->setValue(xd.numericValue());

    xd = d.find(Srs19::Diameter.symbol);
    if (xd.isValid())
        ui->numDiameter->setValue(xd.numericValue());
}

