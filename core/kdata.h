#ifndef KDATA_H
#define KDATA_H

#include <QVariant>
#include <QSharedDataPointer>
#include "radglobal.h"

class KDataPrivate;
class K_CORE_EXPORT KDataItem
{
public:
    KDataItem();
    KDataItem(const QString& nm, const QVariant& v);
    KDataItem(const KDataItem& other);
    KDataItem &operator=(const KDataItem& rhs);
    QString name() const;
    QVariant value() const;
    qreal numericValue() const;
    void setValue(const QVariant& v);

    KDataItem operator*(qreal c) const;
    KDataItem operator/(qreal c) const;
    KDataItem & operator*=(qreal c);
    KDataItem & operator/=(qreal c);

private:
    QString _name;
    QVariant _value;
};

class K_CORE_EXPORT KData
{
private:
    QSharedDataPointer<KDataPrivate> data;
public:
    KData();
    explicit KData(const Symbol * var, const QVariant& v);
    explicit KData(const Symbol * var, const QVector<KDataItem> & items);
    KData(const KData&);
    KData(const Symbol * sym, const KData& other);
    KData &operator=(const KData&);
    virtual ~KData();

    const Symbol & symbol() const;
    int count() const;
    void resize(int cnt);
    bool isValid() const;
    QVariant value(int idx = 0) const;
    void setValue(const QVariant &v);
    void setValue(int idx, const QVariant& v);
    qreal numericValue(int idx = 0) const;

    const KDataItem & at(int idx) const;
    KDataItem & operator[](int idx);
    const KDataItem& operator[](int idx) const;

    KData operator*(qreal c) const;
    KData operator/(qreal c) const;
    KData & operator*=(qreal c);
    KData & operator/=(qreal c);

    bool operator==(const KData& o) const;
    bool operator!=(const KData& o) const;

    virtual QString displayText() const;
};

class K_CORE_EXPORT KDataArray : public QList<KData>
{
public:
    KDataArray();
    KDataArray(const KDataArray& o);
    KDataArray(const QList<KData>& o);

    QVariant valueOf(const Symbol & v) const;
    qreal numericValueOf(const Symbol& v) const;
    const KData & find(const QString& sym) const;
    const KData & find(const Symbol& v) const;
    QString displayText() const;

    void addOrReplace(const KData& di);
};

class K_CORE_EXPORT KDataGroupArray : public QVector<KGroup<KData> >
{
public:
    //DataGroupArray();

    const KData & find(const Symbol& v) const;
    qreal numericValueOf(const Symbol& v) const;
    QVariant valueOf(const Symbol & v) const;
};

#endif // KDATA_H
