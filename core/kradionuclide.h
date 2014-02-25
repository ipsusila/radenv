#ifndef KRADIONUCLIDE_H
#define KRADIONUCLIDE_H

#include "kelement.h"
#include "khalflife.h"

class KRadionuclidePrivate;
class K_CORE_EXPORT KRadionuclide
{
public:
    KRadionuclide();
    KRadionuclide(const QString& n, const KHalfLife& hl, int usg = 0);
    KRadionuclide(const KRadionuclide &);
    KRadionuclide &operator=(const KRadionuclide &);
    virtual ~KRadionuclide();

    bool operator==(const KRadionuclide &other) const;
    bool operator!=(const KRadionuclide &other) const;

    KElement element() const;
    QString nuclide() const;
    KHalfLife halfLife() const;
    int usage() const;
    void use();
    void addAttribute(int attr);
    int attributes() const;
    bool isValid() const;
    bool isVeryShortLived() const;
    bool isNobleGas() const;

private:
    QSharedDataPointer<KRadionuclidePrivate> data;
};

#endif // KRADIONUCLIDE_H
