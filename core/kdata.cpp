#include <QSharedData>
#include <QVector>
#include <QStringList>
#include <QtDebug>
#include "kdata.h"
#include "kgroup.h"
#include "koutput.h"
#include "kradionuclide.h"
#include "kstorage.h"
#include "kpluginmanager.h"

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
    KDataPrivate(const Quantity *qty, const QMap<QString, qreal> & items, KData::ContentTypes t)
        : _quantity(qty)
    {
        Q_ASSERT(_quantity != 0);
        _types |= (KData::Array | t);

        QMap<QString, qreal>::const_iterator it = items.constBegin();
        QMap<QString, qreal>::const_iterator end = items.constEnd();
        while (it != end) {
            _items.append(KDataItem(it.key(), it.value(), t));
            it++;
        }
    }

    QDataStream & serialize(QDataStream & s) const
    {
        s << (qint32)_types;
        //Rad::serialize(s, _quantity);
        KPluginManager::instance()->serialize(s, _quantity);
        s << _items;

        return s;
    }
    inline QDataStream & deserialize(QDataStream & s)
    {
        qint32 itypes;
        s >> itypes;
        _types = KData::ContentTypes(itypes);
        _quantity = KPluginManager::instance()->deserialize(s); //Rad::deserialize(s);
        s >> _items;

        return s;
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

    void remove(const QString& nm)
    {
        for(int k = 0; k < _items.size(); k++) {
            if (_items.at(k).name() == nm) {
                _items.remove(k);
                break;
            }
        }
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
                const KRadionuclide & nuc = KPluginManager::instance()->storage()->radionuclide(di.name());
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
                const KRadionuclide & nuc = KPluginManager::instance()->storage()->radionuclide(beg->name());
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

KDataItem::~KDataItem()
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
QDataStream & KDataItem::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    stream << _name << (int)_types << _value;
    return stream;
}
QDataStream & KDataItem::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    int itypes;
    stream >> _name >> itypes >> _value;
    _types = KData::ContentTypes(itypes);
    return stream;
}

/***********************************************************************************/
KData::KData()
    : dptr(new KDataPrivate())
{
}

KData::KData(const Quantity *qty)
    : dptr(new KDataPrivate(qty, qty->defaultValue))
{
}

KData::KData(const Quantity * qty, const QVariant& v)
    : dptr(new KDataPrivate(qty, v))
{
}
KData::KData(const Quantity * qty, ContentTypes  types, const QVariant & v)
    : dptr(new KDataPrivate(qty, types, v))
{

}

KData::KData(const Quantity * qty, const QString& nm, const QVariant& v, ContentTypes t)
    : dptr(new KDataPrivate(qty, nm, v, t))
{
}

KData::KData(const Quantity * qty, const DataItemArray & items)
    : dptr(new KDataPrivate(qty, items))
{
}
KData::KData(const Quantity * qty, const QMap<QString, qreal> & items, ContentTypes f)
    : dptr(new KDataPrivate(qty, items, f))
{
}

KData::KData(const KData& rhs) : dptr(rhs.dptr)
{
}
KData::KData(const Quantity * qty, const KData& other) : dptr(other.dptr)
{
    if (this != &other) {
        dptr.operator=(other.dptr);
        dptr->_quantity = qty;
    }
}

KData & KData::operator=(const KData& rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}
KData::~KData()
{
}

const Quantity & KData::quantity() const
{
    return *(dptr->_quantity);
}
const Quantity * KData::quantityPtr() const
{
    return dptr->_quantity;
}
KData::ContentTypes KData::contentTypes() const
{
    return dptr->_types;
}
bool KData::contains(ContentType type) const
{
    return dptr->_types.testFlag(type);
}

int KData::count() const
{
    return dptr->_items.size();
}
void KData::resize(int cnt)
{
    dptr->_items.resize(cnt);
}

bool KData::isValid() const
{
    //return !data->_items.isEmpty();
    return dptr->_types != KData::Undefined;
}
bool KData::isEmpty() const
{
    return dptr->_items.isEmpty();
}
void KData::append(const QString& nm, const QVariant& v, ContentTypes t)
{
    dptr->append(nm, v, t);
}
void KData::remove(const QString &nm)
{
    dptr->remove(nm);
}

QVariant KData::value(int idx) const
{
    return dptr->value(idx);
}
void KData::setValue(const QVariant &v)
{
    dptr->setValue(0, v);
}
void KData::setValue(int idx, const QVariant& v)
{
    dptr->setValue(idx, v);
}
qreal KData::numericValue(int idx) const
{
    return dptr->value(idx).toDouble();
}
qreal KData::numericValue(const QString& name, qreal defValue) const
{
    return dptr->numericValue(name, defValue);
}

