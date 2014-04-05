#include <QSharedData>
#include <QVector>
#include "kdata.h"
#include "kgroup.h"
#include "koutput.h"
#include "kradionuclide.h"
#include "kstorage.h"

/** null parameter */
static const KData __nullData;
static const QVariant __nullValue;
static const KDataItem __nullItem;

class KDataPrivate : public QSharedData
{
public:
    const Quantity * _symbol;
    DataItemArray _items;
    KData::ContentTypes _types;

    KDataPrivate() : _symbol(&Rad::EmptySymbol), _types(KData::Undefined)
    {
    }

    KDataPrivate(const Quantity * var, const QVariant& v)
        : _symbol(var), _types(KData::Undefined)
    {
        Q_ASSERT(_symbol != 0);
        if (v.isValid()) {
            KData::ContentType t = guessType(v);
            _types |= t;
            _items.append(KDataItem(var->symbol, v, t));
        }
    }
    KDataPrivate(const Quantity *sym, KData::ContentTypes types, const QVariant& v)
        : _symbol(sym), _types(types)
    {
        if (v.isValid())
            _items.append(KDataItem(sym->symbol, v, types));
    }

    KDataPrivate(const Quantity * var, const QString& nm, const QVariant& v, KData::ContentTypes t)
    {
        _symbol = var;
        _items.append(KDataItem(nm, v, t));
        _types |= t;
    }

    KDataPrivate(const Quantity * var, const DataItemArray & it)
        : _symbol(var), _items(it)
    {
        Q_ASSERT(_symbol != 0);
        _types = KData::Undefined;
        for(int k = 0; k < it.size(); k++)
            _types |= it.at(k).contentTypes();
        _types |= KData::Array;
    }

    inline KData::ContentType guessType(const QVariant & v) {
        KData::ContentType t;
        if (!v.isValid())
            t = KData::Undefined;
        else if (v.type() == QVariant::Bool)
            t = KData::Boolean;
        else if (v.type() == QVariant::String)
            t = KData::Text;
        else if (v.canConvert(QVariant::Double))
            t = KData::Numeric;
        else
            t = KData::Object;
        return t;
    }

    inline void append(const QString& nm, const QVariant& v, KData::ContentTypes t)
    {
        _items.append(KDataItem(nm, v, t));
        _types |= (t | KData::Array) ;
    }

    inline void setValue(int idx, const QVariant& v)
    {
        Q_ASSERT(idx < _items.size());
        _items[idx].setValue(v);
        KData::ContentType t = guessType(v);
        _types |= t;
    }
    inline QVariant value(int idx) const
    {
        if (idx < 0 || idx >= _items.size()) {
            return __nullValue;
        }
        else {
            return _items[idx].value();
        }
    }
    qreal numericValue(const QString& name, qreal defValue) const
    {
        for(int k = _items.size() - 1; k >= 0; k--) {
            if (_items[k].name() == name)
                return _items[k].numericValue();
        }
        return defValue;
    }
    const KDataItem & itemAt(const QString& name) const
    {
        for(int k = _items.size() - 1; k >= 0; k--) {
            if (_items.at(k).name() == name)
                return _items.at(k);
        }
        return __nullItem;
    }
    inline QString toString(const KDataItem &di) const
    {
        bool flag;
        switch(_symbol->type) {
        case Rad::Boolean:
            flag = di.value().toBool();
            return (flag ? "True" : "False");
        case Rad::Text:
            return di.value().toString();
        case Rad::Comment:
            return "//" + di.value().toString();
        default:
            return QString::number(di.numericValue());
        }
    }

    QString displayText() const
    {
        if (_items.isEmpty()) {
            return QString();
        }
        else if (_items.count() == 1 && !_types.testFlag(KData::Array)) {
            const KDataItem & di = _items.first();
            QString txt = QString("%1: %2 %3").arg(_symbol->displayText())
                    .arg(toString(di)).arg(_symbol->unit);
            if (di.contentTypes().testFlag(KData::RadionuclideSource)) {
                const KRadionuclide & nuc = KStorage::storage()->radionuclide(di.name());
                txt += QString(" (%1, lambda = %2 s-1)").arg(nuc.nuclide()).arg(nuc.halfLife().decayConstant());
            }

            return txt;
        }

        DataItemArray::const_iterator beg = _items.begin();
        DataItemArray::const_iterator end = _items.end();
        QString txt = _symbol->displayText();

        while (beg != end) {
            txt += QString("\n  %1: %2 %3").arg(beg->name())
                    .arg(toString(*beg)).arg(_symbol->unit);
            if (beg->contentTypes().testFlag(KData::RadionuclideSource)) {
                const KRadionuclide & nuc = KStorage::storage()->radionuclide(beg->name());
                txt += QString(" (lambda = %1 s-1)").arg(nuc.halfLife().decayConstant());
            }
            beg++;
        }
        return txt;
    }
};

