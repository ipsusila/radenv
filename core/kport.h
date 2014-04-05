#ifndef KPORT_H
#define KPORT_H

#include <QGraphicsItem>
#include "kdata.h"

class KPortList : public QList<KPort *>
{
public:
    KData data(const Quantity& sym) const;
    KData data(int idx, const Quantity& sym) const;
    KLocation firstValidLocation() const;
    bool isConnected(int idx = 0) const;
    bool isAllConnected() const;
    bool isAnyConnected() const;
};

class K_CORE_EXPORT KPort : public QGraphicsItem
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

    KPort(IModel * m, const Quantity * sym, DataDirection dir);
    virtual ~KPort();

    bool isConnected() const;
    const Quantity * symbol() const;
    DataDirection direction() const;
    const KPortList & connectedPorts() const;
    ConnectorList connectors() const;
    KDataArray data() const;            //deprecated (will be removed)
    KData data(const Quantity& sym) const;
    KLocation firstValidLocation() const;

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
    IModel *            _model;
    const Quantity *      _symbol;
    KPort::DataDirection _direction;
    KPortList            _conPorts;
    ConnectorList       _conList;
};


Q_DECLARE_OPERATORS_FOR_FLAGS(KPort::DataDirection)

#endif // KPORT_H
