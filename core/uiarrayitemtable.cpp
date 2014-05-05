#include <QHeaderView>
#include "uiarrayitemtable.h"
#include "uiarrayitemdelegate.h"

UiArrayItemTable::UiArrayItemTable(KStorage * storage, QWidget * parent)
    : UiAutoRowTable(parent), _firstColLabel(tr("Radionuclides"))
{
    setColumnCount(2);
    setRowCount(1);
    setColumnWidth(0, 120);

    UiArrayItemDelegate * itemDelegate = new UiArrayItemDelegate(storage, this);
    this->setItemDelegate(itemDelegate);
    this->setToolTip(tr("Double click cell to input radionuclide and associated value"));
}
UiArrayItemTable::~UiArrayItemTable()
{
    //delete
}

QString UiArrayItemTable::firstColumnLabel() const
{
    return _firstColLabel.isEmpty() ? tr("Radionuclides") : _firstColLabel;
}

void UiArrayItemTable::setFirstColumnLabel(const QString& label)
{
    _firstColLabel = label;
}

KData UiArrayItemTable::data() const
{
    //convert to data
    if (rowCount() >= 1) {
        QTableWidgetItem * cell = this->item(0, 0);
        if (cell == 0 || cell->text().isEmpty())
            return KData();

        DataItemArray items;
        const Quantity * qty = quantity();
        KData::ContentTypes types = contentTypes();
        for (int k = 0; k < rowCount(); k++) {
            QTableWidgetItem * iNuc = item(k, 0);
            QTableWidgetItem * iVal = item(k, 1);
            if (iNuc == 0 || iNuc->text().isEmpty() || iVal == 0 || iVal->text().isEmpty())
                continue;

            QString nuc = iNuc->text();
            if (qty->type == Rad::Boolean) {
                bool v = UiArrayItemDelegate::stringToBoolean(qty, iVal->text());
                KDataItem d(nuc, v, types);
                items.append(d);
            }
            else if (qty->isNumeric()) {
                qreal rate = iVal->text().toDouble();
                KDataItem d(nuc, rate, types);
                items.append(d);
            }
            else {
                KDataItem d(nuc, iVal->text(), types);
                items.append(d);
            }
        }

        if (items.isEmpty()) {
            return KData();
        }
        else {
            return KData(qty, items);
        }
    }
    return KData();
}
void UiArrayItemTable::setData(const KData& d)
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
void UiArrayItemTable::setData(const KDataArray& list)
{
    const KData & qi = list.find(quantity()->symbol);
    setData(qi);
}
void UiArrayItemTable::setData(const KDataGroupArray& list)
{
    const KData & qi = list.find(*quantity());
    setData(qi);
}

const Quantity * UiArrayItemTable::quantity() const
{
    UiArrayItemDelegate * delegate =
            qobject_cast<UiArrayItemDelegate *>(this->itemDelegate());
    if (delegate != 0)
        return delegate->quantity();
    return &Rad::EmptyQuantity;
}
KData::ContentTypes UiArrayItemTable::contentTypes() const
{
    UiArrayItemDelegate * delegate =
            qobject_cast<UiArrayItemDelegate *>(this->itemDelegate());
    if (delegate != 0)
        return delegate->contentTypes();
    return KData::Undefined;
}
void UiArrayItemTable::setQuantity(const Quantity * s, KData::ContentTypes types)
{
    const Quantity * qty = quantity();
    if (qty != s) {
        UiArrayItemDelegate * delegate =
                qobject_cast<UiArrayItemDelegate *>(this->itemDelegate());
        if (delegate != 0)
            delegate->setQuantity(s, types);

        //modify headers
        QStringList headers;
        headers << firstColumnLabel() << s->displayTextWithUnit();
        setHorizontalHeaderLabels(headers);

        //modify resize mode
        QHeaderView * header = this->horizontalHeader();
        if (header != 0) {
#if QT_VERSION >= 0x050000
            header->setSectionResizeMode(0, QHeaderView::Interactive);
            header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#else
            header->setResizeMode(0, QHeaderView::Interactive);
            header->setResizeMode(1, QHeaderView::ResizeToContents);
#endif
        }
    }
}

