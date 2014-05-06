#ifndef UIAUTOROWTABLE_H
#define UIAUTOROWTABLE_H

#include "radglobal.h"
#include <QTableWidget>

class RADENV_API UiAutoRowTable : public QTableWidget
{
    Q_OBJECT
protected:
    explicit UiAutoRowTable(QWidget *parent = 0);
    
    virtual void changeCell(int row, int column);

public:
    virtual ~UiAutoRowTable();

public slots:
    void onCellChanged(int row, int column);
};

#endif // UIAUTOROWTABLE_H