const KDataItem & KData::at(const QString& name) const
{
    return dptr->itemAt(name);
}

const KDataItem & KData::at(int idx) const
{
    Q_ASSERT(!isEmpty());
    return dptr->_items.at(idx);
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
    int sz = dptr->_items.size();
    for (int k = 0; k < sz; k++) {
        dptr->_items[k] *= c;
    }
    return *this;
}

KData & KData::operator/=(qreal c)
{
    int sz = dptr->_items.size();
    for (int k = 0; k < sz; k++) {
        dptr->_items[k] /= c;
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
    return dptr->displayText();
}
QDataStream & KData::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return dptr->serialize(stream);
}
QDataStream & KData::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return dptr->deserialize(stream);
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
KDataArray::~KDataArray()
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
    DataList::const_iterator beg = this->constBegin();
    DataList::const_iterator end = this->constEnd();
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

void KDataArray::separateTo(KDataArray * dArray, KDataTable * dTable) const
{
    Q_ASSERT(dArray != 0);
    Q_ASSERT(dTable != 0);

    int sz = this->size();
    for(int k = 0; k < sz; k++) {
        const KData & dat = at(k);
        if (dat.contains(KData::Radionuclide) || dat.contains(KData::Array))
            dTable->append(dat);
        else
            dArray->append(dat);
    }
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
QDataStream & KDataArray::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    const DataList * dList = this;
    stream << *dList << _location;

    return stream;
}
QDataStream & KDataArray::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    DataList * dList = this;
    stream >> *dList >> _location;
    return stream;
}

///////////////////////////////////////////////////////////////////
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
KDataGroupArray::~KDataGroupArray()
{

}

const KData & KDataGroupArray::find(const Quantity& v, int gid) const
{
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                const KData & d = it->itemAt(k);
                if (d.quantity() == v)
                    return d;
            }
        }
        it++;
    }

    return __nullData;
}

KData * KDataGroupArray::findPtr(const Quantity& v, int gid)
{
    KDataGroupArray::iterator it = this->begin();
    KDataGroupArray::iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                KData * d = it->pointerAt(k);
                if (d->quantity() == v)
                    return d;
            }
        }
        it++;
    }

    return 0;
}


qreal KDataGroupArray::numericValueOf(const Quantity& v, int gid) const
{
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                const KData & d = it->itemAt(k);
                if (d.quantity() == v)
                    return d.numericValue();
            }
        }

        it++;
    }
    return __nullValue.toDouble();
}
QVariant KDataGroupArray::valueOf(const Quantity& v, int gid) const
{
    KDataGroupArray::const_iterator it = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                const KData & d = it->itemAt(k);
                if (d.quantity() == v)
                    return d.value();
            }
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

void KDataGroupArray::removeQuantity(const Quantity &v, int gid)
{
    KDataGroupArray::iterator it = this->begin();
    KDataGroupArray::iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                const KData & d = it->itemAt(k);
                if (d.quantity() == v) {
                    it->removeAt(k);
                    return;
                }
            }
        }
        it++;
    }
}

void KDataGroupArray::appendOrReplace(const KData& di, int gid)
{
    KDataGroupArray::iterator it = this->begin();
    KDataGroupArray::iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            //try replace item
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                const KData & d = it->itemAt(k);
                if (d.quantity() == di.quantity()) {
                    it->replaceAt(k, di);
                    return;
                }
            }

            //add new item
            it->add(di);
        }
        it++;
    }
}

