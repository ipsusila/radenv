#ifndef CONSTANTVALUEDELEGATE_H
#define CONSTANTVALUEDELEGATE_H

#include <QStyledItemDelegate>
#include "radglobal.h"

class ConstantValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ConstantValueDelegate(QWidget *parent = 0);
    virtual ~ConstantValueDelegate();

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setDecimals(int dec);
    int decimals() const;
    const Quantity * quantity(const QString& name) const;
    void setQuantities(const ConstQuantityList & qty);

private:
    int _decimals;
    ConstQuantityList _quantities;
};

#endif // CONSTANTVALUEDELEGATE_H
