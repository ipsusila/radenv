#include "dialogsewagedischarge.h"
#include "ui_dialogsewagedischarge.h"

DialogSewageDischarge::DialogSewageDischarge(const KDataArray& d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSewageDischarge)
{
    ui->setupUi(this);
    ui->tblDiscarges->setVariable(&Srs19::SewageDischargeRate);
    setDataList(d);
}

DialogSewageDischarge::~DialogSewageDischarge()
{
    delete ui;
}

KDataArray DialogSewageDischarge::dataList() const
{
    KDataArray list;
    list.append(KData(&Srs19::DischargePeriod, ui->numDischargePeriod->value()));
    list.append(KData(&Srs19::AnnualSludgeProduction, ui->numSludgeProduction->value()));
    list.append(KData(&Srs19::NumOfServedPerson, ui->numPerson->value()));

    KData xd = ui->tblDiscarges->data();
    if (xd.isValid())
        list.append(xd);

    return list;
}
void DialogSewageDischarge::setDataList(const KDataArray& d)
{
    ui->tblDiscarges->setData(d);

    //set discharge period, anual sludge production, number of served person
    KData xd = d.find(Srs19::DischargePeriod.symbol);
    if (xd.isValid())
        ui->numDischargePeriod->setValue(xd.numericValue());

    xd = d.find(Srs19::AnnualSludgeProduction.symbol);
    if (xd.isValid())
        ui->numSludgeProduction->setValue(xd.numericValue());

    xd = d.find(Srs19::NumOfServedPerson.symbol);
    if (xd.isValid())
        ui->numPerson->setValue(xd.numericValue());
}

