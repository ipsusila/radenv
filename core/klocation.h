#ifndef KLOCATION_H
#define KLOCATION_H

#include <QImage>
#include <QExplicitlySharedDataPointer>
#include "radglobal.h"

class KLocationPrivate;
class K_CORE_EXPORT KLocation
{
private:
    QExplicitlySharedDataPointer<KLocationPrivate> data;
public:
    static const char * Delimiter;

    KLocation(const QString& c = QString());
    KLocation(const KLocation&);
    KLocation &operator=(const KLocation&);
    virtual ~KLocation();

    bool operator==(const KLocation &other) const;
    bool operator!=(const KLocation &other) const;

    void setLatitude(qreal lat);
    qreal latitude() const;

    void setLongitude(qreal lo);
    qreal longitude() const;

    void setAngle(qreal theta);
    qreal angle() const;

    void setName(const QString& n);
    QString name() const;
    QString code() const;

    void setDescription(const QString& d);
    QString description() const;

    void setMarker(const QString& path);
    void setMarker(const QImage& img);
    QImage marker() const;

    QString displayText() const;
    QString detailDisplayText() const;
    bool isValid() const;

    void setDistance(const QString& strDistance, const QString& sep);

    /**
     * @brief Set distance (in metre)
     * @param d - distance in metre
     */
    void addDistance(const qreal & d);

    /**
     * @brief Get distance from discharge source
     * @return distance in metre (m)
     */
    qreal distance() const;
    QList<qreal> distances() const;
    qreal distance(const KCalculationInfo& ci) const;
    QString distances(const QString& sep) const;

    KLocation clone() const;
    qreal distanceBetween(const KLocation & loc) const;
};

#endif // KLOCATION_H
