#ifndef DISCHARGEITEMDELEGATE_H
#define DISCHARGEITEMDELEGATE_H

#include <QStyledItemDelegate>

class DischargeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DischargeItemDelegate(QWidget *parent = 0);
    virtual ~DischargeItemDelegate();

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setDecimals(int dec);
    int decimals() const;

private:
    int _decimals;
};

#endif // DISCHARGEITEMDELEGATE_H
