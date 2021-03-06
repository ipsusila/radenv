#include <QSharedData>
#include <QtCore/qmath.h>
#include <QStringList>
#include <QtDebug>
#include "klocation.h"
#include "kcalculationinfo.h"
#include "kstorage.h"
#include "kapplication.h"

class KLocationPrivate : public QSharedData
{
private:
    QString _code;
    qreal _latitude;
    qreal _longitude;
    qreal _angle;
    QString _name;
    QString _description;
    QList<qreal> _distances;
    QImage _marker;

public:
    KLocationPrivate(const QString& c) : _code(c) {
        _latitude = 0;
        _longitude = 0;
        _angle = 0;
        _name = QString();
        _description = QString();
    }

    inline qreal latitude() const { return _latitude; }
    inline void setLatitude(qreal lat) {
        _latitude = lat;
    }

    inline qreal longitude() const { return _longitude; }
    inline void setLongitude(qreal lo) {
        _longitude = lo;
    }

    inline qreal angle() const { return _angle; }
    inline void setAngle(qreal theta) {
        if (theta < -360.0)
            _angle = -360.0;
        else if (theta > 360.0)
            _angle = 360.0;
        else
            _angle = theta;
    }

    inline QString code() const { return _code; }
    inline QString name() const { return _name; }
    inline void setName(const QString& n) {
        _name = n;
    }

    inline QString description() const { return _description; }
    inline void setDescription(const QString& d) {
        _description = d;
    }

    inline const QList<qreal> & distances() const {
        return _distances;
    }
    inline qreal distance() const {
        return _distances.isEmpty() ? 0 : _distances.first();
    }
    inline qreal distance(const KCalculationInfo & ci) const {
        return _distances.isEmpty() ? 0 : _distances.at(ci.runId());
    }
    inline void addDistance(const qreal& d) {
        _distances.append(d);
    }
    inline void setDistance(const QString& strDistance, const QString& sep) {
        QStringList dList = strDistance.split(sep, QString::SkipEmptyParts);
        for(int k = 0; k < dList.count(); k++) {
            qreal d = dList.at(k).toDouble();
            _distances.append(d);
        }
    }

    inline QImage marker() const { return _marker; }
    inline void setMarker(const QImage& m) {
        _marker = m;
    }
    inline void setMarker(const QString& path) {
        if (!path.isEmpty())
            _marker = QImage(path);
    }
};

const char * KLocation::Delimiter = ";";    //default delimiter
KLocation::KLocation(const QString& c) : dptr(new KLocationPrivate(c))
{
}
KLocation::KLocation(const KLocation &rhs) : dptr(rhs.dptr)
{
}
KLocation & KLocation::operator=(const KLocation &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KLocation::~KLocation()
{

}

bool KLocation::operator==(const KLocation &other) const
{
    return dptr->code() == other.dptr->code();
}
bool KLocation::operator!=(const KLocation &other) const
{
    return dptr->code() != other.dptr->code();
}

bool KLocation::isValid() const
{
    return !dptr->code().isEmpty() && !dptr->name().isEmpty();
}

void KLocation::setLatitude(qreal lat)
{
    dptr->setLatitude(lat);
}

qreal KLocation::latitude() const
{
    return dptr->latitude();
}

void KLocation::setLongitude(qreal lo)
{
    dptr->setLongitude(lo);
}

qreal KLocation::longitude() const
{
    return dptr->longitude();
}

void KLocation::setAngle(qreal theta)
{
    dptr->setAngle(theta);
}

qreal KLocation::angle() const
{
    return dptr->angle();
}

void KLocation::setName(const QString& n)
{
    dptr->setName(n);
}

QString KLocation::name() const
{
    return dptr->name();
}

QString KLocation::code() const
{
    return dptr->code();
}

void KLocation::setDescription(const QString& d)
{
    dptr->setDescription(d);
}

QString KLocation::description() const
{
    return dptr->description();
}

void KLocation::setDistance(const QString& strDistance, const QString& sep)
{
    dptr->setDistance(strDistance, sep);
}

void KLocation::addDistance(const qreal & d)
{
    dptr->addDistance(d);
}

QString KLocation::distances(const QString& sep) const
{
    const QList<qreal> & dList = dptr->distances();
    QList<qreal>::const_iterator iter = dList.constBegin();
    QList<qreal>::const_iterator end = dList.constEnd();

    qreal d;
    QString strList;
    if (iter != end) {
        d = *(iter++);
        if ((d - (int)d) == 0)
            strList = QString::number(d, 'f', 0);
        else
            strList = QString::number(d, 'f', 2);
    }
    while(iter != end) {
        d = *(iter++);
        if ((d-(int)d) == 0)
            strList += sep + QString::number(d, 'f', 0);
        else
            strList += sep + QString::number(*iter, 'f', 2);
    }
    return strList;
}

QList<qreal> KLocation::distances() const
{
    return dptr->distances();
}

qreal KLocation::distance() const
{
    return dptr->distance();
}
qreal KLocation::distance(const KCalculationInfo& ci) const
{
    return dptr->distance(ci);
}

void KLocation::setMarker(const QString& path)
{
    dptr->setMarker(path);
}

void KLocation::setMarker(const QImage &img)
{
    dptr->setMarker(img);
}
QImage KLocation::marker() const
{
    return dptr->marker();
}

KLocation KLocation::clone() const
{
    KLocation loc = *this;
    loc.dptr.detach();

    return loc;
}

qreal KLocation::distanceBetween(const KLocation & loc) const
{
    qreal lat1 = this->latitude();
    qreal long1 = this->longitude();

    qreal lat2 = loc.latitude();
    qreal long2 = loc.longitude();

    //calculate distance
    qreal dLat = lat2 - lat1;
    qreal dLong = long2 - long1;

    return qSqrt(dLat*dLat + dLong*dLong);
}

QString KLocation::displayText() const
{
    QString txt = code() + ": " + name();
    if (distance() > 0)
        txt += "(" + QString::number(distance(), 'f', 0) + " m)";
    return txt;
}
QString KLocation::detailDisplayText() const
{
    QString txt = displayText();
    QString desc = dptr->description();
    if (!desc.isEmpty())
        txt += "\n" + desc;

    return txt;
}

QDataStream & KLocation::serialize(QDataStream & stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    stream << this->code();
    return stream;
}
QDataStream & KLocation::deserialize(QDataStream & stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    QString c;
    stream >> c;
    *this = KApplication::selfInstance()->storage()->location(c);

    return stream;
}
