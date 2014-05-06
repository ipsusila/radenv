#include "kradionuclide.h"
#include <QSharedData>

class KRadionuclidePrivate : public QSharedData {
public:
    QString nuclide;
    KHalfLife halfLife;
    KElement element;
    int usage;
    int attribute;

    KRadionuclidePrivate() : nuclide(QString()), halfLife(0),
        element(KElement::Empty),usage(0),attribute(0)
    {
    }
    KRadionuclidePrivate(const QString& n, const KHalfLife& hl, int usg)
        : nuclide(n), halfLife(hl), element(KElement::fromNuclide(n)), usage(usg),attribute(0)
    {
    }
};

KRadionuclide::KRadionuclide() : dptr(new KRadionuclidePrivate)
{
}
KRadionuclide::KRadionuclide(const QString& n, const KHalfLife& hl, int usg)
    : dptr(new KRadionuclidePrivate(n, hl, usg))
{
}

KRadionuclide::KRadionuclide(const KRadionuclide &rhs) : dptr(rhs.dptr)
{
}

KRadionuclide &KRadionuclide::operator=(const KRadionuclide &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KRadionuclide::~KRadionuclide()
{
}

bool KRadionuclide::operator !=(const KRadionuclide& other) const
{
    return dptr->nuclide != other.dptr->nuclide;
}
bool KRadionuclide::operator ==(const KRadionuclide& other) const
{
    return dptr->nuclide == other.dptr->nuclide;
}

bool KRadionuclide::isValid() const
{
    return dptr->element.isValid();
}
bool KRadionuclide::isVeryShortLived() const
{
    //TODO
    return false;
}
bool KRadionuclide::isNobleGas() const
{
    const KElement & e = this->element();
    return (e == KElement::He || e == KElement::Ne || e == KElement::Ar ||
            e == KElement::Kr || e == KElement::Xe || e == KElement::Rn);
}
bool KRadionuclide::isTritium() const
{
    return dptr->nuclide == "H-3";
}
bool KRadionuclide::isCarbon14() const
{
    return dptr->nuclide == "C-14";
}
bool KRadionuclide::isDepositedInGround() const
{
    return !(isNobleGas() && isTritium() && isCarbon14());
}

const KElement& KRadionuclide::element() const
{
    return dptr->element;
}
QString KRadionuclide::nuclide() const
{
    return dptr->nuclide;
}
KHalfLife KRadionuclide::halfLife() const
{
    return dptr->halfLife;
}
int KRadionuclide::usage() const
{
    return dptr->usage;
}
void KRadionuclide::use()
{
    dptr->usage += 1;
}
int KRadionuclide::attributes() const
{
    return dptr->attribute;
}
void KRadionuclide::addAttribute(int attr)
{
    dptr->attribute |= attr;
}

QString KRadionuclide::elementName(const QString &nuc)
{
    int pos = nuc.indexOf('-');
    if (pos > 0)
        return nuc.left(pos);
    return QString();
}
