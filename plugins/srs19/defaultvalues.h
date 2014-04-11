#ifndef DEFAULTVALUES_H
#define DEFAULTVALUES_H

#include "defaultvaluetable.h"

//////////////////////////////////// table XI //////////////////////////////////////
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
class DistributionCoefficient : public DefaultValueTable<KdValue>
{
public:
    enum { Freshwater = 0, SaltWater = 1 };
    explicit DistributionCoefficient(IModelFactory * factory, KStorage * storage = 0);
};


//////////////////////////////////// table XI //////////////////////////////////////
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

class TransferFactor : public DefaultValueTable<FvValue>
{
public:
    enum { Forage = 0, Crops = 1, Milk = 2, Meat = 3 };

    explicit TransferFactor(IModelFactory * factory, KStorage * storage = 0);
};


//////////////////////////////////// table XIII //////////////////////////////////////
typedef struct _tagBp
{
    QString element;
    qreal freshwaterFish;
    qreal marineFish;
    qreal marineShellfish;

    inline bool isValid() const
    {
        return !element.isEmpty() && (freshwaterFish >= 0 || marineFish >= 0 || marineShellfish >= 0);
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

class BioAccumulationFactor : public DefaultValueTable<BpValue>
{
public:
    enum { FreshwaterFish = 0, MarineFish = 1, MarineShellfish = 2 };

    explicit BioAccumulationFactor(IModelFactory * factory, KStorage * storage = 0);
};

//////////////////////////////////// table XV //////////////////////////////////////
typedef struct _tagExternalDoseCoeff
{
    QString nuclide;
    qreal immersionDose;
    qreal skinDose;
    qreal surfaceDeposit;

    inline bool isValid() const {
        return !nuclide.isEmpty();
    }
} ExtDfValue;

inline QDataStream & operator<<(QDataStream &s, const ExtDfValue & item)
{
    s << item.nuclide << item.immersionDose << item.skinDose << item.surfaceDeposit;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, ExtDfValue & item)
{
    s >> item.nuclide >> item.immersionDose >> item.skinDose >> item.surfaceDeposit;
    return s;
}

class ExternalDoseCoefficient : public DefaultValueTable<ExtDfValue>
{
public:
    enum {ImmersionDose = 0, SkinDose = 1, SurfaceDeposit = 2};

    explicit ExternalDoseCoefficient(IModelFactory * fact, KStorage * storage = 0);
};

//////////////////////////////////// table XVI /////////////////////////////////////
typedef struct _tagInhalationDoseCoeff
{
    QString nuclide;
    qreal infant;           //infant inhalation dose coeff
    qreal adult;            //adult inhalation dose coeff
    uchar lungClass;         //classification of radionuclide
} InhDfValue;

inline QDataStream & operator<<(QDataStream &s, const InhDfValue & item)
{
    s << item.nuclide << item.infant << item.adult << item.lungClass;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, InhDfValue & item)
{
    s >> item.nuclide >> item.infant >> item.adult >> item.lungClass;
    return s;
}

class InhalationDoseCoefficient : public DefaultValueTable<InhDfValue>
{
public:
    enum {Infant = 0, Adult = 1};

    explicit InhalationDoseCoefficient(IModelFactory * fact, KStorage * storage = 0);
};

//////////////////////////////////// table XVII /////////////////////////////////////
typedef struct _tagIngestionDoseCoeff
{
    QString nuclide;
    qreal infant;
    qreal adult;
    qreal gutTransFactor;
} IngDfValue;

inline QDataStream & operator<<(QDataStream &s, const IngDfValue & item)
{
    s << item.nuclide << item.infant << item.adult << item.gutTransFactor;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, IngDfValue & item)
{
    s >> item.nuclide >> item.infant >> item.adult >> item.gutTransFactor;
    return s;
}

class IngestionDoseCoefficient : public DefaultValueTable<IngDfValue>
{
public:
    enum {Infant = 0, Adult = 1};

    explicit IngestionDoseCoefficient(IModelFactory * fact, KStorage * storage = 0);
};

//////////////////////////////// table XIX, XX, XXI //////////////////////////////////
typedef struct _tagDoseCommitment
{
    QString nuclide;
    qreal effDose;
} DoseCommValue;

inline QDataStream & operator<<(QDataStream &s, const DoseCommValue & item)
{
    s << item.nuclide << item.effDose;
    return s;
}
inline QDataStream & operator>>(QDataStream &s, DoseCommValue & item)
{
    s >> item.nuclide >> item.effDose;
    return s;
}

class AtmosphereDoseCommitment : public DefaultValueTable<DoseCommValue>
{
public:
    explicit AtmosphereDoseCommitment(IModelFactory * fact, KStorage * storage = 0);
};

class MarineWaterDoseCommitment : public DefaultValueTable<DoseCommValue>
{
public:
    explicit MarineWaterDoseCommitment(IModelFactory * fact, KStorage * storage = 0);
};
class FreshwaterDoseCommitment : public DefaultValueTable<DoseCommValue>
{
public:
    explicit FreshwaterDoseCommitment(IModelFactory * fact, KStorage * storage = 0);
};


#endif // DEFAULTVALUES_H
