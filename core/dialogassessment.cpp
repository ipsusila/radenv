#include "dialogassessment.h"
#include "ui_dialogassessment.h"
#include "kassessment.h"

DialogAssessment::DialogAssessment(KAssessment *aP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAssessment), asPtr(aP)
{
    ui->setupUi(this);
    if (asPtr != 0) {
        ui->inpName->setText(asPtr->name());
        ui->inpAuthor->setText(asPtr->author());
        ui->inpDescription->setPlainText(asPtr->description());
        ui->inpRemark->setPlainText(asPtr->remark());

        //TODO
        //add document
    }
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onButtonAccepted()));
}

DialogAssessment::~DialogAssessment()
{
    delete ui;
}
void DialogAssessment::onButtonAccepted()
{
    KAssessment * aP;
    if (asPtr != 0)
        aP = asPtr;
    else
        aP = new KAssessment(QDateTime::currentDateTime(), 0);

    aP->setName(ui->inpName->text());
    aP->setAuthor(ui->inpAuthor->text());
    aP->setDescription(ui->inpDescription->toPlainText());
    aP->setRemark(ui->inpRemark->toPlainText());

    //TODO
    //add document

    asPtr = aP;
}

KAssessment * DialogAssessment::assessment(QObject * parent) const
{
    if (asPtr != 0 && asPtr->parent() != parent)
        asPtr->setParent(parent);
    return asPtr;
}
