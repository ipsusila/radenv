#include <QtCore/qmath.h>
#include "kmath.h"

//table V of SRS-19
//page 43
typedef struct _tagMN
{
    qreal M;
    qreal N;
} LDCoeff;

static const LDCoeff _ldCoeff[] =
{
    {0, 0}, {0.01, 0.00028}, {0.02, 0.00115}, {0.03, 0.00237}, {0.04, 0.00427},
    {0.05, 0.00640}, {0.06, 0.00930}, {0.07, 0.0122}, {0.08, 0.0152},
    {0.09, 0.0205}, {0.1, 0.0240}, {0.2, 0.0900}, {0.3, 0.167}, {0.4, 0.267},
    {0.5, 0.350}, {0.6, 0.430}, {0.7, 0.544}, {0.8, 0.610}, {0.9, 0.640},
    {1.0, 0.705}, {2.0, 0.910}, {3.0, 0.940}, {4.0, 0.950}, {5.0, 0.970},
    {6.0, 0.980}, {7.0, 0.990}, {8.0, 0.995}, {9.0, 1.0}
};

/****************************************************************************************/
/* Taken from c numerical recipies */
/****************************************************************************************/
qreal KMath::i0(qreal x)
{
    qreal ax,ans;
    qreal y;

    if ((ax=qAbs(x)) < 3.75) {
        y=x/3.75;
        y*=y;
        ans=1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
        +y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
    } else {
        y=3.75/ax;
        ans=(qExp(ax)/qSqrt(ax))*(0.39894228+y*(0.1328592e-1
        +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
        +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
        +y*0.392377e-2))))))));
    }
    return ans;
}

qreal KMath::k0(qreal x)
{
    qreal y,ans;

    if (x <= 2.0) {
        y=x*x/4.0;
        ans=(-qLn(x/2.0)*KMath::i0(x))+(-0.57721566+y*(0.42278420
        +y*(0.23069756+y*(0.3488590e-1+y*(0.262698e-2
        +y*(0.10750e-3+y*0.74e-5))))));
    } else {
        y=2.0/x;
        ans=(qExp(-x)/qSqrt(x))*(1.25331414+y*(-0.7832358e-1
        +y*(0.2189568e-1+y*(-0.1062446e-1+y*(0.587872e-2
        +y*(-0.251540e-2+y*0.53208e-3))))));
    }
    return ans;
}

qreal KMath::pr(qreal A)
{
    //srs-19 page 177
    qreal prx = ((qExp(A) * k0(A))/(0.142*M_PI));
    return qMax(prx, 1.0);
}
qreal KMath::ratioN(qreal M)
{
    //check M
    M = M < 0.0 ? 0.0 : M;

    //begin calculation
    //(y-y1)*(x2-x1) = (y2-y1)*(x-x1)
    //m = (y2-y1)/(x2-x1)
    //y = m * (x-x1) + y1
    qreal N = 0;
    int p1=0, p2;
    int Ni = sizeof(_ldCoeff) / sizeof(LDCoeff);
    while(p1 < Ni) {
        if (_ldCoeff[p1].M > M) {
            Q_ASSERT(p1 > 0);

            p2 = p1;
            p1 --;

            qreal M1 = _ldCoeff[p1].M;
            qreal N1 = _ldCoeff[p1].N;
            qreal m = (_ldCoeff[p2].N - _ldCoeff[p1].N) / (_ldCoeff[p2].M - _ldCoeff[p1].M);
            N = m * (M - M1) + N1;

            break;
        }
        p1++;
    }

    //not found
    if (p1 == Ni)
        N = 1.0;

    return N;
}
qreal KMath::pe(qreal A, qreal N)
{
    qreal pex = (qExp(A) * k0(A)) / (0.32 * M_PI * qSqrt(N));
    return qMax(pex, 1.0);
}
