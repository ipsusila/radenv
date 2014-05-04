#include <QtGui>
#include <QtDebug>
#include "dialogradionuclide.h"
#include "ui_dialogradionuclide.h"
#include "kstorage.h"
#include "radionuclidemodel.h"
#include "koutput.h"

DialogRadionuclide::DialogRadionuclide(KStorage * stg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRadionuclide)
{
    ui->setupUi(this);
    setupRadionuclides(stg);
}

DialogRadionuclide::~DialogRadionuclide()
{
    delete ui;
}

KRadionuclide DialogRadionuclide::selectedRadionuclide() const
{
    return _model->current();
}

void DialogRadionuclide::setupRadionuclides(KStorage * storage)
{
    Q_ASSERT(storage != 0);

    _model = new RadionuclideModel(storage, this);
    ui->tblRadionuclides->setModel(_model);

    //set default column width
    ui->tblRadionuclides->setColumnWidth(0, 100);
    ui->tblRadionuclides->setColumnWidth(1, 120);
    ui->tblRadionuclides->setColumnWidth(2, 100);
    ui->tblRadionuclides->setColumnWidth(3, 60);
    ui->tblRadionuclides->setColumnWidth(4, 90);

    //set selection model
    ui->tblRadionuclides->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblRadionuclides->setSelectionBehavior(QAbstractItemView::SelectRows);

    //selection models
    QItemSelectionModel * selection = ui->tblRadionuclides->selectionModel();
    connect(selection, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this,
            SLOT(onSelectedRowChanged(QModelIndex,QModelIndex)));

    //select first row
    if (_model->rowCount() > 0) {
        ui->tblRadionuclides->selectRow(0);
    }
}

void DialogRadionuclide::on_cmdNew_clicked()
{
    ui->inpRadionuclide->clear();
    ui->inpDecayConstant->clear();
    ui->valHalfLife->clear();
    ui->valUsage->setText("0");
    ui->inpAttributes->setText("0");
}

void DialogRadionuclide::on_cmdSave_clicked()
{
    //convert input to radionuclide
    qreal lambda = ui->inpDecayConstant->text().toDouble();
    KHalfLife hl(lambda, true);
    int usage = ui->valUsage->text().toInt();
    int attr = ui->inpAttributes->text().toInt();

    //create radionuclide
    KRadionuclide nuc(ui->inpRadionuclide->text(),
                      hl, usage);
    nuc.addAttribute(attr);
    if (!nuc.isValid())
        return;

    //ask user whether they want to save or not
    int ret;
    if (_model->contains(nuc)) {
        ret = QMessageBox::question(this, tr("Save Radionuclide?"),
            tr("Radionuclide [%1] already exists in database. Overwrite?")
            .arg(nuc.nuclide()), QMessageBox::Yes, QMessageBox::No);
    }
    else {
        ret = QMessageBox::question(this, tr("Save Radionuclide?"),
            tr("Do you really want to add radionuclide [%1] to database")
            .arg(nuc.nuclide()), QMessageBox::Yes, QMessageBox::No);
    }

    //save file
    if (ret == QMessageBox::Yes) {
        xTrace() << tr("Saving radionuclide: ") << nuc.nuclide();
        _model->save(nuc);
    }
}

void DialogRadionuclide::on_cmdRemove_clicked()
{
    KRadionuclide nuc = _model->current();
    if (!nuc.isValid())
        return;

    int ret = QMessageBox::question(this, tr("Remove Radionuclide?"),
                                    QString(tr("Do you really want to remove radionuclide [%1] from database"))
                                    .arg(nuc.nuclide()), QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        xTrace() << tr("Removing radionuclide: ") << nuc.nuclide();
        _model->remove(nuc);
    }
}

void DialogRadionuclide::onSelectedRowChanged(const QModelIndex & current, const QModelIndex & /*previous*/ )
{
    KRadionuclide nuclide = _model->radionuclide(current.row());
    if (nuclide.isValid()) {
        _model->setCurrent(nuclide);

        ui->inpRadionuclide->setText(nuclide.nuclide());
        ui->inpDecayConstant->setText(QString::number(nuclide.halfLife().decayConstant()));
        ui->valHalfLife->setText(nuclide.halfLife().displayText());
        ui->valUsage->setText(QString::number(nuclide.usage()));
        ui->inpAttributes->setText(QString::number(nuclide.attributes()));
    }
}
