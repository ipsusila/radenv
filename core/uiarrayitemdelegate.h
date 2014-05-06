#ifndef UIARRAYITEMDELEGATE_H
#define UIARRAYITEMDELEGATE_H

#include "kdata.h"
#include <QStyledItemDelegate>

class RADENV_API UiArrayItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UiArrayItemDelegate(KStorage * storage, QObject *parent = 0);
    virtual ~UiArrayItemDelegate();
    
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setQuantity(const Quantity * qty, KData::ContentTypes types);
    const Quantity * quantity() const;
    KData::ContentTypes contentTypes() const;

    static bool stringToBoolean(const Quantity * qty, const QString & txt);

private:
    KStorage * _storage;
    const Quantity * _quantity;
    KData::ContentTypes _types;
};

#endif // UIARRAYITEMDELEGATE_H
