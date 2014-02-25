#include <QtDebug>
#include <QPainter>
#include <QGraphicsScene>
#include "imodel.h"
#include "kport.h"
#include "kconnector.h"
#include "koutput.h"

KPort::KPort(IModel * m, const Symbol * sym, KPort::DataDirection dir)
    : QGraphicsItem(m), _model(m), _symbol(sym), _direction(dir)
{
    //calculate position
    this->setZValue(m->zValue()+1);
}
KPort::~KPort()
{
    qDebug() << "Port destroyed";
}

IModel * KPort::model() const
{
    return _model;
}

const Symbol * KPort::symbol() const
{
    return _symbol;
}
KPort::DataDirection KPort::direction() const
{
    return _direction;
}
const PortList & KPort::connectedPorts() const
{
    return _conPorts;
}
ConnectorList KPort::connectors() const
{
    return _conList;
}
KDataArray KPort::result() const
{
    KDataArray da;
    foreach(KPort * p, _conPorts) {
        IModel * model = p->model();
        if (model)
            da << model->result();
    }
    return da;
}

bool KPort::isConnected() const
{
    return !_conPorts.isEmpty();
}

void KPort::connectTo(KPort *dest, KConnector *con)
{
    Q_ASSERT(dest);
    Q_ASSERT(con);
    Q_ASSERT(con->isConnected());

    if (con->oppositePort(this) == dest) {
        _conPorts.append(dest);
        dest->_conPorts.append(this);
        _conList.append(con);
        dest->_conList.append(con);
    }
}
void KPort::disconnectFrom(KPort * dest, KConnector * con)
{
    Q_ASSERT(dest);
    Q_ASSERT(con);
    Q_ASSERT(con->isConnected());
    if (con->oppositePort(this) == dest) {
        _conPorts.removeOne(dest);
        dest->_conPorts.removeOne(this);
        _conList.removeOne(con);
        dest->_conList.removeOne(con);
    }
}

void KPort::removeConnections()
{
    QGraphicsScene * sc = this->scene();
    while (!_conList.isEmpty()) {
        //call disconnect will also remove connector
        KConnector * con = _conList.first();
        con->disconnect();

        if (sc != 0)
            sc->removeItem(con);
        delete con;
    }
}

bool KPort::canConnect(KPort * port) const
{
    bool tInp = (this->direction() & KPort::Input) == KPort::Input;
    bool tOut = (this->direction() & KPort::Output) == KPort::Output;
    bool oInp = (port->direction() & KPort::Input) == KPort::Input;
    bool oOut = (port->direction() & KPort::Output) == KPort::Output;

    //if direction is OK, then check type
    //this: input -> other: output, this: output -> other: input
    if ((tInp && oOut) || (tOut || oInp)) {
        return (port->symbol() == this->symbol()) != 0;
    }
    else {
        return false;
    }
}
QRectF KPort::boundingRect () const
{
    return QRectF(-8, -6, 16, 12);
}
void KPort::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect = boundingRect();
    float w = model()->modelRect().width();

    painter->save();
    painter->setBrush(Qt::lightGray);
    painter->setPen(QPen(Qt::darkGray, 1));
    painter->drawRect(rect);

    QFont f = painter->font();
    f.setPointSize(10);
    painter->setFont(f);
    if (xHas(this->direction(), KPort::Input)) {
        painter->drawText(rect.right()+2, rect.top(), w, rect.height(), Qt::AlignLeft | Qt::AlignVCenter, _symbol->symbol);
    }
    else {
        painter->drawText(rect.x() - w, rect.top(), w-2, rect.height(), Qt::AlignRight | Qt::AlignVCenter, _symbol->symbol);
    }

    painter->restore();
}
void KPort::rearrangeConnectors(const QPointF& oldPos, const QPointF& newPos)
{
    qDebug() << "Rearrange connector, old: " << oldPos << ",new: " << newPos;
}

int KPort::type() const
{
    return Type;
}

