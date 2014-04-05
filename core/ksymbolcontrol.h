#ifndef KSYMBOLCONTROL_H
#define KSYMBOLCONTROL_H

#include <QSharedDataPointer>
#include "radglobal.h"

class KSymbolControlPrivate;
class K_CORE_EXPORT KSymbolControl
{
public:
    explicit KSymbolControl(const Quantity * cntrl, bool enableOnSet);
    KSymbolControl(const KSymbolControl &);
    KSymbolControl &operator=(const KSymbolControl &);
    ~KSymbolControl();
    
    bool isValid() const;
    bool isEnabledOnSet() const;
    const Quantity * controller() const;
    ConstSymbolList controlledSymbols() const;
    void append(const Quantity * sym);
    void append(const DataGroup & group);
    void remove(const Quantity * sym);
    KSymbolControl & operator<<(const Quantity * sym);
    KSymbolControl & operator<<(const DataGroup& group);

private:
    QSharedDataPointer<KSymbolControlPrivate> data;
};

#endif // KSYMBOLCONTROL_H
