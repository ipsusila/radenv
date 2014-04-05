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
    void setSymbol(const Quantity * sym);
    const Quantity * symbol() const;

private:
    const Quantity * _symbol;
};

#endif // UIRADIONUCLIDEITEMDELEGATE_H
