#ifndef KDATA_H
#define KDATA_H

#include <QVariant>
#include <QSharedDataPointer>
#include "klocation.h"
#include "ksymbolcontrol.h"

class KDataPrivate;
class K_CORE_EXPORT KData
{
private:
    QSharedDataPointer<KDataPrivate> data;
public:
    enum ContentType {
        Undefined = 0x00,

        Numeric = 0x01,
        Text = 0x02,
        Boolean = 0x04,
        Object = 0x08,

        Array = 0x10,
        Source = 0x20,
        Radionuclide = 0x40 | Numeric,
        Receptor = 0x80 | Numeric,
        RadionuclideArray = Array | Radionuclide,
        RadionuclideSource = Radionuclide | Source,
        RadionuclideSourceArray = RadionuclideSource | Array,
        ReceptorArray = Receptor | Array,
        Any = 0xFFFFFFFF
    };
    Q_DECLARE_FLAGS(ContentTypes, ContentType)

    KData();
    explicit KData(const Quantity * sym);
    explicit KData(const Quantity * var, const QVariant& v);
    explicit KData(const Quantity * sym, ContentTypes types, const QVariant & v);
    explicit KData(const Quantity * var, const QString& nm, const QVariant& v, ContentTypes f);
    explicit KData(const Quantity * var, const DataItemArray & items);
    KData(const KData&);
    KData(const Quantity * sym, const KData& other);
    KData &operator=(const KData&);
    virtual ~KData();

    const Quantity & symbol() const;
    const Quantity * symbolPtr() const;
    ContentTypes contentTypes() const;
    bool contains(ContentType type) const;
    int count() const;
    bool isValid() const;
    bool isEmpty() const;
    void resize(int cnt);
    qreal numericValue(int idx = 0) const;
    qreal numericValue(const QString& name, qreal defValue = 0) const;
    QVariant value(int idx = 0) const;
    void setValue(const QVariant &v);
    void setValue(int idx, const QVariant& v);
    void append(const QString& nm, const QVariant& v, ContentTypes t);

    const KDataItem & at(int idx) const;
    const KDataItem & at(const QString& name) const;

    KData operator*(qreal c) const;
    KData operator/(qreal c) const;
    KData & operator*=(qreal c);
    KData & operator/=(qreal c);

    bool operator==(const KData& o) const;
    bool operator!=(const KData& o) const;

    virtual QString displayText() const;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(KData::ContentTypes)

class K_CORE_EXPORT KDataItem
{
public:
    KDataItem();
    KDataItem(const QString& nm, const QVariant& v, KData::ContentTypes types);
    KDataItem(const KDataItem& other);
    KDataItem &operator=(const KDataItem& rhs);
    QString name() const;
    QVariant value() const;
    qreal numericValue() const;
    KData::ContentTypes contentTypes() const;
    void setValue(const QVariant& v);

    KDataItem operator*(qreal c) const;
    KDataItem operator/(qreal c) const;
    KDataItem & operator*=(qreal c);
    KDataItem & operator/=(qreal c);

private:
    QString _name;
    QVariant _value;
    KData::ContentTypes _types;
};

class K_CORE_EXPORT KDataArray : public DataList
{
public:
    KDataArray();
    KDataArray(const KDataArray& o);
    KDataArray(const DataList& o);

    QVariant valueOf(const Quantity & v) const;
    qreal numericValueOf(const Quantity& v) const;
    const KData & find(const QString& sym) const;
    const KData & find(const Quantity& v) const;
    QString displayText() const;

    void appendOrMerge(const Quantity * sym, const QString& nm,
                       const QVariant& v, KData::ContentTypes t);
    void appendOrReplace(const KData& di);
    void setLocation(const KLocation& loc);
    KLocation location() const;
    ConstSymbolList symbols() const;

private:
    KLocation _location;
};

class K_CORE_EXPORT KDataGroupArray : public QVector<DataGroup >
{
public:
    KDataGroupArray();
    explicit KDataGroupArray(const QString& name, const KDataArray & da);
    const KData & find(const Quantity& v) const;
    qreal numericValueOf(const Quantity& v) const;
    QVariant valueOf(const Quantity & v) const;
    ConstSymbolList symbols() const;
    KDataArray toDataArray() const;

    void replace(const KData& di);
    void addQuantityControl(const KSymbolControl & qc);
    const QuantityControlList & quantityControls() const;

private:
    QuantityControlList _controlList;
};

#endif // KDATA_H
