#include <QSharedData>
#include <QtCore/qmath.h>
#include "klocation.h"
#include "kcalculationinfo.h"

class KLocationPrivate : public QSharedData
{
private:
    QString _code;
    qreal _latitude;
    qreal _longitude;
    qreal _angle;
    QString _name;
    QString _description;
    qreal _distance;
    QImage _marker;

public:
    KLocationPrivate(const QString& c) : _code(c) {
        _latitude = 0;
        _longitude = 0;
        _angle = 0;
        _name = QString();
        _description = QString();
        _distance = 0;
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

    inline qreal distance() const { return _distance; }
    inline void setDistance(const qreal& d) {
        _distance = d;
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

KLocation::KLocation(const QString& c) : data(new KLocationPrivate(c))
{
}
KLocation::KLocation(const KLocation &rhs) : data(rhs.data)
{
}
KLocation & KLocation::operator=(const KLocation &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KLocation::~KLocation()
{

}

bool KLocation::operator==(const KLocation &other) const
{
    return data->code() == other.data->code();
}
bool KLocation::operator!=(const KLocation &other) const
{
    return data->code() != other.data->code();
}

bool KLocation::isValid() const
{
    return !data->code().isEmpty() && !data->name().isEmpty();
}

void KLocation::setLatitude(qreal lat)
{
    data->setLatitude(lat);
}

qreal KLocation::latitude() const
{
    return data->latitude();
}

void KLocation::setLongitude(qreal lo)
{
    data->setLongitude(lo);
}

qreal KLocation::longitude() const
{
    return data->longitude();
}

void KLocation::setAngle(qreal theta)
{
    data->setAngle(theta);
}

qreal KLocation::angle() const
{
    return data->angle();
}

void KLocation::setName(const QString& n)
{
    data->setName(n);
}

QString KLocation::name() const
{
    return data->name();
}

QString KLocation::code() const
{
    return data->code();
}

void KLocation::setDescription(const QString& d)
{
    data->setDescription(d);
}

QString KLocation::description() const
{
    return data->description();
}

void KLocation::setDistance(const qreal & d)
{
    data->setDistance(d);
}

qreal KLocation::distance() const
{
    return data->distance();
}
qreal KLocation::distance(const KCalculationInfo& ci) const
{
    Q_UNUSED(ci);

    //TODO
    //handle multiple locations
    return data->distance();
}

void KLocation::setMarker(const QString& path)
{
    data->setMarker(path);
}

void KLocation::setMarker(const QImage &img)
{
    data->setMarker(img);
}
QImage KLocation::marker() const
{
    return data->marker();
}

KLocation KLocation::clone() const
{
    KLocation loc = *this;
    loc.data.detach();

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
    QString desc = data->description();
    if (!desc.isEmpty())
        txt += "\n" + desc;

    return txt;
}
