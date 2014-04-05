#include "dischargeitemtable.h"
#include "dischargeitemdelegate.h"
#include "symbol.h"

DischargeItemTable::DischargeItemTable(QWidget *parent) :
    UiAutoRowTable(parent), _variable(&Rad::EmptySymbol)
{
    setColumnCount(2);
    setRowCount(1);
    setColumnWidth(0, 120);
    setColumnWidth(1, 230);

    DischargeItemDelegate * itemDelegate = new DischargeItemDelegate(this);
    this->setItemDelegate(itemDelegate);
    this->setToolTip(tr("Double click cell to input radionuclide and associated value"));
}
DischargeItemTable::~DischargeItemTable()
{
    //delete
}
void DischargeItemTable::setDecimals(int dec)
{
    DischargeItemDelegate * delegate = qobject_cast<DischargeItemDelegate *>(this->itemDelegate());
    if (delegate != 0)
        delegate->setDecimals(dec);
}

int DischargeItemTable::decimals() const
{
    DischargeItemDelegate * delegate = qobject_cast<DischargeItemDelegate *>(this->itemDelegate());
    if (delegate != 0)
        return delegate->decimals();
    return 0;
}

KData DischargeItemTable::data() const
{
    //convert to data
    if (rowCount() >= 1) {
        QTableWidgetItem * cell = this->item(0, 0);
        if (cell == 0 || cell->text().isEmpty())
            return KData();

        DataItemArray items;
        for (int k = 0; k < rowCount(); k++) {
            QTableWidgetItem * iNuc = item(k, 0);
            QTableWidgetItem * iVal = item(k, 1);
            if (iNuc == 0 || iNuc->text().isEmpty() || iVal == 0 || iVal->text().isEmpty())
                continue;

            QString nuc = iNuc->text();
            qreal rate = iVal->text().toDouble();

            if (!nuc.isEmpty() && rate > 0.0) {
                KDataItem d(nuc, rate, KData::Radionuclide);
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
    if (d.isEmpty())
        return;

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
    const KData & qi = list.find(_variable->symbol);
    setData(qi);
}
void DischargeItemTable::setData(const KDataGroupArray& list)
{
    const KData & qi = list.find(*_variable);
    setData(qi);
}

const Quantity * DischargeItemTable::variable() const
{
    return _variable;
}
void DischargeItemTable::setVariable(const Quantity * s)
{
    if (_variable != s) {
        _variable = s;
        QStringList headers;
        headers << tr("Radionuclides") << s->displayTextWithUnit();
        setHorizontalHeaderLabels(headers);
    }
}
