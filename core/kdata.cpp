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
    const Quantity * _quantity;
    DataItemArray _items;
    KData::ContentTypes _types;

    KDataPrivate() : _quantity(&Rad::EmptyQuantity), _types(KData::Undefined)
    {
    }

    KDataPrivate(const Quantity * qty, const QVariant& v)
        : _quantity(qty), _types(KData::Undefined)
    {
        Q_ASSERT(_quantity != 0);
        if (v.isValid()) {
            KData::ContentType t = guessType(v);
            _types |= t;
            _items.append(KDataItem(qty->symbol, v, t));
        }
    }
    KDataPrivate(const Quantity *qty, KData::ContentTypes types, const QVariant& v)
        : _quantity(qty), _types(types)
    {
        if (v.isValid())
            _items.append(KDataItem(qty->symbol, v, types));
    }

    KDataPrivate(const Quantity * qty, const QString& nm, const QVariant& v, KData::ContentTypes t)
    {
        _quantity = qty;
        _items.append(KDataItem(nm, v, t));
        _types |= t;
    }

    KDataPrivate(const Quantity * qty, const DataItemArray & it)
        : _quantity(qty), _items(it)
    {
        Q_ASSERT(_quantity != 0);
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
        switch(_quantity->type) {
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
            QString txt = QString("%1: %2 %3").arg(_quantity->displayText())
                    .arg(toString(di)).arg(_quantity->unit);
            if (di.contentTypes().testFlag(KData::RadionuclideSource)) {
                const KRadionuclide & nuc = KStorage::storage()->radionuclide(di.name());
                txt += QString(" (%1, lambda = %2 s-1)").arg(nuc.nuclide()).arg(nuc.halfLife().decayConstant());
            }

            return txt;
        }

        DataItemArray::const_iterator beg = _items.begin();
        DataItemArray::const_iterator end = _items.end();
        QString txt = _quantity->displayText();

        while (beg != end) {
            txt += QString("\n  %1: %2 %3").arg(beg->name())
                    .arg(toString(*beg)).arg(_quantity->unit);
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

KData::KData(const Quantity *qty)
    : data(new KDataPrivate(qty, qty->defaultValue))
{
}

KData::KData(const Quantity * qty, const QVariant& v)
    : data(new KDataPrivate(qty, v))
{
}
KData::KData(const Quantity * qty, ContentTypes  types, const QVariant & v)
    : data(new KDataPrivate(qty, types, v))
{

}

KData::KData(const Quantity * qty, const QString& nm, const QVariant& v, ContentTypes t)
    : data(new KDataPrivate(qty, nm, v, t))
{
}

KData::KData(const Quantity * qty, const DataItemArray & items)
    : data(new KDataPrivate(qty, items))
{
}

KData::KData(const KData& rhs) : data(rhs.data)
{
}
KData::KData(const Quantity * qty, const KData& other) : data(other.data)
{
    if (this != &other) {
        data.operator=(other.data);
        data->_quantity = qty;
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

const Quantity & KData::quantity() const
{
    return *(data->_quantity);
}
const Quantity * KData::quantityPtr() const
{
    return data->_quantity;
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
    return &this->quantity() == &o.quantity();
}
bool KData::operator!=(const KData& o) const
{
    return &this->quantity() != &o.quantity();
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
ConstQuantityList KDataArray::quantities() const
{
    ConstQuantityList list;
    for(int k = 0; k < size(); k++) {
        const Quantity * qty = at(k).quantityPtr();
        if (!list.contains(qty))
            list.append(qty);
    }
    return list;
}

const KData & KDataArray::find(const QString& qty) const
{
    DataList::const_iterator beg = this->begin();
    DataList::const_iterator end = this->end();
    while (beg != end) {
        if (beg->quantity().symbol == qty)
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
        if (beg->quantity() == v)
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
        if (beg->quantity() == v)
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
        if (beg->quantity() == v)
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

void KDataArray::appendOrMerge(const Quantity * qty, const QString& nm, const QVariant& v, KData::ContentTypes t)
{
    for(int k = 0; k < size(); k++) {
        KData & d = (*this)[k];
        if (d.quantity() == *qty) {
            d.append(nm, v, t);
            return;
        }
    }
    append(KData(qty, nm, v, t | KData::Array));
}

void KDataArray::appendOrReplace(const KData &di)
{
    for(int k = 0; k < size(); k++) {
        if (this->at(k).quantity() == di.quantity()) {
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
            if (d.quantity() == v)
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
            if (d.quantity() == v)
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
            if (d.quantity() == v)
                return d.value();
        }

        it++;
    }
    return __nullValue;
}
ConstQuantityList KDataGroupArray::quantities() const
{
    ConstQuantityList list;
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        int nz = it->count();
        for(int k = 0; k < nz; k++) {
            const Quantity * qty = it->itemAt(k).quantityPtr();
            if (!list.contains(qty))
                list.append(qty);
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
            if (d.quantity() == di.quantity()) {
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
void KDataGroupArray::addQuantityControl(const KQuantityControl & qc)
{
    if (qc.controller() == 0)
        return;

    const ConstQuantityList & list = qc.controlledQuantities();
    for (int k = 0; k < list.size(); k++) {
        //if the quantity is not found in this data array
        //ignore the control
        const Quantity * qty = list.at(k);
        if (!this->find(*qty).isValid())
            return;
    }

    //add to control list
    _controlList.append(qc);
}

const QuantityControlList & KDataGroupArray::quantityControls() const
{
    return _controlList;
}



