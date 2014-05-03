#ifndef KQUANTITYCONTROL_H
#define KQUANTITYCONTROL_H

#include "iserializable.h"

class KQuantityControlPrivate;
class K_CORE_EXPORT KQuantityControl : public ISerializable
{
public:
    KQuantityControl();
    explicit KQuantityControl(const Quantity * cntrl, bool enableOnSet);
    KQuantityControl(const KQuantityControl &);
    KQuantityControl &operator=(const KQuantityControl &);
    virtual ~KQuantityControl();
    
    bool isValid() const;
    bool isEnabledOnSet() const;
    const Quantity * controller() const;
    const ConstQuantityList & controlledQuantities() const;
    void append(const Quantity * qty);
    void append(const DataGroup & group);
    void remove(const Quantity * qty);
    KQuantityControl & operator<<(const Quantity * qty);
    KQuantityControl & operator<<(const DataGroup& group);

    virtual QDataStream & serialize(QDataStream &stream) const;
    virtual QDataStream & deserialize(QDataStream &stream);

private:
    QSharedDataPointer<KQuantityControlPrivate> data;
};

K_DECLARE_SERIALIZABLE(KQuantityControl)

#endif // KQUANTITYCONTROL_H