/***********************************************************************************/
KDataItem::KDataItem()
    : _name(QString()), _value(QVariant()), _types(KData::Undefined)
{
}
KDataItem::KDataItem(const QString& nm, const QVariant& v, KData::ContentTypes t)
    : _name(nm), _value(v)
{
    //make sure no array
    _types = t & (~KData::Array);
}
KDataItem::KDataItem(const KDataItem& other)
    : _name(other._name), _value(other._value), _types(other._types)
{
}
KDataItem &KDataItem::operator=(const KDataItem& rhs)
{
    if (this != &rhs) {
        _name = rhs._name;
        _value = rhs._value;
        _types = rhs._types;
    }
    return *this;
}
QString KDataItem::name() const
{
    return _name;
}
QVariant KDataItem::value() const
{
    return _value;
}
qreal KDataItem::numericValue() const
{
    return _value.toDouble();
}
KData::ContentTypes KDataItem::contentTypes() const
{
    return _types;
}
void KDataItem::setValue(const QVariant &v)
{
    _value = v;
}

KDataItem KDataItem::operator*(qreal c) const
{
    return KDataItem(_name, _value.toDouble()*c, _types);
}

KDataItem KDataItem::operator/(qreal c) const
{
    return KDataItem(_name, _value.toDouble()/c, _types);
}
KDataItem & KDataItem::operator*=(qreal c)
{
    _value = _value.toDouble() * c;
    return *this;
}

KDataItem & KDataItem::operator/=(qreal c)
{
    _value = _value.toDouble() / c;
    return *this;
}

/***********************************************************************************/
KData::KData()
    : data(new KDataPrivate())
{
}

KData::KData(const Quantity *sym)
    : data(new KDataPrivate(sym, sym->defaultValue))
{
}

KData::KData(const Quantity * var, const QVariant& v)
    : data(new KDataPrivate(var, v))
{
}
KData::KData(const Quantity * var, ContentTypes  types, const QVariant & v)
    : data(new KDataPrivate(var, types, v))
{

}

KData::KData(const Quantity * var, const QString& nm, const QVariant& v, ContentTypes t)
    : data(new KDataPrivate(var, nm, v, t))
{
}

KData::KData(const Quantity * var, const DataItemArray & items)
    : data(new KDataPrivate(var, items))
{
}

KData::KData(const KData& rhs) : data(rhs.data)
{
}
KData::KData(const Quantity * sym, const KData& other) : data(other.data)
{
    if (this != &other) {
        data.operator=(other.data);
        data->_symbol = sym;
    }
}

KData & KData::operator=(const KData& rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}
KData::~KData()
{
}

const Quantity & KData::symbol() const
{
    return *(data->_symbol);
}
const Quantity * KData::symbolPtr() const
{
    return data->_symbol;
}
KData::ContentTypes KData::contentTypes() const
{
    return data->_types;
}
bool KData::contains(ContentType type) const
{
    return data->_types.testFlag(type);
}

int KData::count() const
{
    return data->_items.size();
}
void KData::resize(int cnt)
{
    data->_items.resize(cnt);
}

bool KData::isValid() const
{
    //return !data->_items.isEmpty();
    return data->_types != KData::Undefined;
}
bool KData::isEmpty() const
{
    return data->_items.isEmpty();
}
void KData::append(const QString& nm, const QVariant& v, ContentTypes t)
{
    data->append(nm, v, t);
}

QVariant KData::value(int idx) const
{
    return data->value(idx);
}
void KData::setValue(const QVariant &v)
{
    data->setValue(0, v);
}
void KData::setValue(int idx, const QVariant& v)
{
    data->setValue(idx, v);
}
qreal KData::numericValue(int idx) const
{
    return data->value(idx).toDouble();
}
qreal KData::numericValue(const QString& name, qreal defValue) const
{
    return data->numericValue(name, defValue);
}

const KDataItem & KData::at(const QString& name) const
{
    return data->itemAt(name);
}

const KDataItem & KData::at(int idx) const
{
    Q_ASSERT(!isEmpty());
    return data->_items.at(idx);
}
KData KData::operator*(qreal c) const
{
    KData p(*this);
    p *= c;

    return p;
}

KData KData::operator/(qreal c) const
{
    KData p(*this);
    p /= c;

    return p;
}

KData & KData::operator*=(qreal c)
{
    int sz = data->_items.size();
    for (int k = 0; k < sz; k++) {
        data->_items[k] *= c;
    }
    return *this;
}

KData & KData::operator/=(qreal c)
{
    int sz = data->_items.size();
    for (int k = 0; k < sz; k++) {
        data->_items[k] /= c;
    }
    return *this;
}
bool KData::operator==(const KData& o) const
{
    return &this->symbol() == &o.symbol();
}
bool KData::operator!=(const KData& o) const
{
    return &this->symbol() != &o.symbol();
}

