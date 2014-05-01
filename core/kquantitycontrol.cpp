#include <QSharedData>
#include "kquantitycontrol.h"
#include "radcore.h"

class KQuantityControlPrivate : public QSharedData {
public:
    const Quantity * _controller;
    bool _enableOnSet;
    ConstQuantityList _controlledQuantities;

    KQuantityControlPrivate(const Quantity * ctrl, bool en)
        : _controller(ctrl), _enableOnSet(en)
    {

    }

    inline void remove(const Quantity * qty)
    {
        _controlledQuantities.removeOne(qty);
    }

    inline void append(const Quantity * qty)
    {
        if (qty != _controller && !_controlledQuantities.contains(qty))
            _controlledQuantities.append(qty);
    }
    void append(const DataGroup & group)
    {
        for(int k = 0; k < group.count(); k++) {
            const KData * d = group.pointerAt(k);
            this->append(d->quantityPtr());
        }
    }
};

//helper function
QDataStream & operator<<(QDataStream &s, const KQuantityControl &qc)
{
    return s;
}

QDataStream & operator>>(QDataStream &s, KQuantityControl &qc)
{
    return s;
}

KQuantityControl::KQuantityControl(const Quantity * ctrl, bool enableOnSet)
    : data(new KQuantityControlPrivate(ctrl, enableOnSet))
{
}

KQuantityControl::KQuantityControl(const KQuantityControl &rhs) : data(rhs.data)
{
}

KQuantityControl &KQuantityControl::operator=(const KQuantityControl &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KQuantityControl::~KQuantityControl()
{
}

bool KQuantityControl::isEnabledOnSet() const
{
    return data->_enableOnSet;
}
const Quantity * KQuantityControl::controller() const
{
    return data->_controller;
}
const ConstQuantityList & KQuantityControl::controlledQuantities() const
{
    return data->_controlledQuantities;
}

void KQuantityControl::append(const Quantity * qty)
{
    data->append(qty);
}
void KQuantityControl::append(const DataGroup & group)
{
    data->append(group);
}

void KQuantityControl::remove(const Quantity * qty)
{
    data->remove(qty);
}

KQuantityControl & KQuantityControl::operator<<(const Quantity * qty)
{
    data->append(qty);
    return *this;
}
KQuantityControl & KQuantityControl::operator<<(const DataGroup& group)
{
    data->append(group);
    return *this;
}
