#include <QSettings>
#include "kapplication.h"
#include "koptions.h"
#include "dialogoption.h"
#include "ui_dialogoption.h"

DialogOption::DialogOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOption)
{
    ui->setupUi(this);
    KApplication::selfInstance()->setupValidGeometry("optiondialog/geometry", this);
    loadOptions();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(applyOptions()));
}

DialogOption::~DialogOption()
{
    QSettings settings;
    settings.setValue("optiondialog/geometry", saveGeometry());
    delete ui;
}

void DialogOption::loadOptions()
{
    KOptions * opt = KApplication::selfInstance()->options();
    ui->inpSceneWidth->setValue(opt->scenarioSceneSize().width());
    ui->inpSceneHeight->setValue(opt->scenarioSceneSize().height());
    ui->inpUiMinWidth->setValue(opt->minInputWidth());
    ui->inpUiMaxWidth->setValue(opt->maxInputWidth());
    ui->inpNumberPrecision->setValue(opt->numberPrecision());

}
void DialogOption::applyOptions()
{
    KOptions * opt = KApplication::selfInstance()->options();
    opt->setScenarioSceneSize(ui->inpSceneWidth->value(), ui->inpSceneHeight->value());
    opt->setMinInputWidth(ui->inpUiMinWidth->value());
    opt->setMaxInputWidth(ui->inpUiMaxWidth->value());
    opt->setNumberPrecision(ui->inpNumberPrecision->value());
}
