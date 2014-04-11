#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include "uiradionuclideitemdelegate.h"
#include "kstorage.h"
#include "kradionuclide.h"

UiRadionuclideItemDelegate::UiRadionuclideItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent), _quantity(&Rad::EmptyQuantity)
{
}
UiRadionuclideItemDelegate::~UiRadionuclideItemDelegate()
{
}

void UiRadionuclideItemDelegate::setQuantity(const Quantity * qty)
{
    _quantity = qty;
}
const Quantity * UiRadionuclideItemDelegate::quantity() const
{
    return _quantity;
}

QWidget* UiRadionuclideItemDelegate::createEditor( QWidget *parent,
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
        QWidget * w = 0;
        QDoubleSpinBox * sb;
        QCheckBox * chk;
        switch(_quantity->type) {
        case Rad::Boolean:
            chk = new QCheckBox(parent);
            chk->setText(_quantity->text);
            w = chk;
            break;
        case Rad::Integer:
        case Rad::Real:
            sb = new QDoubleSpinBox(parent);
            sb->setMinimum(_quantity->minValue);
            sb->setMaximum(_quantity->maxValue);
            sb->setDecimals(_quantity->decimal);
            w = sb;
            break;
        default:
            w = new QLineEdit(parent);
            break;
        }
        Q_ASSERT(w != 0);
        w->setToolTip(_quantity->description);
        return w;
    }
    else
        return QStyledItemDelegate::createEditor(parent, option, index);
}


void UiRadionuclideItemDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    if(cb) {
        // get the index of the text in the combobox that matches the current value of the itenm
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);
        // if it is valid, adjust the combobox
        if(cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);
        return;
    }

    QDoubleSpinBox * sb = qobject_cast<QDoubleSpinBox *>(editor);
    if (sb) {
        QVariant data = index.data(Qt::EditRole);
        if (data.isValid())
            sb->setValue(data.toDouble());
        else
            sb->setValue(_quantity->defaultValue);
        return;
    }

    QLineEdit * ed = qobject_cast<QLineEdit *>(editor);
    if (ed) {
        QVariant data = index.data(Qt::EditRole);
        if (data.isValid())
            ed->setText(data.toString());
        else if (_quantity->isNumeric())
            ed->setText(QString::number(_quantity->defaultValue));
        return;
    }

    QCheckBox * chk = qobject_cast<QCheckBox *>(editor);
    if (chk) {
        /*
        QVariant data = index.data(Qt::EditRole);
        if (data.isValid())
            chk->setChecked(data.toBool());
        else
            chk->setChecked((bool)_quantity->defaultValue);
        */
        QString dataStr = index.data(Qt::EditRole).toString();
        chk->setChecked(stringToBoolean(_quantity, dataStr));
        return;
    }


    QStyledItemDelegate::setEditorData(editor, index);
}


void UiRadionuclideItemDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    if(cb) {
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);
        return;
    }

    QDoubleSpinBox * sb = qobject_cast<QDoubleSpinBox *>(editor);
    if (sb) {
        model->setData(index, sb->value(), Qt::EditRole);
        return;
    }

    QLineEdit * ed = qobject_cast<QLineEdit *>(editor);
    if (ed) {
        QString str = ed->text();
        if (_quantity->isNumeric()) {
            qreal v = str.toDouble();
            if (v < _quantity->minValue)
                model->setData(index, _quantity->minValue, Qt::EditRole);
            else if (v > _quantity->maxValue)
                model->setData(index, _quantity->maxValue, Qt::EditRole);
            else
                model->setData(index, v, Qt::EditRole);
        }
        else {
            model->setData(index, str, Qt::EditRole);
        }
        return;
    }

    QCheckBox * chk = qobject_cast<QCheckBox *>(editor);
    if (chk) {
        //model->setData(index, chk->isChecked(), Qt::EditRole);
        if (chk->isChecked())
            model->setData(index, chk->text() + " Yes", Qt::EditRole);
        else
            model->setData(index, chk->text() + " No", Qt::EditRole);
        return;
    }

    QStyledItemDelegate::setModelData(editor, model, index);
}

bool UiRadionuclideItemDelegate::stringToBoolean(const Quantity * qty, const QString & txt)
{
    if (txt.endsWith("Yes"))
        return true;
    else if (txt.endsWith("No"))
        return false;
    else
        return qty->defaultValue;
}

