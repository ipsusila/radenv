#include <QtDebug>
#include <QPainter>
#include <QGraphicsScene>
#include "imodel.h"
#include "kport.h"
#include "kconnector.h"
#include "koutput.h"

KData KPortList::data(const Quantity &qty) const
{
    for(int k = 0; k < this->size(); k++) {
        KData d = this->at(k)->data(qty);
        if (d.isValid())
            return d;
    }
    return KData();
}

KData KPortList::data(int idx, const Quantity &qty) const
{
    if (idx < 0 || idx >= size())
        return KData();

    return this->at(idx)->data(qty);
}

KLocation KPortList::firstValidLocation() const
{
    for(int k = 0; k < this->size(); k++) {
        KLocation loc = at(k)->firstValidLocation();
        if (loc.isValid())
            return loc;
    }
    return KLocation();
}

bool KPortList::isConnected(int idx) const
{
    if (idx < 0 || idx >= this->size())
        return false;
    return this->at(idx)->isConnected();
}
bool KPortList::isAllConnected() const
{
    for(int k = 0; k < size(); k++)
        if (!at(k)->isConnected())
            return false;
    return !isEmpty();
}
bool KPortList::isAnyConnected() const
{
    for(int k = 0; k < size(); k++)
        if (at(k)->isConnected())
            return true;
    return false;
}

KPort::KPort(IModel * m, const Quantity * qty, KPort::DataDirection dir)
    : QGraphicsItem(m), _model(m), _quantity(qty), _direction(dir)
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

const Quantity * KPort::quantity() const
{
    return _quantity;
}
KPort::DataDirection KPort::direction() const
{
    return _direction;
}
const KPortList & KPort::connectedPorts() const
{
    return _conPorts;
}
ConnectorList KPort::connectors() const
{
    return _conList;
}
KLocation KPort::firstValidLocation() const
{
    KLocation loc;
    if ((direction() & KPort::Output) == KPort::Output)
    {
        IModel * model = this->model();
        if (model)
            loc = model->result().location();
    }
    else {
        foreach(KPort * p, _conPorts) {
            IModel * model = p->model();
            if (model) {
                loc = model->result().location();
                if (loc.isValid())
                    break;
            }
        }
    }
    return loc;
}
KDataArray KPort::data() const
{
    KDataArray da;
    if ((direction() & KPort::Output) == KPort::Output)
    {
        IModel * model = this->model();
        if (model)
            da = model->result();
    }
    else {
        foreach(KPort * p, _conPorts) {
            IModel * model = p->model();
            if (model)
                da << model->result();
        }
    }
    return da;
}
KData KPort::data(const Quantity &qty) const
{
    //for output port
    //request data from the model
    if ((direction() & KPort::Output) == KPort::Output)
    {
        IModel * model = this->model();
        if (model)
            return model->data(qty);
    }

    if ((direction() & KPort::Input) == KPort::Input) {
        //for input port
        //request data from connected model
        //return when valid data found
        foreach(KPort * p, _conPorts) {
            IModel * model = p->model();
            if (model) {
                KData d = model->data(qty);
                if (d.isValid())
                    return d;
            }
        }
    }

    //if not any valid data found
    //return invalid data
    return KData();
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
        return (port->quantity() == this->quantity()) != 0;
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
        painter->drawText(rect.right()+2, rect.top(), w, rect.height(), Qt::AlignLeft | Qt::AlignVCenter, _quantity->symbol);
    }
    else {
        painter->drawText(rect.x() - w, rect.top(), w-2, rect.height(), Qt::AlignRight | Qt::AlignVCenter, _quantity->symbol);
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

