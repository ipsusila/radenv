#include <QDoubleSpinBox>
#include <QComboBox>
#include "constantvaluedelegate.h"
#include "quantity.h"

ConstantValueDelegate::ConstantValueDelegate(QWidget *parent) :
    QStyledItemDelegate(parent), _decimals(4)
{
    _quantities = Srs19::availableQuantities();
}
ConstantValueDelegate::~ConstantValueDelegate()
{
}

void ConstantValueDelegate::setQuantities(const ConstQuantityList & qty)
{
    _quantities = qty;
}

const Quantity * ConstantValueDelegate::quantity(const QString &name) const
{
    foreach(const Quantity * qty, _quantities)
        if (qty->symbol == name)
            return qty;
    return 0;
}

void ConstantValueDelegate::setDecimals(int dec)
{
    _decimals = dec;
}

int ConstantValueDelegate::decimals() const
{
    return _decimals;
}

QWidget* ConstantValueDelegate::createEditor( QWidget *parent,
                                              const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option);

    // ComboBox at item 1, QDoubleSpinBox at item 2
    if(index.column() == 0) {
        //create combobox
        QComboBox * cb = new QComboBox(parent);

        //add quantities
        cb->addItem("");
        foreach(const Quantity * qty, _quantities) {
            if (qty->isNumeric())
                cb->addItem(qty->displayQuantityWithUnit(), qVariantFromValue((void*)qty));
        }

        return cb;
    }
    else if (index.column() == 1) {
        QDoubleSpinBox * sb = new QDoubleSpinBox(parent);
        sb->setMaximum(99999999999);
        sb->setDecimals(_decimals);
        return sb;
    }
    else
        return 0;
        //return QStyledItemDelegate::createEditor(parent, option, index);
}


void ConstantValueDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    QDoubleSpinBox * sb = qobject_cast<QDoubleSpinBox *>(editor);

    if(cb) {
        // get the index of the text in the combobox that matches the current value of the itenm
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);
        // if it is valid, adjust the combobox
        if(cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);
    }
    else if (sb) {
        qreal v = index.data(Qt::EditRole).toDouble();
        sb->setValue(v);
    }
    else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}


void ConstantValueDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    QDoubleSpinBox * sb = qobject_cast<QDoubleSpinBox *>(editor);
    if(cb) {
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);

        //get the 3rd column
        QModelIndex idx2 = index.sibling(index.row(), index.column()+1);
        QModelIndex idx3 = index.sibling(index.row(), index.column()+2);

        // add the description at 3rd column
        QVariant vSym = cb->itemData(cb->currentIndex());
        if (vSym.isValid() && !vSym.isNull()) {
            const Quantity * qty = static_cast<const Quantity *>(vSym.value<void*>());
            model->setData(index, vSym, Qt::UserRole);
            model->setData(idx2, qty->defaultValue, Qt::EditRole);
            model->setData(idx3, qty->description, Qt::EditRole);
        }
        else {
            model->setData(idx2, 0, Qt::EditRole);
            model->setData(idx3, "", Qt::EditRole);
        }
    }
    else if (sb)
        model->setData(index, sb->value(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

