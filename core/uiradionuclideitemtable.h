#ifndef UIRADIONUCLIDEITEMTABLE_H
#define UIRADIONUCLIDEITEMTABLE_H

#include "uiautorowtable.h"
#include "kdata.h"

class K_CORE_EXPORT UiRadionuclideItemTable : public UiAutoRowTable
{
    Q_OBJECT
public:
    explicit UiRadionuclideItemTable(QWidget * parent = 0);
    ~UiRadionuclideItemTable();

    KData data() const;
    void setData(const KData& d);
    void setData(const KDataArray& list);
    void setData(const KDataGroupArray& list);
    const Quantity * quantity() const;
    void setQuantity(const Quantity * s);
    QString firstColumnLabel() const;
    void setFirstColumnLabel(const QString& label);

private:
    QString _firstColLabel;
    const Quantity * _quantity;
    KData::ContentTypes _types;
};

#endif // UIRADIONUCLIDEITEMTABLE_H
