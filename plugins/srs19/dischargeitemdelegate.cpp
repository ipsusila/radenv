#include <QDoubleSpinBox>
#include <QComboBox>
#include "dischargeitemdelegate.h"
#include "kstorage.h"
#include "kradionuclide.h"

/*
 * reference : http://qt-project.org/wiki/Combo_Boxes_in_Item_Views
 */

DischargeItemDelegate::DischargeItemDelegate(QWidget *parent) :
    QStyledItemDelegate(parent), _decimals(4)
{
}
DischargeItemDelegate::~DischargeItemDelegate()
{
}

void DischargeItemDelegate::setDecimals(int dec)
{
    this->_decimals = dec;
}
int DischargeItemDelegate::decimals() const
{
    return _decimals;
}

QWidget* DischargeItemDelegate::createEditor( QWidget *parent,
                                              const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    // ComboBox at item 1, QDoubleSpinBox at item 2
    if(index.column() == 0) {
        //create combobox
        QComboBox * cb = new QComboBox(parent);
        cb->addItem("");

        //add radionuclide
        const RadionuclideList * items = KStorage::storage()->radionuclides();
        foreach(KRadionuclide nuc, *items) {
            cb->addItem(nuc.nuclide());
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
        return QStyledItemDelegate::createEditor(parent, option, index);
}


void DischargeItemDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
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


void DischargeItemDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    QDoubleSpinBox * sb = qobject_cast<QDoubleSpinBox *>(editor);
    if(cb)
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);
    else if (sb)
        model->setData(index, sb->value(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
