#include <QSettings>
#include "kapplication.h"
#include "dialogscenario.h"
#include "ui_dialogscenario.h"
#include "kassessment.h"
#include "kscenario.h"

DialogScenario::DialogScenario(KAssessment *aP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScenario),
    asPtr(aP), scPtr(0)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onButtonAccept()));

    KApplication::selfInstance()->setupValidGeometry("scenariodialog/geometry", this);
}
DialogScenario::DialogScenario(KScenario *scP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScenario),
    asPtr(0), scPtr(scP)
{
    ui->setupUi(this);
    ui->inpScenarioName->setText(scP->name());
    ui->inpDescription->setPlainText(scP->description());
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onButtonAccept()));

    KApplication::selfInstance()->setupValidGeometry("scenariodialog/geometry", this);
}

DialogScenario::~DialogScenario()
{
    QSettings settings;
    settings.setValue("scenariodialog/geometry", this->saveGeometry());
    delete ui;
}
void DialogScenario::onButtonAccept()
{
    KScenario * sP;
    if (asPtr == 0 && scPtr == 0)
        return;
    else if (scPtr != 0) {
        sP = scPtr;
        sP->setName(ui->inpScenarioName->text());
    }
    else {
        sP = asPtr->createScenario(ui->inpScenarioName->text());
    }

    sP->setDescription(ui->inpDescription->toPlainText());
    scPtr = sP;
}

KScenario * DialogScenario::scenario() const
{
    return scPtr;
}