QString KData::displayText() const
{
    return data->displayText();
}

/***********************************************************************************/
KDataArray::KDataArray()
{
    //xInfo() << "#Null value: " << __nullData.numericValue();
}

KDataArray::KDataArray(const KDataArray& o)
    : DataList(o)
{
    if (this != &o)
        this->_location = o.location();
}
KDataArray::KDataArray(const DataList &o)
    : DataList(o)
{
}

void KDataArray::setLocation(const KLocation &loc)
{
    this->_location = loc;
}
KLocation KDataArray::location() const
{
    return this->_location;
}
ConstSymbolList KDataArray::symbols() const
{
    ConstSymbolList list;
    for(int k = 0; k < size(); k++) {
        const Quantity * sym = at(k).symbolPtr();
        if (!list.contains(sym))
            list.append(sym);
    }
    return list;
}

const KData & KDataArray::find(const QString& sym) const
{
    DataList::const_iterator beg = this->begin();
    DataList::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol().symbol == sym)
            return *beg;
        beg++;
    }

    return __nullData;
}

const KData & KDataArray::find(const Quantity& v) const
{
    DataList::const_iterator beg = this->begin();
    DataList::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol() == v)
            return *beg;
        beg++;
    }

    return __nullData;
}

qreal KDataArray::numericValueOf(const Quantity& v) const
{
    DataList::const_iterator beg = this->begin();
    DataList::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol() == v)
            return beg->numericValue();
        beg++;
    }
    return __nullValue.toDouble();
}

QVariant KDataArray::valueOf(const Quantity& v) const
{
    DataList::const_iterator beg = this->begin();
    DataList::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol() == v)
            return beg->value();
        beg++;
    }
    return __nullValue;
}

QString KDataArray::displayText() const
{
    if (isEmpty())
        return QString();

    DataList::const_iterator beg = this->begin();
    DataList::const_iterator end = this->end();
    QString txt = beg->displayText();

    while ((++beg) != end) {
        txt += "\n" + beg->displayText();
    }

    return txt;
}

void KDataArray::appendOrMerge(const Quantity * sym, const QString& nm, const QVariant& v, KData::ContentTypes t)
{
    for(int k = 0; k < size(); k++) {
        KData & d = (*this)[k];
        if (d.symbol() == *sym) {
            d.append(nm, v, t);
            return;
        }
    }
    append(KData(sym, nm, v, t | KData::Array));
}

void KDataArray::appendOrReplace(const KData &di)
{
    for(int k = 0; k < size(); k++) {
        if (this->at(k).symbol() == di.symbol()) {
            (*this)[k] = di;
            return;
        }
    }
    append(di);
}

const KData & KDataGroupArray::find(const Quantity& v) const
{
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            const KData & d = it->itemAt(k);
            if (d.symbol() == v)
                return d;
        }

        it++;
    }

    return __nullData;
}

KDataGroupArray::KDataGroupArray() : QVector<DataGroup >()
{
}

KDataGroupArray::KDataGroupArray(const QString& name, const KDataArray & da)
    : QVector<DataGroup >()
{
    DataGroup g(name);
    foreach(const KData& d, da) {
        g.add(d);
    }
    this->append(g);
}

qreal KDataGroupArray::numericValueOf(const Quantity& v) const
{
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            const KData & d = it->itemAt(k);
            if (d.symbol() == v)
                return d.numericValue();
        }

        it++;
    }
    return __nullValue.toDouble();
}
QVariant KDataGroupArray::valueOf(const Quantity& v) const
{
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            const KData & d = it->itemAt(k);
            if (d.symbol() == v)
                return d.value();
        }

        it++;
    }
    return __nullValue;
}
ConstSymbolList KDataGroupArray::symbols() const
{
    ConstSymbolList list;
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            const Quantity * sym = it->itemAt(k).symbolPtr();
            if (!list.contains(sym))
                list.append(sym);
        }

        it++;
    }
    return list;
}
void KDataGroupArray::replace(const KData &di)
{
    KDataGroupArray::iterator it = this->begin();
    KDataGroupArray::iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            const KData & d = it->itemAt(k);
            if (d.symbol() == di.symbol()) {
                it->replaceAt(k, di);
                return;
            }
        }

        it++;
    }
}
KDataArray KDataGroupArray::toDataArray() const
{
    KDataArray list;
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            list << it->itemAt(k);
        }

        it++;
    }
    return list;
}
void KDataGroupArray::addQuantityControl(const KSymbolControl & qc)
{
    _controlList.append(qc);
}

const QuantityControlList & KDataGroupArray::quantityControls() const
{
    return _controlList;
}



