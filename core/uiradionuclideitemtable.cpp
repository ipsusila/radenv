#include <QHeaderView>
#include "uiradionuclideitemtable.h"
#include "uiradionuclideitemdelegate.h"

UiRadionuclideItemTable::UiRadionuclideItemTable(QWidget * parent)
    : UiAutoRowTable(parent), _firstColLabel(tr("Radionuclides")),
      _quantity(&Rad::EmptyQuantity), _types(KData::Undefined)
{
    setColumnCount(2);
    setRowCount(1);
    setColumnWidth(0, 120);

    UiRadionuclideItemDelegate * itemDelegate = new UiRadionuclideItemDelegate(this);
    this->setItemDelegate(itemDelegate);
    this->setToolTip(tr("Double click cell to input radionuclide and associated value"));
}
UiRadionuclideItemTable::~UiRadionuclideItemTable()
{
    //delete
}

QString UiRadionuclideItemTable::firstColumnLabel() const
{
    return _firstColLabel.isEmpty() ? tr("Radionuclides") : _firstColLabel;
}

void UiRadionuclideItemTable::setFirstColumnLabel(const QString& label)
{
    _firstColLabel = label;
}

KData UiRadionuclideItemTable::data() const
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
            if (_quantity->type == Rad::Boolean) {
                bool v = UiRadionuclideItemDelegate::stringToBoolean(_quantity, iVal->text());
                KDataItem d(nuc, v, _types);
                items.append(d);
            }
            else if (_quantity->isNumeric()) {
                qreal rate = iVal->text().toDouble();
                KDataItem d(nuc, rate, _types);
                items.append(d);
            }
            else {
                KDataItem d(nuc, iVal->text(), _types);
                items.append(d);
            }
        }

        if (items.isEmpty()) {
            return KData();
        }
        else {
            return KData(_quantity, items);
        }
    }
    return KData();
}
void UiRadionuclideItemTable::setData(const KData& d)
{
    //remember types
    _types |= d.contentTypes();

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
void UiRadionuclideItemTable::setData(const KDataArray& list)
{
    const KData & qi = list.find(_quantity->symbol);
    setData(qi);
}
void UiRadionuclideItemTable::setData(const KDataGroupArray& list)
{
    const KData & qi = list.find(*_quantity);
    setData(qi);
}

const Quantity * UiRadionuclideItemTable::quantity() const
{
    return _quantity;
}
void UiRadionuclideItemTable::setQuantity(const Quantity * s)
{
    if (_quantity != s) {
        _quantity = s;
        UiRadionuclideItemDelegate * delegate =
                qobject_cast<UiRadionuclideItemDelegate *>(this->itemDelegate());
        if (delegate != 0)
            delegate->setQuantity(s);

        //modify headers
        QStringList headers;
        headers << firstColumnLabel() << s->displayTextWithUnit();
        setHorizontalHeaderLabels(headers);

        //modify resize mode
        QHeaderView * header = this->horizontalHeader();
        if (header != 0) {
            header->setResizeMode(0, QHeaderView::Interactive);
            header->setResizeMode(1, QHeaderView::ResizeToContents);
        }
    }
}

