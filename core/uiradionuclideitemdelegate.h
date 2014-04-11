#ifndef UIRADIONUCLIDEITEMDELEGATE_H
#define UIRADIONUCLIDEITEMDELEGATE_H

#include "radglobal.h"
#include <QStyledItemDelegate>

class K_CORE_EXPORT UiRadionuclideItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UiRadionuclideItemDelegate(QObject *parent = 0);
    virtual ~UiRadionuclideItemDelegate();
    
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setQuantity(const Quantity * qty);
    const Quantity * quantity() const;

    static bool stringToBoolean(const Quantity * qty, const QString & txt);

private:
    const Quantity * _quantity;
};

#endif // UIRADIONUCLIDEITEMDELEGATE_H
