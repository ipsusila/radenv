#ifndef TRANSFERFACTOR_H
#define TRANSFERFACTOR_H

#include <QSharedDataPointer>
#include "kstoragecontent.h"

typedef struct _tagFv
{
    QString element;
    qreal forage;
    qreal crops;
    qreal milk;
    qreal meat;

    inline bool isValid() const
    {
        return (forage >= 0 || crops >= 0 || milk >= 0 || meat >= 0);
    }
} FvValue;

inline QDataStream & operator<<(QDataStream &s, const FvValue & item)
{
    s << item.element << item.forage << item.crops << item.milk << item.meat;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, FvValue & item)
{
    s >> item.element >> item.forage >> item.crops >> item.milk >> item.meat;
    return s;
}

class TransferFactorPrivate;
class TransferFactor
{
public:
    enum ValueType { Forage = 1, Crops, Milk, Meat };

    explicit TransferFactor(IModelFactory * factory, KStorage * storage = 0);
    TransferFactor(const TransferFactor &);
    TransferFactor &operator=(const TransferFactor &);
    ~TransferFactor();

    bool save();
    bool saveDefault();
    bool load(bool createOnEmpty = false);

    qreal value(const QString& nuclide, ValueType type) const;
    FvValue value(const QString& nuclide) const;
    
private:
    QSharedDataPointer<TransferFactorPrivate> data;
};

#endif // TRANSFERFACTOR_H
