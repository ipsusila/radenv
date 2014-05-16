#include <QtDebug>
#include <QSettings>
#include "kapplication.h"
#include "dialogcalculation.h"
#include "ui_dialogcalculation.h"

DialogCalculation::DialogCalculation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCalculation)
{
    ui->setupUi(this);
    ui->rbOnce->setChecked(true);
    ui->numNTimes->setEnabled(false);
    ui->numInterval->setEnabled(false);
    ui->chkQueueMode->setChecked(true);

    //TODO
    //continous is disabled
    ui->rbContinuous->setEnabled(false);

    KApplication::selfInstance()->setupValidGeometry("calcdialog/geometry", this);
}

DialogCalculation::~DialogCalculation()
{
    QSettings settings;
    settings.setValue("calcdialog/geometry", saveGeometry());
    delete ui;
    qDebug() << "Remove calculation dialog";
}
void DialogCalculation::on_rbContinuous_toggled(bool checked)
{
    if (checked) {
        ui->numNTimes->setEnabled(false);
        ui->numInterval->setEnabled(true);
    }
}

void DialogCalculation::on_rbOnce_toggled(bool checked)
{
    if (checked) {
        ui->numNTimes->setEnabled(false);
        ui->numInterval->setEnabled(false);
    }
}

void DialogCalculation::on_rbNTimes_toggled(bool checked)
{
    if (checked) {
        ui->numNTimes->setEnabled(true);
        ui->numInterval->setEnabled(true);
    }
}
KCalculationInfo DialogCalculation::calculationInfo() const
{
    int runCnt;
    if (ui->rbContinuous->isChecked())
        runCnt = KCalculationInfo::Continuous;
    else if (ui->rbNTimes->isChecked())
        runCnt = ui->numNTimes->value();
    else
        runCnt = KCalculationInfo::Once;

    int nsec = ui->numInterval->value();
    return KCalculationInfo(nsec, runCnt, 0, ui->chkQueueMode->isChecked(), ui->chkContinueOnError->isChecked());
}
