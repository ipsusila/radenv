#include "constantvaluetable.h"
#include "constantvaluedelegate.h"

ConstantValueTable::ConstantValueTable(QWidget *parent) :
    UiAutoRowTable(parent)
{
    setColumnCount(3);
    setRowCount(1);
    setColumnWidth(0, 120);
    setColumnWidth(1, 120);
    setColumnWidth(2, 340);

    ConstantValueDelegate * itemDelegate = new ConstantValueDelegate(this);
    this->setItemDelegate(itemDelegate);
    this->setToolTip(tr("Double click cell to input value"));

    QStringList headers;
    headers << tr("Symbol (unit)") << tr("Value") << tr("Remarks");
    setHorizontalHeaderLabels(headers);
}
ConstantValueTable::~ConstantValueTable()
{
}
void ConstantValueTable::setDecimals(int d)
{
    ConstantValueDelegate * del = qobject_cast<ConstantValueDelegate *>(this->itemDelegate());
    Q_ASSERT(del != 0);
    del->setDecimals(d);
}

void ConstantValueTable::setSymbols(const ConstSymbolList & sym)
{
    ConstantValueDelegate * del = qobject_cast<ConstantValueDelegate *>(this->itemDelegate());
    Q_ASSERT(del != 0);
    del->setSymbols(sym);
}

void ConstantValueTable::setData(const KDataArray & da)
{
    int ncount = da.count();
    int nrow = ncount+1;

    //set row count
    this->setRowCount(nrow);

    //setup data
    int row = 0;
    for(int k = 0; k < ncount; k++) {
        const KData &item = da.at(k);
        //if multiple value/radionuclide related data, continue
        if (item.contains(KData::Radionuclide)) {
            nrow--;
            continue;
        }

        const Quantity * sym = item.symbolPtr();
        QTableWidgetItem * cell = new QTableWidgetItem(sym->displaySymbolWithUnit());
        cell->setData(Qt::UserRole, qVariantFromValue((void*)sym));
        setItem(row, 0, cell);

        cell = new QTableWidgetItem(item.value().toString());
        setItem(row, 1, cell);

        cell = new QTableWidgetItem(sym->description);
        setItem(row, 2, cell);

        row++;
    }
    //final row count
    this->setRowCount(nrow);
}

KDataArray ConstantValueTable::data() const
{
    KDataArray result;
    //convert to data
    if (rowCount() >= 1) {
        QTableWidgetItem * cell = this->item(0, 0);
        if (cell == 0 || cell->text().isEmpty())
            return result;

        for (int k = 0; k < rowCount(); k++) {
            QTableWidgetItem * iSym = item(k, 0);
            QTableWidgetItem * iVal = item(k, 1);
            if (iSym == 0 || iSym->text().isEmpty() || iVal == 0 || iVal->text().isEmpty())
                continue;

            QVariant vSym = iSym->data(Qt::UserRole);
            if (!vSym.isValid() || !vSym.isNull()) {
                qreal val = iVal->text().toDouble();
                const Quantity * sym = static_cast<const Quantity *>(vSym.value<void*>());
                result << KData(sym, val);
            }
        }

        return result;
    }
    return result;
}

