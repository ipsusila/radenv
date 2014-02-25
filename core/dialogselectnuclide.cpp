#include <QDebug>
#include "dialogselectnuclide.h"
#include "ui_dialogselectnuclide.h"
#include "kstorage.h"
#include "kradionuclide.h"

DialogSelectNuclide::DialogSelectNuclide(const QStringList & selectedList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectNuclide)
{
    ui->setupUi(this);
    setupRadionuclides(selectedList);
}

DialogSelectNuclide::~DialogSelectNuclide()
{
    qDebug() << "Dialog select nuclide destroyed";
    delete ui;
}

void DialogSelectNuclide::setupRadionuclides(const QStringList & selectedList)
{
    const RadionuclideList * sources = KStorage::storage()->radionuclides();
    QStringList items;

    foreach(KRadionuclide nuc, *sources) {
        if (!selectedList.contains(nuc.nuclide()))
            items << nuc.nuclide();
    }
    ui->lstSource->addItems(items);
    ui->lstSource->sortItems();

    ui->lstDestination->addItems(selectedList);
    ui->lstDestination->sortItems();

    //selection mode
    ui->lstSource->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->lstDestination->setSelectionMode(QAbstractItemView::MultiSelection);
}

QStringList DialogSelectNuclide::selectedRadionuclides() const
{
    QStringList items;
    for (int k = 0; k < ui->lstDestination->count(); k++) {
        items << ui->lstDestination->item(k)->text();
    }

    return items;
}

void DialogSelectNuclide::on_cmdSelect_clicked()
{
    QListWidget * widget = ui->lstSource;
    QList<QListWidgetItem *> items = widget->selectedItems();
    for(int k = 0; k < items.size(); k++) {
        QListWidgetItem * item = widget->takeItem(widget->row(items[k]));
        ui->lstDestination->addItem(item);
    }
    ui->lstDestination->sortItems();
}

void DialogSelectNuclide::on_cmdSelectAll_clicked()
{
    while (ui->lstSource->count() > 0) {
        QListWidgetItem * item = ui->lstSource->takeItem(0);
        ui->lstDestination->addItem(item);
    }
    ui->lstDestination->sortItems();
}

void DialogSelectNuclide::on_cmdDeselect_clicked()
{
    QListWidget * widget = ui->lstDestination;
    QList<QListWidgetItem *> items = widget->selectedItems();
    for(int k = 0; k < items.size(); k++) {
        QListWidgetItem * item = widget->takeItem(widget->row(items[k]));
        ui->lstSource->addItem(item);
    }
    ui->lstSource->sortItems();
}

void DialogSelectNuclide::on_cmdDeselectAll_clicked()
{
    while (ui->lstDestination->count() > 0) {
        QListWidgetItem * item = ui->lstDestination->takeItem(0);
        ui->lstSource->addItem(item);
    }
    ui->lstSource->sortItems();
}
