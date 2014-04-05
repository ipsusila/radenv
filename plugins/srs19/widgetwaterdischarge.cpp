#include "widgetwaterdischarge.h"
#include "ui_widgetwaterdischarge.h"

WidgetWaterDischarge::WidgetWaterDischarge(KDataArray * userParam, QWidget *parent) :
    IUserInput(parent),
    ui(new Ui::WidgetWaterDischarge),
    userParameter(userParam)
{
    ui->setupUi(this);
    ui->tblDiscarges->setVariable(&Srs19::WaterDischargeRate);
    ui->tblDiscarges->setData(*userParam);

    //set discharge period and flowrate
    KData xd = userParam->find(Srs19::DischargePeriod);
    ui->numDischargePeriod->setToolTip(Srs19::DischargePeriod.description);
    if (xd.isValid())
        ui->numDischargePeriod->setValue(xd.numericValue());

    xd = userParam->find(Srs19::LiquidFlowRate);
    ui->numFlowrate->setToolTip(Srs19::LiquidFlowRate.description);
    if (xd.isValid())
        ui->numFlowrate->setValue(xd.numericValue());
}

WidgetWaterDischarge::~WidgetWaterDischarge()
{
    delete ui;
}

QSize WidgetWaterDischarge::sizeHint() const
{
    return QSize(400, 300);
}
void WidgetWaterDischarge::acceptValues()
{
    Q_ASSERT(userParameter != 0);

    KDataArray list;
    list.append(KData(&Srs19::DischargePeriod, ui->numDischargePeriod->value()));
    list.append(KData(&Srs19::LiquidFlowRate, ui->numFlowrate->value()));

    KData xd = ui->tblDiscarges->data();
    if (xd.isValid())
        list.append(xd);

    *userParameter = list;
}
