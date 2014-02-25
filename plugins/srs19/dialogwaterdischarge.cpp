#include "dialogwaterdischarge.h"
#include "ui_dialogwaterdischarge.h"
#include "symbol.h"

DialogWaterDischarge::DialogWaterDischarge(const KDataArray& d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWaterDischarge)
{
    ui->setupUi(this);
    ui->tblDiscarges->setVariable(&Srs19::WaterDischargeRate);
    setDataList(d);
}

DialogWaterDischarge::~DialogWaterDischarge()
{
    delete ui;
}

KDataArray DialogWaterDischarge::dataList() const
{
    KDataArray list;
    list.append(KData(&Srs19::DischargePeriod, ui->numDischargePeriod->value()));
    list.append(KData(&Srs19::LiquidFlowRate, ui->numFlowrate->value()));

    KData xd = ui->tblDiscarges->data();
    if (xd.isValid())
        list.append(xd);

    return list;
}
void DialogWaterDischarge::setDataList(const KDataArray& d)
{
    ui->tblDiscarges->setData(d);

    //set discharge period and flowrate
    KData xd = d.find(Srs19::DischargePeriod.symbol);
    if (xd.isValid())
        ui->numDischargePeriod->setValue(xd.numericValue());

    xd = d.find(Srs19::LiquidFlowRate.symbol);
    if (xd.isValid())
        ui->numFlowrate->setValue(xd.numericValue());
}
