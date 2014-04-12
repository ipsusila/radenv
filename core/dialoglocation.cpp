#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "dialoglocation.h"
#include "ui_dialoglocation.h"
#include "kstorage.h"
#include "locationmodel.h"
#include "koutput.h"

DialogLocation::DialogLocation(KStorage * stg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLocation)
{
    ui->setupUi(this);
    setupLocations(stg);
}

DialogLocation::~DialogLocation()
{
    delete ui;
}

void DialogLocation::setupLocations(KStorage * stg)
{
    if (stg == 0)
        stg = KStorage::storage();

    _model = new LocationModel(stg, this);
    ui->tblLocation->setModel(_model);

    //set default column width

    //set selection model
    ui->tblLocation->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblLocation->setSelectionBehavior(QAbstractItemView::SelectRows);

    //selection models
    QItemSelectionModel * selection = ui->tblLocation->selectionModel();
    connect(selection, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this,
            SLOT(onSelectedRowChanged(QModelIndex,QModelIndex)));

    //select first row
    if (_model->rowCount() > 0) {
        ui->tblLocation->selectRow(0);
    }
}

KLocation DialogLocation::selectedLocation() const
{
    return _model->current();
}

void DialogLocation::on_cmdNew_clicked()
{
    ui->inpCode->clear();
    ui->inpName->clear();
    ui->inpDescription->clear();
    ui->inpMarker->clear();
    ui->edDistances->setText(QString());
    ui->numDegree->setValue(0);
    ui->numLatitude->setValue(0);
    ui->numLongitude->setValue(0);
}

void DialogLocation::on_cmdSave_clicked()
{
    KLocation loc(ui->inpCode->text());
    loc.setName(ui->inpName->text());
    //loc.addDistance(ui->numDistance->value());
    loc.setDistance(ui->edDistances->text(), KLocation::Delimiter);
    loc.setAngle(ui->numDegree->value());
    loc.setLatitude(ui->numLatitude->value());
    loc.setLongitude(ui->numLongitude->value());
    loc.setDescription(ui->inpDescription->toPlainText());
    loc.setMarker(ui->inpMarker->text());

    if (!loc.isValid())
        return;

    //ask user whether they want to save or not
    int ret;
    if (_model->contains(loc)) {
        ret = QMessageBox::question(this, tr("Save Location?"),
            tr("Location [%1] already exists in database. Overwrite?")
            .arg(loc.code()), QMessageBox::Yes, QMessageBox::No);
    }
    else {
        ret = QMessageBox::question(this, tr("Save Location?"),
            tr("Do you really want to add location [%1] to database")
            .arg(loc.code()), QMessageBox::Yes, QMessageBox::No);
    }

    //save file
    if (ret == QMessageBox::Yes) {
        xTrace() << tr("Saving location: ") << loc.displayText();
        _model->save(loc);
    }
}

void DialogLocation::on_cmdRemove_clicked()
{
    KLocation loc = _model->current();
    if (!loc.isValid())
        return;

    int ret = QMessageBox::question(this, tr("Remove Location?"),
                                    QString(tr("Do you really want to remove location [%1] from database"))
                                    .arg(loc.code()), QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        xTrace() << tr("Removing location: ") << loc.displayText();
        _model->remove(loc);
    }
}

void DialogLocation::on_cmdBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    "",
                                                    tr("Images (*.png *.xpm *.jpg *.bmp *.gif)"));
    if (!fileName.isEmpty()) {
        ui->inpMarker->setText(fileName);
        displayMarker(QPixmap(fileName));
    }
}

void DialogLocation::onSelectedRowChanged(const QModelIndex & current, const QModelIndex & /*previous*/ )
{
    KLocation loc = _model->location(current.row());
    if (loc.isValid()) {
        _model->setCurrent(loc);

        ui->inpCode->setText(loc.code());
        ui->inpName->setText(loc.name());
        ui->inpDescription->setText(loc.description());
        //ui->numDistance->setValue(loc.distance());
        ui->edDistances->setText(loc.distances(KLocation::Delimiter));
        ui->numDegree->setValue(loc.angle());
        ui->numLatitude->setValue(loc.latitude());
        ui->numLongitude->setValue(loc.longitude());

        ui->inpMarker->clear();
        displayMarker(QPixmap::fromImage(loc.marker()));
    }
}
void DialogLocation::displayMarker(const QPixmap& pic)
{
    const int sz = 24;
    if (pic.width() > sz || pic.height() > sz) {
        ui->imgMarker->setPixmap(pic.scaled(sz, sz, Qt::KeepAspectRatio));
    }
    else {
        ui->imgMarker->setPixmap(pic);
    }

}

void DialogLocation::on_tblLocation_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->accept();
}