void KDataGroupArray::replace(const KData &di, int gid)
{
    KDataGroupArray::iterator it = this->begin();
    KDataGroupArray::iterator end = this->end();
    while (it != end) {
        if (it->groupId == gid) {
            int nz = it->count();
            for(int k = 0; k < nz; k++) {
                const KData & d = it->itemAt(k);
                if (d.quantity() == di.quantity()) {
                    it->replaceAt(k, di);
                    return;
                }
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

void KDataGroupArray::separateTo(KDataGroupArray * dArray, KDataTable * dTable, KData::ContentTypes type) const
{
    Q_ASSERT(dArray != 0);
    Q_ASSERT(dTable != 0);

    KDataGroupArray::const_iterator gr = this->begin();
    KDataGroupArray::const_iterator end = this->end();
    while (gr != end) {
        int nz = gr->count();
        DataGroup group = *gr;
        for(int k = 0; k < nz; k++) {
            const KData & d = gr->itemAt(k);
            //TODO:
            //select only radionuclide (or parameterized)
            //if (d.contains(KData::Radionuclide) || d.contains(KData::Array)) {
            if ((d.contentTypes() & type) == type) {
                dTable->append(d);
                group.remove(d);
            }
        }
        if (!group.isEmpty()) {
            const KData & d1 = group.itemAt(0);
            //if (!(group.count() == 1 && (d1.quantity() == Rad::UseDefaultValue || d1.quantity() == Rad::UseDefaultValue2)))
            if (!(group.count() == 1 && d1.quantity().isComment()))
                dArray->append(group);
        }

        gr++;
    }
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

QDataStream & KDataGroupArray::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    const QVector<DataGroup> * vItems = this;
    stream << *vItems << _controlList;

    return stream;
}
QDataStream & KDataGroupArray::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    QVector<DataGroup> * vItems = this;
    stream >> *vItems >> _controlList;
    return stream;
}

//*************************************************************************************************

class KDataTablePrivate : public QSharedData
{
private:
    bool transposed;                    //Swap row and column
    ConstQuantityList quantities;       //Quantity describing values
    QStringList names;                  //Name associated to value
    QList<QList<QVariant> > values;
    //Table containing values. When not transposed, row: name, column: quantity

public:
    KDataTablePrivate() : transposed(false)
    {
    }

    //clear names, values, and quantities
    inline void clear() {
        quantities.clear();
        names.clear();
        values.clear();
    }

    inline const Quantity * quantity(int idx) const
    {
        if (idx >= 0 && idx < quantities.count())
            return quantities.at(idx);
        return 0;
    }
    inline QString name(int idx) const
    {
        if (idx >= 0 && idx < names.count())
            return names.at(idx);
        return QString();
    }

    inline bool isTransposed() const
    {
        return transposed;
    }
    inline void transpose()
    {
        transposed = !transposed;
    }
    inline int rowCount() const
    {
        if (transposed)
            return quantities.count();
        else
            return names.count();
    }
    inline int columnCount() const
    {
        if (transposed)
            return names.count();
        else
            return quantities.count();
    }

    QVariant value(int row, int col) const
    {
        if (transposed)
            return values.at(col).at(row);
        else
            return values.at(row).at(col);
    }

    QString columnHeader(int col) const
    {
        if (transposed)
            return names.at(col);
        else
            return quantities.at(col)->displayQuantityWithUnit();
    }

    QString rowHeader(int row) const
    {
        if (transposed)
            return quantities.at(row)->displayQuantityWithUnit();
        else
            return names.at(row);
    }

    void append(const KData & d)
    {
        quantities.append(d.quantityPtr());
        int sz = quantities.count();

        //merge
        for(int k = 0; k < d.count(); k++) {
            const KDataItem & di = d.at(k);
            int p = names.indexOf(di.name());
            if (p < 0) {
                QList<QVariant> vals;
                //create row with invalid values
                for(int k = sz - 2; k >= 0; k--)
                    vals.append(QVariant());
                //add new values
                vals.append(di.value());
                values.append(vals);
                names.append(di.name());
            }
            else
                values[p].append(di.value());   //add to existing row
        }

        for(int k = values.size() - 1; k >= 0; k--) {
            QList<QVariant> & vals = values[k];
            if (vals.size() < sz)
                vals.append(QVariant());        //add empty values
        }
    }

    inline void replace(int row, int col, const QVariant & v) {
        if (transposed)
            values[col].replace(row, v);
        else
            values[row].replace(col, v);
    }
    inline bool isEmpty() const
    {
        return quantities.isEmpty() || names.isEmpty();
    }
    int maxRowHeaderLength() const
    {
        int len = 0;
        int sz = rowCount();
        for(int k = 0; k < sz; k++)
            len = qMax(len, rowHeader(k).length());
        return len;
    }
    int maxColumnLength(int col) const
    {
        int nrow = rowCount();
        int len = columnHeader(col).length();

        for(int r = 0; r < nrow; r++)
            len = qMax(len, value(r,col).toString().length());

        return len;
    }
};

KDataTable::KDataTable() : dptr(new KDataTablePrivate())
{
}

KDataTable::KDataTable(const KDataTable& rhs) : dptr(rhs.dptr)
{
}

KDataTable & KDataTable::operator=(const KDataTable& rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KDataTable::~KDataTable()
{
}

void KDataTable::clear()
{
    dptr->clear();
}

void KDataTable::append(const KData& d)
{
    dptr->append(d);
}
void KDataTable::transpose()
{
    dptr->transpose();
}

bool KDataTable::isTransposed() const
{
    return dptr->isTransposed();
}

bool KDataTable::isEmpty() const
{
    return dptr->isEmpty();
}

int KDataTable::rowCount() const
{
    return dptr->rowCount();
}
int KDataTable::columnCount() const
{
    return dptr->columnCount();
}
int KDataTable::maxRowHeaderLength() const
{
    return dptr->maxRowHeaderLength();
}
int KDataTable::maxColumnLength(int col) const
{
    return dptr->maxColumnLength(col);
}
const Quantity * KDataTable::quantity(int idx) const
{
    return dptr->quantity(idx);
}
QString KDataTable::name(int idx) const
{
    return dptr->name(idx);
}
QString KDataTable::columnHeader(int col) const
{
    return dptr->columnHeader(col);
}
QString KDataTable::rowHeader(int row) const
{
    return dptr->rowHeader(row);
}
QVariant KDataTable::value(int row, int col) const
{
    return dptr->value(row, col);
}
void KDataTable::replace(int row, int col, const QVariant& v)
{
    dptr->replace(row, col, v);
}

QTextStream & operator<<(QTextStream & s, const KData & d)
{
    if (d.quantity().isComment())
        s << "//" << d.value().toString() << Rad::LatinEndLine;
    else if (d.isValid())
        s << d.displayText() << Rad::LatinEndLine;
    return s;
}

QTextStream & operator<<(QTextStream & s, const KDataArray & da)
{
    if (!da.isEmpty()) {
        s << Rad::LatinEndLine
          << "-----------------------------------------------"
             "----------------------------------" << Rad::LatinEndLine;
        for(int k = 0; k < da.size(); k++)
            s << da.at(k);
    }
    return s;
}

QTextStream & operator<<(QTextStream & s, const KDataTable & table)
{
    if (!table.isEmpty()) {
        int fw = s.fieldWidth();
        QTextStream::FieldAlignment fa = s.fieldAlignment();

        const int colPad = 3;
        int rowHeaderLen = table.maxRowHeaderLength();
        int nrow = table.rowCount();
        int ncol = table.columnCount();
        int lineLen = 4 + rowHeaderLen;
        for(int k = 0; k < ncol; k++)
            lineLen += (table.maxColumnLength(k) + colPad + 1); //column padding + separator

        //draw line for table header
        for(int k = 0; k < lineLen; k++)
            s << '=';
        s << Rad::LatinEndLine;

        //first column
        s.setFieldWidth(0);
        s << "| ";
        s.setFieldWidth(rowHeaderLen);
        s.setFieldAlignment(QTextStream::AlignLeft);
        s << " ";
        s.setFieldWidth(0);
        s << " |";

        //column header label
        s.setPadChar(' ');
        s.setFieldAlignment(QTextStream::AlignCenter);
        for(int c = 0; c < ncol; c++) {
            s.setFieldWidth(table.maxColumnLength(c) + colPad);
            s << table.columnHeader(c);
            s.setFieldWidth(0);
            s << "|";
        }
        s << Rad::LatinEndLine;

        s.setFieldWidth(0);
        s << "|";
        s.setPadChar('-');
        s.setFieldWidth(rowHeaderLen+2);
        s << "-";
        s.setFieldWidth(0);
        s << "|";
        for(int c = 0; c < ncol; c++) {
            s.setFieldWidth(table.maxColumnLength(c) + colPad);
            s << '-';
            s.setFieldWidth(0);
            s << "|";
        }
        s << Rad::LatinEndLine;

        s.setPadChar(' ');
        s.setFieldAlignment(QTextStream::AlignLeft);
        for(int r = 0; r < nrow; r++) {
            s.setFieldWidth(0);
            s << "| ";
            s.setFieldWidth(rowHeaderLen);
            s << table.rowHeader(r);
            s.setFieldWidth(0);
            s << " |";

            for(int c = 0; c < ncol; c++) {
                s.setFieldWidth(table.maxColumnLength(c) + colPad);
                s << (" " + table.value(r, c).toString());
                s.setFieldWidth(0);
                s << "|";
            }
            s << Rad::LatinEndLine;
        }

        //draw line
        for(int k = 0; k < lineLen; k++)
            s << '-';
        s << Rad::LatinEndLine;

        //restore
        s.setFieldAlignment(fa);
        s.setFieldWidth(fw);
    }

    return s;
}



