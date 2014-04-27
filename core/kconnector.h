#ifndef KCONNECTOR_H
#define KCONNECTOR_H

#include <QGraphicsItem>
#include <QSharedDataPointer>
#include "radglobal.h"

class KConnectorPrivate;
class K_CORE_EXPORT KConnector : public QGraphicsItem
{
public:
    enum { Type = UserType + TYPE_CONNECTOR};

    KConnector();
    KConnector(const KConnector &);
    KConnector &operator=(const KConnector &);
    ~KConnector();

    void setTemporaryPoint(const QPointF& pt);  ///< scene coordinate
    void addPoint(const QPointF &pt);           ///< scene coordinate
    void setPort(KPort * port);

    KPort * inputPort() const;
    KPort * outputPort() const;
    KPort * oppositePort(KPort * port) const;
    void movePos(KPort * p, const QPointF & oldPos, const QPointF & newPos);

    bool isConnected() const;
    void connect();
    void disconnect();
    void copyTo(KConnector * con, KPort * out, KPort * inp) const;

    int type() const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    
private:
    QSharedDataPointer<KConnectorPrivate> data;
};

#endif // KCONNECTOR_H
