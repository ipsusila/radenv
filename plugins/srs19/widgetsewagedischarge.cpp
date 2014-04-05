#include "widgetsewagedischarge.h"
#include "ui_widgetsewagedischarge.h"

WidgetSewageDischarge::WidgetSewageDischarge(KDataArray * userParam, QWidget *parent) :
    IUserInput(parent),
    ui(new Ui::WidgetSewageDischarge),
    userParameter(userParam)
{
    ui->setupUi(this);
    ui->tblDiscarges->setVariable(&Srs19::SewageDischargeRate);
    ui->tblDiscarges->setData(*userParam);

    //set discharge period, anual sludge production, number of served person
    KData xd = userParam->find(Srs19::DischargePeriod);
    ui->numDischargePeriod->setToolTip(Srs19::DischargePeriod.description);
    if (xd.isValid())
        ui->numDischargePeriod->setValue(xd.numericValue());

    xd = userParam->find(Srs19::AnnualSludgeProduction);
    ui->numSludgeProduction->setToolTip(Srs19::AnnualSludgeProduction.description);
    if (xd.isValid())
        ui->numSludgeProduction->setValue(xd.numericValue());

    xd = userParam->find(Srs19::NumOfServedPerson);
    ui->numPerson->setToolTip(Srs19::NumOfServedPerson.description);
    if (xd.isValid())
        ui->numPerson->setValue(xd.numericValue());
}

WidgetSewageDischarge::~WidgetSewageDischarge()
{
    delete ui;
}

QSize WidgetSewageDischarge::sizeHint() const
{
    return QSize(400, 400);
}
void WidgetSewageDischarge::acceptValues()
{
    Q_ASSERT(userParameter != 0);

    KDataArray list;
    list.append(KData(&Srs19::DischargePeriod, ui->numDischargePeriod->value()));
    list.append(KData(&Srs19::AnnualSludgeProduction, ui->numSludgeProduction->value()));
    list.append(KData(&Srs19::NumOfServedPerson, ui->numPerson->value()));

    KData xd = ui->tblDiscarges->data();
    if (xd.isValid())
        list.append(xd);

    *userParameter = list;
}
