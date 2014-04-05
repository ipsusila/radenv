#include <QSharedData>
#include "ksymbolcontrol.h"
#include "radcore.h"

class KSymbolControlPrivate : public QSharedData {
public:
    const Quantity * _controller;
    bool _enableOnSet;
    ConstSymbolList _controlledSymbols;

    KSymbolControlPrivate(const Quantity * ctrl, bool en)
        : _controller(ctrl), _enableOnSet(en)
    {

    }

    inline void remove(const Quantity * sym)
    {
        _controlledSymbols.removeOne(sym);
    }

    inline void append(const Quantity * sym)
    {
        if (sym != _controller && !_controlledSymbols.contains(sym))
            _controlledSymbols.append(sym);
    }
    void append(const DataGroup & group)
    {
        for(int k = 0; k < group.count(); k++) {
            const KData * d = group.pointerAt(k);
            this->append(d->symbolPtr());
        }
    }
};

KSymbolControl::KSymbolControl(const Quantity * ctrl, bool enableOnSet)
    : data(new KSymbolControlPrivate(ctrl, enableOnSet))
{
}

KSymbolControl::KSymbolControl(const KSymbolControl &rhs) : data(rhs.data)
{
}

KSymbolControl &KSymbolControl::operator=(const KSymbolControl &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KSymbolControl::~KSymbolControl()
{
}

bool KSymbolControl::isEnabledOnSet() const
{
    return data->_enableOnSet;
}
const Quantity * KSymbolControl::controller() const
{
    return data->_controller;
}
ConstSymbolList KSymbolControl::controlledSymbols() const
{
    return data->_controlledSymbols;
}

void KSymbolControl::append(const Quantity * sym)
{
    data->append(sym);
}
void KSymbolControl::append(const DataGroup & group)
{
    data->append(group);
}

void KSymbolControl::remove(const Quantity * sym)
{
    data->remove(sym);
}

KSymbolControl & KSymbolControl::operator<<(const Quantity * sym)
{
    data->append(sym);
    return *this;
}
KSymbolControl & KSymbolControl::operator<<(const DataGroup& group)
{
    data->append(group);
    return *this;
}
