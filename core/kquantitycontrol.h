#ifndef KQUANTITYCONTROL_H
#define KQUANTITYCONTROL_H

#include <QSharedDataPointer>
#include "radglobal.h"

class KQuantityControlPrivate;
class K_CORE_EXPORT KQuantityControl
{
public:
    explicit KQuantityControl(const Quantity * cntrl, bool enableOnSet);
    KQuantityControl(const KQuantityControl &);
    KQuantityControl &operator=(const KQuantityControl &);
    ~KQuantityControl();
    
    bool isValid() const;
    bool isEnabledOnSet() const;
    const Quantity * controller() const;
    const ConstQuantityList & controlledQuantities() const;
    void append(const Quantity * qty);
    void append(const DataGroup & group);
    void remove(const Quantity * qty);
    KQuantityControl & operator<<(const Quantity * qty);
    KQuantityControl & operator<<(const DataGroup& group);

private:
    QSharedDataPointer<KQuantityControlPrivate> data;
};

#endif // KQUANTITYCONTROL_H
