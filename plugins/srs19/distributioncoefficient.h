#ifndef DISTRIBUTIONCOEFFICIENT_H
#define DISTRIBUTIONCOEFFICIENT_H

#include <QSharedDataPointer>
#include "kstoragecontent.h"

/**
 * Distribution coefficient value
 */
typedef struct _tagKd {
    QString element;
    qreal freshWater;
    qreal saltWater;

    inline bool isValid() const
    {
        return freshWater >= 0 || saltWater >= 0;
    }
} KdValue;

inline QDataStream & operator<<(QDataStream &s, const KdValue & item)
{
    s << item.element << item.freshWater << item.saltWater;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, KdValue & item)
{
    s >> item.element >> item.freshWater >> item.saltWater;
    return s;
}

/**
 * Distribution coefficient
 */
class DistributionCoefficientPrivate;
class DistributionCoefficient
{
public:
    explicit DistributionCoefficient(IModelFactory * factory, KStorage * storage = 0);
    DistributionCoefficient(const DistributionCoefficient & o);
    DistributionCoefficient & operator=(const DistributionCoefficient & o);
    ~DistributionCoefficient();

    bool save();
    bool saveDefault();
    bool load(bool createOnEmpty = false);

    qreal value(const QString& nuclide, bool saltWater) const;
    KdValue value(const QString& nuclide) const;
private:
    QSharedDataPointer<DistributionCoefficientPrivate> data;
};

#endif // DISTRIBUTIONCOEFFICIENT_H
