#ifndef KELEMENT_H
#define KELEMENT_H

#include "radglobal.h"

class KElementPrivate;

/**
 * @brief Class for representing chemical element and its name.
 */
class K_CORE_EXPORT KElement
{
private:
    KElement(const QString& e, const QString& n);
public:
    KElement(const KElement &);
    KElement &operator=(const KElement &);
    ~KElement();

    QString symbol() const;
    QString name() const;
    bool isValid() const;

    bool operator==(const KElement &other) const;
    bool operator!=(const KElement &other) const;

    /**
     * @brief Create new element object from given symbol
     * @param symbol of the element, e.g. C, Ag, O
     * @return Element for given symbol
     */
    static KElement fromSymbol(const QString& symbol);
    static KElement fromNuclide(const QString& nuclide);
    static KElement ** elements();
    static int elementCount();

    /**
     *static element value
     */
    static KElement Empty;
    static KElement Ac;
    static KElement Ag;
    static KElement Al;
    static KElement Am;
    static KElement Ar;
    static KElement As;
    static KElement At;
    static KElement Au;
    static KElement B;
    static KElement Ba;
    static KElement Be;
    static KElement Bh;
    static KElement Bi;
    static KElement Bk;
    static KElement Br;
    static KElement C;
    static KElement Ca;
    static KElement Cd;
    static KElement Ce;
    static KElement Cf;
    static KElement Cl;
    static KElement Cm;
    static KElement Cn;
    static KElement Co;
    static KElement Cr;
    static KElement Cs;
    static KElement Cu;
    static KElement Db;
    static KElement Ds;
    static KElement Dy;
    static KElement Er;
    static KElement Es;
    static KElement Eu;
    static KElement F;
    static KElement Fe;
    static KElement Fl;
    static KElement Fm;
    static KElement Fr;
    static KElement Ga;
    static KElement Gd;
    static KElement Ge;
    static KElement H;
    static KElement He;
    static KElement Hf;
    static KElement Hg;
    static KElement Ho;
    static KElement Hs;
    static KElement I;
    static KElement In;
    static KElement Ir;
    static KElement K;
    static KElement Kr;
    static KElement La;
    static KElement Li;
    static KElement Lr;
    static KElement Lu;
    static KElement Lv;
    static KElement Md;
    static KElement Mg;
    static KElement Mn;
    static KElement Mo;
    static KElement Mt;
    static KElement N;
    static KElement Na;
    static KElement Nb;
    static KElement Nd;
    static KElement Ne;
    static KElement Ni;
    static KElement No;
    static KElement Np;
    static KElement O;
    static KElement Os;
    static KElement P;
    static KElement Pa;
    static KElement Pb;
    static KElement Pd;
    static KElement Pm;
    static KElement Po;
    static KElement Pr;
    static KElement Pt;
    static KElement Pu;
    static KElement Ra;
    static KElement Rb;
    static KElement Re;
    static KElement Rf;
    static KElement Rg;
    static KElement Rh;
    static KElement Rn;
    static KElement Ru;
    static KElement S;
    static KElement Sb;
    static KElement Sc;
    static KElement Se;
    static KElement Sg;
    static KElement Si;
    static KElement Sm;
    static KElement Sn;
    static KElement Sr;
    static KElement Ta;
    static KElement Tb;
    static KElement Tc;
    static KElement Te;
    static KElement Th;
    static KElement Ti;
    static KElement Tl;
    static KElement Tm;
    static KElement U;
    static KElement Uuo;
    static KElement Uup;
    static KElement Uus;
    static KElement Uut;
    static KElement V;
    static KElement W;
    static KElement Xe;
    static KElement Y;
    static KElement Yb;
    static KElement Zn;
    static KElement Zr;

private:
    QSharedDataPointer<KElementPrivate> data;
};

#endif // KELEMENT_H
