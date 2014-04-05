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

KRadionuclide::KRadionuclide() : data(new KRadionuclidePrivate)
{
}
KRadionuclide::KRadionuclide(const QString& n, const KHalfLife& hl, int usg)
    : data(new KRadionuclidePrivate(n, hl, usg))
{
}

KRadionuclide::KRadionuclide(const KRadionuclide &rhs) : data(rhs.data)
{
}

KRadionuclide &KRadionuclide::operator=(const KRadionuclide &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KRadionuclide::~KRadionuclide()
{
}

bool KRadionuclide::operator !=(const KRadionuclide& other) const
{
    return data->nuclide != other.data->nuclide;
}
bool KRadionuclide::operator ==(const KRadionuclide& other) const
{
    return data->nuclide == other.data->nuclide;
}

bool KRadionuclide::isValid() const
{
    return data->element.isValid();
}
bool KRadionuclide::isVeryShortLived() const
{
    //TODO
    return false;
}
bool KRadionuclide::isNobleGas() const
{
    //TODO
    return false;
}
bool KRadionuclide::isTritium() const
{
    return data->nuclide == "H-3";
}
bool KRadionuclide::isCarbon14() const
{
    return data->nuclide == "C-14";
}
bool KRadionuclide::isDepositedInGround() const
{
    return !(isNobleGas() && isTritium() && isCarbon14());
}

const KElement& KRadionuclide::element() const
{
    return data->element;
}
QString KRadionuclide::nuclide() const
{
    return data->nuclide;
}
KHalfLife KRadionuclide::halfLife() const
{
    return data->halfLife;
}
int KRadionuclide::usage() const
{
    return data->usage;
}
void KRadionuclide::use()
{
    data->usage += 1;
}
int KRadionuclide::attributes() const
{
    return data->attribute;
}
void KRadionuclide::addAttribute(int attr)
{
    data->attribute |= attr;
}

QString KRadionuclide::elementName(const QString &nuc)
{
    int pos = nuc.indexOf('-');
    if (pos > 0)
        return nuc.left(pos);
    return QString();
}
