/******************************************************************************
 * Author   : I Putu Susila (putu [at] batan.go.id)
 * Created  : 19/02/2014
 *
 * History  :
 ******************************************************************************/

/*!
   \class KHalfLife

    \brief The class for representing half-life of a radionuclide
    \since 1.0
    \inmodule radcore

    Half life, decay constant and mean time of a radionuclide.
    The class manage half-life value which can be converted to/from decay constant,
    and mean life time (1/decay constant).
 */

#include<QtCore/qmath.h>
#include "khalflife.h"

#ifndef M_LN2
#define M_LN2		0.69314718055994530942	/* log_e 2 */
#endif

KHalfLife::KHalfLife(qreal t12, bool isDecay, UnitType t)
{
    int nt = (int)t;
    if (isDecay)
        T1_2 = M_LN2 / (t12 * nt);
    else
        T1_2 = t12 * nt;
}
qreal KHalfLife::decayConstant(UnitType t /*= Second*/) const
{
    int nt = (int)t;
    return (nt*M_LN2) / T1_2;
}
qreal KHalfLife::meanLifeTime(UnitType t /*= Second*/) const
{
    int nt = (int)t;
    return T1_2 / (nt*M_LN2);
}
qreal KHalfLife::value(UnitType t /*= Second*/) const
{
    int nt = (int)t;
    return (T1_2/nt);
}

QString KHalfLife::displayText() const
{
    QString txt;
    double v;

    const int div[] = {(int)Year, (int)Month, (int)Day, (int)Hour, (int)Minute, (int)Second};
    const QString units[] = {
        QObject::tr("year(s)"), QObject::tr("month(s)"), QObject::tr("day(s)"),
        QObject::tr("hour(s)"), QObject::tr("minute(s)"), QObject::tr("second(s)")
    };

    int n = sizeof(div)/sizeof(int);
    for (int k = 0; k < n; k++) {
        v = T1_2 / div[k];
        if (v > 1) {
            txt = QString("%1 %2").arg(v).arg(units[k]);
            break;
        }
    }

    if (txt.isNull()) {
        txt = QString("%1 %2").arg(T1_2).arg(units[n-1]);
    }

    return txt;
}
