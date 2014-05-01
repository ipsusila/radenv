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

class KPortPrivate : public QSharedData
{
public:
    IModel *                _model;
    const Quantity *        _quantity;
    KPort::DataDirection    _direction;
    int                     _index;
    bool                    _autoHide;
    KPortList               _conPorts;
    ConnectorList           _conList;

    KPortPrivate(IModel * m, const Quantity * qty, KPort::DataDirection dir)
        : _model(m), _quantity(qty), _direction(dir), _index(0), _autoHide(false)
    {
    }

    KLocation firstValidLocation() const
    {
        KLocation loc;
        if ((_direction & KPort::Output) == KPort::Output)
        {
            if (_model != 0)
                loc = _model->result().location();
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

    KDataArray data() const
    {
        KDataArray da;
        if ((_direction & KPort::Output) == KPort::Output)
        {
            if (_model != 0)
                da = _model->result();
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

    KData data(const Quantity &qty) const
    {
        //for output port
        //request data from the model
        if ((_direction & KPort::Output) == KPort::Output)
        {
            if (_model)
                return _model->data(qty);
        }

        if ((_direction & KPort::Input) == KPort::Input) {
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

    KData data(int idx) const
    {
        if (xHas(_direction, KPort::Output))
        {
            if (_model != 0)
                return _model->data(*_quantity);
        }
        else if (xHas(_direction, KPort::Input)) {
            if (idx >= 0 && idx < _conPorts.size()) {
                KPort * p = _conPorts.at(idx);
                IModel * m = p->model();
                if (m != 0)
                    return m->data(*(p->quantity()));
            }
        }

        //if not any valid data found
        //return invalid data
        return KData();
    }

    inline void append(KPort * p, KConnector * c)
    {
        _conPorts.append(p);
        _conList.append(c);
    }

    inline void remove(KPort * p, KConnector * c)
    {
        _conPorts.removeOne(p);
        _conList.removeOne(c);
    }

    void removeConnections(KPort * port)
    {
        QGraphicsScene * sc = port->scene();
        while (!_conList.isEmpty()) {
            //call disconnect will also remove connector
            KConnector * con = _conList.first();
            con->disconnect();

            if (sc != 0)
                sc->removeItem(con);
            delete con;
        }
    }

    inline void rearrangeConnectors(KPort * port, const QPointF& oldPos, const QPointF& newPos)
    {
        foreach(KConnector * con, _conList)
            con->movePos(port, oldPos, newPos);
    }

};


KPort::KPort(IModel * m, const Quantity * qty, KPort::DataDirection dir)
    : QGraphicsItem(m), dptr(new KPortPrivate(m, qty, dir))
{
    //calculate position
    this->setZValue(m->zValue()+1);
    this->setVisible(!isAutoHide());
}
KPort::~KPort()
{
    qDebug() << "Port destroyed";
}

IModel * KPort::model() const
{
    return dptr->_model;
}

const Quantity * KPort::quantity() const
{
    return dptr->_quantity;
}
KPort::DataDirection KPort::direction() const
{
    return dptr->_direction;
}
const KPortList & KPort::connectedPorts() const
{
    return dptr->_conPorts;
}
ConnectorList KPort::connectors() const
{
    return dptr->_conList;
}
void KPort::setIndex(int idx)
{
    dptr->_index = idx;
}

int KPort::index() const
{
    return dptr->_index;
}
bool KPort::isAutoHide() const
{
    return dptr->_autoHide;
}
void KPort::setAutoHide(bool ah)
{
    dptr->_autoHide = ah;
}

KLocation KPort::firstValidLocation() const
{
    return dptr->firstValidLocation();
}

KDataArray KPort::data() const
{
    return dptr->data();
}

KData KPort::data(const Quantity &qty) const
{
    return dptr->data(qty);
}
KData KPort::data(int idx) const
{
    return dptr->data(idx);
}


bool KPort::isConnected() const
{
    return !dptr->_conPorts.isEmpty();
}

void KPort::connectTo(KPort *dest, KConnector *con)
{
    Q_ASSERT(dest);
    Q_ASSERT(con);
    Q_ASSERT(con->isConnected());

    if (con->oppositePort(this) == dest) {
        dptr->append(dest, con);
        dest->dptr->append(this, con);
    }
}
void KPort::disconnectFrom(KPort * dest, KConnector * con)
{
    Q_ASSERT(dest);
    Q_ASSERT(con);
    Q_ASSERT(con->isConnected());
    if (con->oppositePort(this) == dest) {
        dptr->remove(dest, con);
        dest->dptr->remove(this, con);
    }
}

void KPort::removeConnections()
{
    dptr->removeConnections(this);
}

bool KPort::canConnect(KPort * port) const
{
    //TODO
    bool tInp = (this->direction() & KPort::Input) == KPort::Input;
    bool tOut = (this->direction() & KPort::Output) == KPort::Output;
    bool oInp = (port->direction() & KPort::Input) == KPort::Input;
    bool oOut = (port->direction() & KPort::Output) == KPort::Output;

    //if direction is OK, then check type
    //this: input -> other: output, this: output -> other: input
    if ((tInp && oOut) || (tOut && oInp)) {
        //return (port->quantity() == this->quantity()) != 0;
        //TODO
        //validate quantity
        return true;
    }
    else {
        return false;
    }
}
QRectF KPort::boundingRect () const
{
    return QRectF(-5, -4, 10, 8);
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

    QFontMetrics fm = painter->fontMetrics();
    float th = fm.height();
    float dh = th - rect.height();
    if (dh > 0)
        rect.adjust(0, -dh/2, 0, dh/2);

    if (xHas(this->direction(), KPort::Input)) {
        painter->drawText(rect.right()+2, rect.top(), w, rect.height(),
                          Qt::AlignLeft | Qt::AlignVCenter, quantity()->symbol);
    }
    else {
        painter->drawText(rect.x() - w, rect.top(), w-2, rect.height(),
                          Qt::AlignRight | Qt::AlignVCenter, quantity()->symbol);
    }

    painter->restore();
}
void KPort::rearrangeConnectors(const QPointF& oldPos, const QPointF& newPos)
{
    dptr->rearrangeConnectors(this, oldPos, newPos);
}

int KPort::type() const
{
    return Type;
}


