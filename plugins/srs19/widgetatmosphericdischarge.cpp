#include "widgetatmosphericdischarge.h"
#include "ui_widgetatmosphericdischarge.h"

WidgetAtmosphericDischarge::WidgetAtmosphericDischarge(KStorage * storage, KDataArray * userParam, QWidget *parent) :
    IUserInput(parent),
    ui(new Ui::WidgetAtmosphericDischarge),
    userParameter(userParam)
{
    Q_ASSERT(userParam);

    ui->setupUi(this);
    ui->lblComment->setText(tr("The followings are needed if H <= 2.5Hb and x <= sqrt(Ab), source and receptor *not* on same "
                            "building surface (Refer to Eq. 8, pp. 24)"));

    //setup data
    ui->tblDiscarges->setStorage(storage);
    ui->tblDiscarges->setVariable(&Srs19::AtmosphericDischargeRate);
    ui->tblDiscarges->setData(*userParam);

    //set discharge period, height, flowrate and diameter
    KData xd = userParam->find(Srs19::DischargePeriod);
    ui->numDischargePeriod->setToolTip(Srs19::DischargePeriod.description);
    if (xd.isValid())
        ui->numDischargePeriod->setValue(xd.numericValue());

    xd = userParam->find(Srs19::ReleaseHeight);
    ui->numReleaseHeight->setToolTip(Srs19::ReleaseHeight.description);
    if (xd.isValid())
        ui->numReleaseHeight->setValue(xd.numericValue());

    xd = userParam->find(Srs19::AirFlowRate);
    ui->numFlowrate->setToolTip(Srs19::AirFlowRate.description);
    if (xd.isValid())
        ui->numFlowrate->setValue(xd.numericValue());

    xd = userParam->find(Srs19::Diameter);
    ui->numDiameter->setToolTip(Srs19::Diameter.description);
    if (xd.isValid())
        ui->numDiameter->setValue(xd.numericValue());
}

WidgetAtmosphericDischarge::~WidgetAtmosphericDischarge()
{
    delete ui;
}
QSize WidgetAtmosphericDischarge::sizeHint() const
{
    return QSize(400, 400);
}
bool WidgetAtmosphericDischarge::isEmpty() const
{
    return false;
}
bool WidgetAtmosphericDischarge::validate()
{
    return true;
}

void WidgetAtmosphericDischarge::acceptValues()
{
    Q_ASSERT(userParameter != 0);

    KDataArray list;
    list.append(KData(&Srs19::DischargePeriod, ui->numDischargePeriod->value()));
    list.append(KData(&Srs19::ReleaseHeight, ui->numReleaseHeight->value()));
    list.append(KData(&Srs19::AirFlowRate, ui->numFlowrate->value()));
    list.append(KData(&Srs19::Diameter, ui->numDiameter->value()));

    KData xd = ui->tblDiscarges->data();
    if (xd.isValid())
        list.append(xd);

    *userParameter = list;
}
