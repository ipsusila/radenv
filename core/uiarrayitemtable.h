#ifndef UIARRAYITEMTABLE_H
#define UIARRAYITEMTABLE_H

#include "uiautorowtable.h"
#include "kdata.h"

class K_CORE_EXPORT UiArrayItemTable : public UiAutoRowTable
{
    Q_OBJECT
public:
    explicit UiArrayItemTable(KStorage * storage, QWidget * parent = 0);
    ~UiArrayItemTable();

    KData data() const;
    void setData(const KData& d);
    void setData(const KDataArray& list);
    void setData(const KDataGroupArray& list);
    KData::ContentTypes contentTypes() const;
    const Quantity * quantity() const;
    void setQuantity(const Quantity * s, KData::ContentTypes types);
    QString firstColumnLabel() const;
    void setFirstColumnLabel(const QString& label);

private:
    QString _firstColLabel;
};

#endif // UIARRAYITEMTABLE_H
