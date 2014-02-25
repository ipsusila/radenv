#ifndef DISCHARGEITEMTABLE_H
#define DISCHARGEITEMTABLE_H

#include <QTableWidget>
#include "kdata.h"
#include "symbol.h"

class DischargeItemTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit DischargeItemTable(QWidget *parent = 0);
    ~DischargeItemTable();

    KData data() const;
    void setData(const KData& d);
    void setData(const KDataArray& list);
    const Symbol * variable() const;
    void setVariable(const Symbol * s);
    
public slots:
    void onCellChanged(int row, int column);

private:
    const Symbol * _variable;
};

#endif // DISCHARGEITEMTABLE_H
