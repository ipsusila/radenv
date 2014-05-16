#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "kapplication.h"
#include "kstorage.h"
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
        ui->inpDocumentName->setText(asPtr->docname());
    }
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onButtonAccepted()));
    KApplication::selfInstance()->setupValidGeometry("asdialog/geometry", this);
}

DialogAssessment::~DialogAssessment()
{
    QSettings settings;
    settings.setValue("asdialog/geometry", this->saveGeometry());
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

    //open document file
    QFile file(ui->inpDocumentName->text());
    if (file.open(QIODevice::ReadOnly)) {
        aP->setDocument(file.readAll());
        aP->setDocname(file.fileName());
    }

    asPtr = aP;
}

KAssessment * DialogAssessment::assessment(QObject * parent) const
{
    if (asPtr != 0 && asPtr->parent() != parent)
        asPtr->setParent(parent);
    return asPtr;
}

void DialogAssessment::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
        QString aName = ui->inpName->text();
        if (KApplication::selfInstance()->storage()->assessmentExists(aName)) {
            int ret = QMessageBox::warning(this, tr("Already exist"), tr("Assessment '") + aName
                                           + tr("' already exists, overwrite existing?"),
                                           QMessageBox::Yes, QMessageBox::No);
            if (ret == QMessageBox::Yes)
                this->accept();
        }
        else {
            this->accept();
        }
    }
}

void DialogAssessment::on_btnBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",
                                                     tr("Document files (*.doc *.docx *.pdf *.txt);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        ui->inpDocumentName->setText(fileName);
    }
}
