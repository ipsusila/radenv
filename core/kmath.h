#ifndef KMATH_H
#define KMATH_H

#include "radglobal.h"

class RADENV_API KMath
{
public:

    static qreal i0(qreal x);
    static qreal k0(qreal x);
    static qreal pr(qreal A);
    static qreal ratioN(qreal M);
    static qreal pe(qreal A, qreal N);
};

#endif // KMATH_H
