#ifndef CONSTANTVALUETABLE_H
#define CONSTANTVALUETABLE_H

#include "kdata.h"
#include "uiautorowtable.h"

class ConstantValueTable : public UiAutoRowTable
{
public:
    explicit ConstantValueTable(QWidget *parent = 0);
    ~ConstantValueTable();
    void setDecimals(int d);
    void setQuantities(const ConstQuantityList & qty);

    void setData(const KDataArray & da);
    KDataArray data() const;
};

#endif // CONSTANTVALUETABLE_H
