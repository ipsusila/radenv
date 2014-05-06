#ifndef KRADIONUCLIDE_H
#define KRADIONUCLIDE_H

#include "kelement.h"
#include "khalflife.h"

class KRadionuclidePrivate;
class RADENV_API KRadionuclide
{
public:
    KRadionuclide();
    KRadionuclide(const QString& n, const KHalfLife& hl, int usg = 0);
    KRadionuclide(const KRadionuclide &);
    KRadionuclide &operator=(const KRadionuclide &);
    virtual ~KRadionuclide();

    bool operator==(const KRadionuclide &other) const;
    bool operator!=(const KRadionuclide &other) const;

    const KElement& element() const;
    QString nuclide() const;
    KHalfLife halfLife() const;
    int usage() const;
    void use();
    void addAttribute(int attr);
    int attributes() const;
    bool isValid() const;
    bool isVeryShortLived() const;
    bool isNobleGas() const;
    bool isTritium() const;
    bool isCarbon14() const;
    bool isDepositedInGround() const;

    static QString elementName(const QString &nuc);

private:
    QSharedDataPointer<KRadionuclidePrivate> dptr;
};

#endif // KRADIONUCLIDE_H
