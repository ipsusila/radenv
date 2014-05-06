#include <QSharedData>
#include "kquantitycontrol.h"
#include "radcore.h"
#include "kpluginmanager.h"
#include "imodelfactory.h"

class KQuantityControlPrivate : public QSharedData {
public:
    const Quantity * _controller;
    bool _enableOnSet;
    ConstQuantityList _controlledQuantities;

    KQuantityControlPrivate(const Quantity * ctrl, bool en)
        : _controller(ctrl), _enableOnSet(en)
    {

    }

    inline QDataStream & serialize(QDataStream &s) const
    {
        s << _enableOnSet;
        KPluginManager::instance()->serialize(s, _controller);
        //Rad::serialize(s, _controller);

        qint32 nz = _controlledQuantities.size();
        s << nz;
        for(int k = 0; k < nz; k++)
            KPluginManager::instance()->serialize(s, _controlledQuantities.at(k));
            //Rad::serialize(s, _controlledQuantities.at(k));

        return s;
    }
    inline QDataStream & deserialize(QDataStream &s)
    {
        s >> _enableOnSet;
        _controller = KPluginManager::instance()->deserialize(s); //Rad::deserialize(s);
        qint32 nz;
        s >> nz;
        _controlledQuantities.clear();
        for(int k = 0; k < nz; k++) {
            const Quantity * qty = KPluginManager::instance()->deserialize(s); //Rad::deserialize(s);
            if (qty != 0)
                _controlledQuantities.append(qty);
        }

        return s;
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

KQuantityControl::KQuantityControl()
    : dptr(new KQuantityControlPrivate(&Rad::EmptyQuantity, false))
{
}

KQuantityControl::KQuantityControl(const Quantity * ctrl, bool enableOnSet)
    : dptr(new KQuantityControlPrivate(ctrl, enableOnSet))
{
}

KQuantityControl::KQuantityControl(const KQuantityControl &rhs) : dptr(rhs.dptr)
{
}

KQuantityControl &KQuantityControl::operator=(const KQuantityControl &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KQuantityControl::~KQuantityControl()
{
}

bool KQuantityControl::isEnabledOnSet() const
{
    return dptr->_enableOnSet;
}
const Quantity * KQuantityControl::controller() const
{
    return dptr->_controller;
}
const ConstQuantityList & KQuantityControl::controlledQuantities() const
{
    return dptr->_controlledQuantities;
}

void KQuantityControl::append(const Quantity * qty)
{
    dptr->append(qty);
}
void KQuantityControl::append(const DataGroup & group)
{
    dptr->append(group);
}

void KQuantityControl::remove(const Quantity * qty)
{
    dptr->remove(qty);
}

KQuantityControl & KQuantityControl::operator<<(const Quantity * qty)
{
    dptr->append(qty);
    return *this;
}
KQuantityControl & KQuantityControl::operator<<(const DataGroup& group)
{
    dptr->append(group);
    return *this;
}

QDataStream & KQuantityControl::serialize(QDataStream &s) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << s.device()->pos();
    return dptr->serialize(s);
}

QDataStream & KQuantityControl::deserialize(QDataStream &s)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << s.device()->pos();
    return dptr->deserialize(s);
}
