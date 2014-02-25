#include "dischargeitemtable.h"
#include "dischargeitemdelegate.h"
#include "symbol.h"

DischargeItemTable::DischargeItemTable(QWidget *parent) :
    QTableWidget(parent), _variable(&Rad::EmptySymbol)
{
    setColumnCount(2);
    setRowCount(1);
    setColumnWidth(0, 120);
    setColumnWidth(1, 230);

    DischargeItemDelegate * itemDelegate = new DischargeItemDelegate(this);
    this->setItemDelegate(itemDelegate);

    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged(int,int)));
    this->setToolTip(tr("Discharge rate table. Double click cell to input radionuclide and discharge rate"));
}
DischargeItemTable::~DischargeItemTable()
{
    //delete
}

void DischargeItemTable::onCellChanged(int row, int column)
{
    if (column == 0) {
        bool lastRow = (row + 1) == this->rowCount();
        QTableWidgetItem * item = this->item(row, column);
        if (lastRow) {
            if (!item->text().isEmpty())
                this->setRowCount(row+2);
        }
        else {
            if (item->text().isEmpty()) {
                this->removeRow(row);
            }
        }
    }
}

KData DischargeItemTable::data() const
{
    //convert to data
    if (rowCount() >= 1) {
        QTableWidgetItem * cell = this->item(0, 0);
        if (cell == 0 || cell->text().isEmpty())
            return KData();

        QVector<KDataItem> items;
        for (int k = 0; k < rowCount(); k++) {
            QTableWidgetItem * iNuc = item(k, 0);
            QTableWidgetItem * iRate = item(k, 1);
            if (iNuc == 0 || iNuc->text().isEmpty() || iRate == 0 || iRate->text().isEmpty())
                continue;

            QString nuc = iNuc->text();
            qreal rate = iRate->text().toDouble();

            if (!nuc.isEmpty() && rate > 0.0) {
                KDataItem d(nuc, rate);
                items.append(d);
            }
        }

        if (items.isEmpty()) {
            return KData();
        }
        else {
            return KData(_variable, items);
        }
    }
    return KData();
}
void DischargeItemTable::setData(const KData& d)
{
    //set row count
    this->setRowCount(d.count()+1);

    //setup data
    for(int k = 0; k < d.count(); k++) {
        const KDataItem &item = d.at(k);
        QTableWidgetItem * cell = new QTableWidgetItem(item.name());
        setItem(k, 0, cell);

        cell = new QTableWidgetItem(item.value().toString());
        setItem(k, 1, cell);
    }
}
void DischargeItemTable::setData(const KDataArray& list)
{
    KData qi = list.find(_variable->symbol);
    if (qi.isValid()) {
        setData(qi);
    }
}

const Symbol * DischargeItemTable::variable() const
{
    return _variable;
}
void DischargeItemTable::setVariable(const Symbol * s)
{
    if (_variable != s) {
        _variable = s;
        QStringList headers;
        headers << tr("Radionuclides") << QString("%1 (%2)").arg(s->text).arg(s->unit);
        setHorizontalHeaderLabels(headers);
    }
}
