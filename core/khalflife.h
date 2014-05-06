#ifndef KHALFLIFE_H
#define KHALFLIFE_H

#include "radglobal.h"

class RADENV_API KHalfLife
{
public:
    /// Type of unit for the half-life value.
    enum UnitType {
        Second = 1,         ///< Second or 1/second for decay constant
        Minute = 60,        ///< Minute
        Hour = 3600,        ///< Hour
        Day = 84600,        ///< Days
        Month = 2592000 ,   ///< Month (approximately), which is equal to 30 days
        Year = 31536000     ///< Year (approximately), which is equal to 365 days
    };

    explicit KHalfLife(qreal t12, bool isDecay = false, UnitType t = Second);

    qreal decayConstant(UnitType t = Second) const;
    qreal meanLifeTime(UnitType t = Second) const;
    qreal value(UnitType t = Second) const;
    QString displayText() const;
private:
    qreal T1_2;
};

#endif // KHALFLIFE_H
