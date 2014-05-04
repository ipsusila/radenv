#ifndef DISCHARGEITEMTABLE_H
#define DISCHARGEITEMTABLE_H

#include "uiautorowtable.h"
#include "kdata.h"
#include "quantity.h"

class DischargeItemTable : public UiAutoRowTable
{
public:
    explicit DischargeItemTable(QWidget *parent = 0);
    ~DischargeItemTable();

    KData data() const;
    void setData(const KData& d);
    void setData(const KDataArray& list);
    void setData(const KDataGroupArray& list);
    const Quantity * variable() const;
    void setVariable(const Quantity * s);
    void setDecimals(int dec);
    int decimals() const;
    void setStorage(KStorage * stg);

private:
    const Quantity * _variable;
};

#endif // DISCHARGEITEMTABLE_H
