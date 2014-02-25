#include <QSharedData>
#include <QVector>
#include "kdata.h"
#include "kgroup.h"
#include "koutput.h"

/** empty variable */
namespace Rad {

const char LatinEndLine = '\n';

const Symbol EmptySymbol = {0, Rad::Real, 0, 0, 0, 0, "","","","","",""};
const Symbol ScalarInput =
    {std::numeric_limits<unsigned int>::max(), Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X","X",QObject::tr("Input"),"","","Arbitrary numeric value"};
const Symbol ScalarInput1 =
    {std::numeric_limits<unsigned int>::max() - 1, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X1","X1",QObject::tr("Input 1"),"","","Arbitrary numeric value"};
const Symbol ScalarInput2 =
    {std::numeric_limits<unsigned int>::max() - 2, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X2","X2",QObject::tr("Input 2"),"","","Arbitrary numeric value"};
const Symbol ScalarInput3 =
    {std::numeric_limits<unsigned int>::max() - 3, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X3","X3",QObject::tr("Input 3"),"","","Arbitrary numeric value"};
const Symbol ScalarInput4 =
    {std::numeric_limits<unsigned int>::max() - 4, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X4","X4",QObject::tr("Input 4"),"","","Arbitrary numeric value"};
const Symbol ScalarInput5 =
    {std::numeric_limits<unsigned int>::max() - 5, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X5","X5",QObject::tr("Input 5"),"","","Arbitrary numeric value"};
const Symbol ScalarInput6 =
    {std::numeric_limits<unsigned int>::max() - 6, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X6","X6",QObject::tr("Input 6"),"","","Arbitrary numeric value"};
const Symbol ScalarInput7 =
    {std::numeric_limits<unsigned int>::max() - 7, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X7","X7",QObject::tr("Input 7"),"","","Arbitrary numeric value"};
const Symbol ScalarInput8 =
    {std::numeric_limits<unsigned int>::max() - 8, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X8","X8",QObject::tr("Input 8"),"","","Arbitrary numeric value"};
const Symbol ScalarInput9 =
    {std::numeric_limits<unsigned int>::max() - 9, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X9","X9",QObject::tr("Input 9"),"","","Arbitrary numeric value"};

const Symbol ScalarOutput =
    {std::numeric_limits<unsigned int>::max() - 10, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y","Y",QObject::tr("Output"),"","","Arbitrary numeric value"};
const Symbol ScalarOutput1 =
    {std::numeric_limits<unsigned int>::max() - 11, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y1","Y1",QObject::tr("Output 1"),"","","Arbitrary numeric value"};
const Symbol ScalarOutput2 =
    {std::numeric_limits<unsigned int>::max() - 12, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y2","Y2",QObject::tr("Output 2"),"","","Arbitrary numeric value"};
const Symbol ScalarOutput3 =
    {std::numeric_limits<unsigned int>::max() - 13, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y3","Y3",QObject::tr("Output 3"),"","","Arbitrary numeric value"};
const Symbol ScalarOutput4 =
    {std::numeric_limits<unsigned int>::max() - 14, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y4","Y4",QObject::tr("Output 4"),"","","Arbitrary numeric value"};
const Symbol ScalarOutput5 =
    {std::numeric_limits<unsigned int>::max() - 15, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y5","Y5",QObject::tr("Output 5"),"","","Arbitrary numeric value"};

const Symbol CommentSymbol =
    {std::numeric_limits<unsigned int>::max() - 16, Rad::Comment, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "C","C",QObject::tr("Comment"),"","","Arbitrary comment"};
}

/** null parameter */
static const KData __nullData;
static const QVariant __nullValue;

class KDataPrivate : public QSharedData
{
public:
    const Symbol * _symbol;
    //XData::Types _types;
    QVector<KDataItem> _items;

    KDataPrivate() : _symbol(&Rad::EmptySymbol)
    {
    }
    KDataPrivate(const Symbol * var, const QVariant& v) : _symbol(var),
        _items(QVector<KDataItem>(1, KDataItem(var->symbol, v)))
    {
        Q_ASSERT(_symbol != 0);
    }

    KDataPrivate(const Symbol * var, const QVector<KDataItem> & it)
        : _symbol(var), _items(it)
    {
        Q_ASSERT(_symbol != 0);
    }
    inline void setValue(int idx, const QVariant& v)
    {
        Q_ASSERT(idx < _items.size());
        _items[idx].setValue(v);
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
    inline QString toString(const KDataItem &di) const
    {
        bool flag;
        switch(_symbol->type) {
        case Rad::Boolean:
            flag = di.value().toBool();
            return (flag ? "True" : "False");
        default:
            return QString::number(di.numericValue());
        }
    }

    QString displayText() const
    {
        if (_items.isEmpty()) {
            return QString();
        }
        else if (_items.count() == 1) {
            return QString("%1: %2 %3").arg(_symbol->displayText())
                    .arg(toString(_items.at(0))).arg(_symbol->unit);
        }

        QVector<KDataItem>::const_iterator beg = _items.begin();
        QVector<KDataItem>::const_iterator end = _items.end();
        QString txt = _symbol->displayText();

        while (beg != end) {
            txt += QString("\n  %1: %2 %3").arg(beg->name())
                    .arg(toString(*beg)).arg(_symbol->unit);
            beg++;
        }

        return txt;
    }
};

/***********************************************************************************/
KDataItem::KDataItem()
    : _name(QString()), _value(QVariant()) {}
KDataItem::KDataItem(const QString& nm, const QVariant& v)
    : _name(nm), _value(v) {}
KDataItem::KDataItem(const KDataItem& other)
    : _name(other._name), _value(other._value) {}
KDataItem &KDataItem::operator=(const KDataItem& rhs) {
    if (this != &rhs) {
        _name = rhs._name;
        _value = rhs._value;
    }
    return *this;
}
QString KDataItem::name() const { return _name; }
QVariant KDataItem::value() const { return _value; }
qreal KDataItem::numericValue() const { return _value.toDouble(); }
void KDataItem::setValue(const QVariant &v)
{
    _value = v;
}

KDataItem KDataItem::operator*(qreal c) const
{
    return KDataItem(_name, _value.toDouble()*c);
}

KDataItem KDataItem::operator/(qreal c) const
{
    return KDataItem(_name, _value.toDouble()/c);
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
KData::KData(const Symbol * var, const QVariant& v)
    : data(new KDataPrivate(var, v))
{
}

KData::KData(const Symbol * var, const QVector<KDataItem> & items)
    : data(new KDataPrivate(var, items))
{
}

KData::KData(const KData& rhs) : data(rhs.data)
{
}
KData::KData(const Symbol * sym, const KData& other) : data(other.data)
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

const Symbol & KData::symbol() const
{
    return *(data->_symbol);
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
    return !data->_items.isEmpty();
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

const KDataItem & KData::at(int idx) const
{
    Q_ASSERT(isValid());
    return data->_items.at(idx);
}
KDataItem & KData::operator[](int idx)
{
    Q_ASSERT(isValid());
    return data->_items[idx];
}

const KDataItem& KData::operator[](int idx) const
{
    Q_ASSERT(isValid());
    return data->_items[idx];
}
KData KData::operator*(qreal c) const
{
    KData p(*this);
    int sz = data->_items.size();
    for (int k = 0; k < sz; k++) {
        p[k] = data->_items.at(k) * c;
    }

    return p;
}

KData KData::operator/(qreal c) const
{
    KData p(*this);
    int sz = data->_items.size();
    for (int k = 0; k < sz; k++) {
        p[k] = data->_items.at(k) / c;
    }

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
    : QList<KData>(o)
{
}
KDataArray::KDataArray(const QList<KData> &o)
    : QList<KData>(o)
{
}

const KData & KDataArray::find(const QString& sym) const
{
    QList<KData>::const_iterator beg = this->begin();
    QList<KData>::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol().symbol == sym)
            return *beg;
        beg++;
    }

    return __nullData;
}

const KData & KDataArray::find(const Symbol& v) const
{
    QList<KData>::const_iterator beg = this->begin();
    QList<KData>::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol() == v)
            return *beg;
        beg++;
    }

    return __nullData;
}

qreal KDataArray::numericValueOf(const Symbol& v) const
{
    QList<KData>::const_iterator beg = this->begin();
    QList<KData>::const_iterator end = this->end();
    while (beg != end) {
        if (beg->symbol() == v)
            return beg->numericValue();
        beg++;
    }
    return __nullValue.toDouble();
}

QVariant KDataArray::valueOf(const Symbol& v) const
{
    QList<KData>::const_iterator beg = this->begin();
    QList<KData>::const_iterator end = this->end();
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

    QList<KData>::const_iterator beg = this->begin();
    QList<KData>::const_iterator end = this->end();
    QString txt = beg->displayText();

    while ((++beg) != end) {
        txt += "\n" + beg->displayText();
    }

    return txt;
}
void KDataArray::addOrReplace(const KData &di)
{
    for(int k = 0; k < size(); k++) {
        if (this->at(k).symbol() == di.symbol()) {
            (*this)[k] = di;
            return;
        }
    }
    this->append(di);
}

const KData & KDataGroupArray::find(const Symbol& v) const
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
qreal KDataGroupArray::numericValueOf(const Symbol& v) const
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
QVariant KDataGroupArray::valueOf(const Symbol& v) const
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



