#include <QDebug>
#include <QSharedData>
#include <QGraphicsScene>
#include <QPainter>
#include "kport.h"
#include "kconnector.h"

class KConnectorPrivate : public QSharedData {
    KConnector * connector;
public:
    KPort * input;
    KPort * output;
    QPointF tempPoint;
    QVector<QPointF> points;
    QRectF rect;

    KConnectorPrivate(KConnector * it) : connector(it), input(0), output(0), tempPoint(QPointF()) {

    }
    inline bool canConnect(KPort * p) const {
        if (p == 0 || isConnected())
            return false;
        else if (input != 0)
            return input->canConnect(p);
        else if (output != 0)
            return output->canConnect(p);
        return true;
    }

    void setPort(KPort * p) {
        if (p == 0 || !canConnect(p))
            return;

        //assign port
        bool modified = false;
        if ((p->direction() & KPort::Input) == KPort::Input) {
            if (input == 0) {
                input = p;
                modified = true;
            }
        }
        else if ((p->direction() & KPort::Output) == KPort::Output) {
            if (output == 0) {
                output = p;
                modified = true;
            }
        }

        //check if modified
        if (modified) {
            if (points.isEmpty()) {
                //first time connected
                points.append(QPointF(0,0));
                connector->setPos(p->scenePos());
                connector->setZValue(p->zValue()-1);
            }
            else if (isConnected()) {
                //connection closed
                QPointF scPt = p->scenePos();
                addPoint(scPt);

                //now connect
                this->connect();
            }
        }
    }
    inline void addPoint(const QPointF & pt) {
        QPointF itemPt = connector->mapFromScene(pt);

        rect = rectWith(itemPt);
        points.append(itemPt);
        tempPoint = QPointF();
    }
    QRectF rectWith(QPointF & pt) {
        qreal lx = pt.x();
        qreal rx = pt.x();
        qreal ty = pt.y();
        qreal by = pt.y();

        foreach(QPointF p, points) {
            if (p.x() < lx)
                lx = p.x();
            if (p.x() > rx)
                rx = p.x();
            if (p.y() < ty)
                ty = p.y();
            if (p.y() > by)
                by = p.y();
        }
        //TODO
        //must be specified as pen width
        //adjustment for selection
        return QRectF(lx+1, ty+1, rx-lx-1, by-ty-1);
    }
    inline void setTempPoint(const QPointF& pt) {
        QPointF itemPt = connector->mapFromScene(pt);
        tempPoint = itemPt;
        rect = rectWith(itemPt);
    }

    inline bool isEmpty() const {
        return points.isEmpty();
    }
    inline bool isConnected() const {
        return input != 0 && output != 0;
    }
    inline void disconnect() {
        if (isConnected()) {
            input->disconnectFrom(output, connector);
        }
        input = 0;
        output = 0;
    }
    inline void connect() {
        if (isConnected()) {
            input->connectTo(output, connector);
        }
    }
};

KConnector::KConnector() : data(new KConnectorPrivate(this))
{
}

KConnector::KConnector(const KConnector &rhs)
    : QGraphicsItem(rhs.parentItem()), data(rhs.data)
{
}

KConnector &KConnector::operator=(const KConnector &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
        this->setParentItem(rhs.parentItem());
    }
    return *this;
}

KConnector::~KConnector()
{
    qDebug() << "Connector removed";
}
void KConnector::disconnect()
{
    data->disconnect();
}
void KConnector::connect()
{
    data->connect();
}

void KConnector::addPoint(const QPointF &pt)
{
    //prepare geometri change
    prepareGeometryChange();
    data->addPoint(pt);
}
void KConnector::setTemporaryPoint(const QPointF& pt)
{
    prepareGeometryChange();
    data->setTempPoint(pt);
}

void KConnector::setPort(KPort * p)
{
    prepareGeometryChange();
    data->setPort(p);
}

KPort * KConnector::inputPort() const
{
    return data->output;
}
KPort * KConnector::outputPort() const
{
    return data->output;
}
KPort * KConnector::oppositePort(KPort *port) const
{
    if (data->input == port)
        return data->output;
    else if (data->output == port)
        return data->input;
    return 0;
}

bool KConnector::isConnected() const
{
    return data->isConnected();
}

int KConnector::type() const
{
    return Type;
}
QRectF KConnector::boundingRect () const
{
    return data->rect;
}
void KConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (data->isEmpty())
        return;

    painter->save();
    painter->setPen(QPen(Qt::red));
    //painter->setRenderHint(QPainter::Antialiasing, false);

    QPointF pt2;
    QPointF pt1 = data->points.first();
    for(int k = 1; k < data->points.size(); k++) {
        pt2 = data->points.at(k);
        painter->drawLine(pt1, pt2);
        pt1 = pt2;
    }

    //has temporary
    if (!data->tempPoint.isNull()) {
        painter->drawLine(pt1, data->tempPoint);
    }

//    QRectF r = data->rect.adjusted(0, 0, -1, -1);
//    painter->drawRect(r);
    /*
     *static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
    {
        // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
        // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
        const qreal penWidthZero = qreal(0.00000001);

        if (path == QPainterPath())
            return path;
        QPainterPathStroker ps;
        ps.setCapStyle(pen.capStyle());
        if (pen.widthF() <= 0.0)
            ps.setWidth(penWidthZero);
        else
            ps.setWidth(pen.widthF());
        ps.setJoinStyle(pen.joinStyle());
        ps.setMiterLimit(pen.miterLimit());
        QPainterPath p = ps.createStroke(path);
        p.addPath(path);
        return p;
    }
    */

    /*
     *QPainterPath QGraphicsLineItem::shape() const
        {
            Q_D(const QGraphicsLineItem);
            QPainterPath path;
            if (d->line == QLineF())
                return path;

            path.moveTo(d->line.p1());
            path.lineTo(d->line.p2());
            return qt_graphicsItem_shapeFromPath(path, d->pen);
        }
        */

    painter->restore();
}
