#ifndef KPORT_H
#define KPORT_H

#include <QGraphicsItem>
#include "kdata.h"

class RADENV_API KPortList : public QList<KPort *>
{
public:
    KData data(const Quantity& qty) const;
    KData data(int idx, const Quantity& qty) const;
    KLocation firstValidLocation() const;
    bool isConnected(int idx = 0) const;
    bool isAllConnected() const;
    bool isAnyConnected() const;
};

class KPortPrivate;
class RADENV_API KPort : public QGraphicsItem
{
public:
    enum { Type = UserType + TYPE_PORT};

    enum Direction
    {
        Input       = 0x01,
        Output      = 0x02,
        InputOutput = Input | Output
    };
    Q_DECLARE_FLAGS(DataDirection, Direction)

    KPort(IModel * m, const Quantity * qty, DataDirection dir);
    virtual ~KPort();

    bool isConnected() const;
    const Quantity * quantity() const;
    DataDirection direction() const;
    const KPortList & connectedPorts() const;
    ConnectorList connectors() const;
    KDataArray data() const;            //deprecated (will be removed)
    KData data(const Quantity& qty) const;
    KData data(int idx) const;
    KLocation firstValidLocation() const;
    void setIndex(int idx);
    int index() const;
    bool isAutoHide() const;
    void setAutoHide(bool ah);

    void removeConnections();
    void connectTo(KPort * dest, KConnector * con);
    void disconnectFrom(KPort * dest, KConnector * con);

    IModel * model() const;
    int type() const;
    virtual bool canConnect(KPort * port) const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void rearrangeConnectors(const QPointF& oldPos, const QPointF& newPos);

private:
    Q_DISABLE_COPY(KPort)
    QSharedDataPointer<KPortPrivate> dptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KPort::DataDirection)

#endif // KPORT_H
