#ifndef KCONNECTOR_H
#define KCONNECTOR_H

#include <QGraphicsItem>
#include "iserializable.h"

class KConnectorPrivate;
class RADENV_API KConnector : public QGraphicsItem, public ISerializable
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

    bool isValid() const;
    bool isConnected() const;
    void connect();
    void disconnect();
    void copyTo(KConnector * con, KPort * out, KPort * inp) const;

    virtual int type() const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual QDataStream & serialize(QDataStream &stream) const;
    virtual QDataStream & deserialize(QDataStream &stream);
    
private:
    QSharedDataPointer<KConnectorPrivate> dptr;
};

K_DECLARE_SERIALIZABLE(KConnector)

#endif // KCONNECTOR_H
