#ifndef BIOACCUMULATIONFACTOR_H
#define BIOACCUMULATIONFACTOR_H

#include <QSharedDataPointer>
#include "kstoragecontent.h"

typedef struct _tagBp
{
    QString element;
    qreal freshwaterFish;
    qreal marineFish;
    qreal marineShellfish;

    inline bool isValid() const
    {
        return (freshwaterFish >= 0 || marineFish >= 0 || marineShellfish >= 0);
    }
} BpValue;

inline QDataStream & operator<<(QDataStream &s, const BpValue & item)
{
    s << item.element << item.freshwaterFish << item.marineFish << item.marineShellfish;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, BpValue & item)
{
    s >> item.element >> item.freshwaterFish >> item.marineFish >> item.marineShellfish;
    return s;
}

class BioAccumulationFactorPrivate;
class BioAccumulationFactor
{
public:
    enum ValueType { FreshWaterFish = 1, MarineFish, MarineShellfish };

    explicit BioAccumulationFactor(IModelFactory * factory, KStorage * storage = 0);
    BioAccumulationFactor(const BioAccumulationFactor &);
    BioAccumulationFactor &operator=(const BioAccumulationFactor &);
    ~BioAccumulationFactor();

    bool save();
    bool saveDefault();
    bool load(bool createOnEmpty = false);

    qreal value(const QString& nuclide, ValueType type) const;
    BpValue value(const QString& nuclide) const;
    
private:
    QSharedDataPointer<BioAccumulationFactorPrivate> data;
};

#endif // BIOACCUMULATIONFACTOR_H
