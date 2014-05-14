#include <QtDebug>
#include <QSharedData>
#include <QGraphicsScene>
#include <QPainter>
#include "kport.h"
#include "kconnector.h"
#include "imodel.h"
#include "kscenario.h"

class KConnectorPrivate : public QSharedData {
    KConnector * connector;
public:
    KPort * firstPort;
    KPort * input;              //Port which input direction
    KPort * output;             //Port which has output direction
    QPointF tempPoint;
    QVector<QPointF> points;
    QRectF rect;

    KConnectorPrivate(KConnector * it) : connector(it), firstPort(0),
        input(0), output(0), tempPoint(QPointF())
    {

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
                firstPort = p;
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
                connector->setZValue(p->model()->zValue()-1);
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
    inline QDataStream & serialize(QDataStream &s) const
    {
        Q_ASSERT(input != 0);
        Q_ASSERT(output != 0);
        Q_ASSERT(firstPort != 0);

        s << input->model()->tagName() << input->index()
          << output->model()->tagName() << output->index()
          << (firstPort == input ? 1 : 2) << rect << points << connector->pos();

        return s;
    }
    inline QDataStream & deserialize(QDataStream &s)
    {
        QPointF pos;
        QString inpTagName, outTagName;
        int inpIndex, outIndex, firstPortIndex;

        s >> inpTagName >> inpIndex >> outTagName >> outIndex
          >> firstPortIndex >> rect >> points >> pos;

        //TODO
        //check for boundary conditions
        //z-value

        //search port
        KScenario * scene = reinterpret_cast<KScenario*>(connector->scene());
        Q_ASSERT(scene != 0);
        IModel * mi = scene->findModel(inpTagName);
        IModel * mo = scene->findModel(outTagName);
        Q_ASSERT(mi != 0);
        Q_ASSERT(mo != 0);
        Q_ASSERT(inpIndex >= 0 && inpIndex < mi->inputs().size());
        Q_ASSERT(outIndex >= 0 && outIndex < mo->outputs().size());

        //get port
        input = mi->inputs().at(inpIndex);
        output = mo->outputs().at(outIndex);

        if (firstPortIndex == 1)
            firstPort = input;
        else
            firstPort = output;
        connect();
        connector->setPos(pos);
        return s;
    }

    inline bool isEmpty() const {
        return points.isEmpty();
    }
    inline bool isConnected() const {
        return input != 0 && output != 0;
    }
    inline bool isValid() const {
        return isConnected() && input->canConnect(output);
    }
    inline KPort * oppositePort(KPort * port) const {
        if (input == port)
            return output;
        else if (output == port)
            return input;
        return 0;
    }
    inline void disconnect() {
        if (isConnected()) {
            input->disconnectFrom(output, connector);

            //notify model
            if (input->model())
                input->model()->connectionModified(input, connector, false);
            if (output->model())
                output->model()->connectionModified(output, connector, false);
        }
        input = 0;
        output = 0;
    }
    inline void connect() {
        if (isConnected()) {
            input->connectTo(output, connector);

            //notify model
            if (input->model())
                input->model()->connectionModified(input, connector, true);
            if (output->model())
                output->model()->connectionModified(output, connector, true);
        }
    }
    inline void copyTo(KConnectorPrivate * d, KPort * out, KPort * inp) const {
        d->input = inp;
        d->output = out;
        d->points = this->points;
        d->rect = this->rect;
    }
    void recalculateRect()
    {
        if (!points.isEmpty()) {
            rect = rectWith(points.first());
        }
    }
    void moveByPort(KPort * p, const QPointF & oldPos, const QPointF & newPos)
    {
        if (p == firstPort) {
            QPointF & pt = points.first();
            pt.setX(pt.x() + newPos.x() - oldPos.x());
            pt.setY(pt.y() + newPos.y() - oldPos.y());
        }
        else {
            QPointF & pt = points.last();
            pt.setX(pt.x() + newPos.x() - oldPos.x());
            pt.setY(pt.y() + newPos.y() - oldPos.y());
        }
        recalculateRect();
    }
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        if (isEmpty())
            return;

        painter->save();
        painter->setPen(QPen(Qt::darkGreen));
        //painter->setRenderHint(QPainter::Antialiasing, false);

        QPointF pt2;
        QPointF pt1 = points.first();
        for(int k = 1; k < points.size(); k++) {
            pt2 = points.at(k);
            painter->drawLine(pt1, pt2);
            pt1 = pt2;
        }

        //has temporary
        if (!tempPoint.isNull()) {
            painter->drawLine(pt1, tempPoint);
            pt1 = tempPoint;
        }

        //invalid
        if (isConnected() && !isValid()) {
            painter->setPen(Qt::red);
            QRect r(pt1.x()-3, pt1.y()-3, 6, 6);
            painter->drawLine(r.topLeft(), r.bottomRight());
            painter->drawLine(r.bottomLeft(), r.topRight());

            r.adjust(2, 2, -2, -2);
            QPainterPath pp;
            pp.addEllipse(r);
            painter->fillPath(pp, QBrush(Qt::red));
        }

        painter->restore();
    }
};

KConnector::KConnector() : dptr(new KConnectorPrivate(this))
{
}

KConnector::KConnector(const KConnector &rhs)
    : QGraphicsItem(rhs.parentItem()), dptr(rhs.dptr)
{
}

KConnector &KConnector::operator=(const KConnector &rhs)
{
    if (this != &rhs) {
        dptr.operator=(rhs.dptr);
        this->setParentItem(rhs.parentItem());
    }
    return *this;
}

KConnector::~KConnector()
{
    qDebug() << "Connector removed";
}
void KConnector::copyTo(KConnector * con, KPort * out, KPort * inp) const
{
    dptr->copyTo(con->dptr, out, inp);
}
void KConnector::disconnect()
{
    dptr->disconnect();
}
void KConnector::connect()
{
    dptr->connect();
}

void KConnector::addPoint(const QPointF &pt)
{
    //prepare geometri change
    prepareGeometryChange();
    dptr->addPoint(pt);
}
void KConnector::setTemporaryPoint(const QPointF& pt)
{
    prepareGeometryChange();
    dptr->setTempPoint(pt);
}

void KConnector::setPort(KPort * p)
{
    prepareGeometryChange();
    dptr->setPort(p);
}

KPort * KConnector::inputPort() const
{
    return dptr->input;
}
KPort * KConnector::outputPort() const
{
    return dptr->output;
}
KPort * KConnector::oppositePort(KPort *port) const
{
    return dptr->oppositePort(port);
}
void KConnector::movePos(KPort * p, const QPointF & oldPos, const QPointF & newPos)
{
    prepareGeometryChange();
    dptr->moveByPort(p, oldPos, newPos);
}
bool KConnector::isValid() const
{
    return dptr->isValid();
}
bool KConnector::isConnected() const
{
    return dptr->isConnected();
}

int KConnector::type() const
{
    return Type;
}
QRectF KConnector::boundingRect () const
{
    return dptr->rect;
}
void KConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    dptr->paint(painter, option, widget);
    /*
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
        pt1 = data->tempPoint;
    }
    */

    //test
    //QRectF r = data->rect.adjusted(0, 0, -1, -1);
    //painter->drawRect(r);

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

    //painter->restore();
}

QDataStream & KConnector::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return dptr->serialize(stream);
}

QDataStream & KConnector::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    prepareGeometryChange();
    return dptr->deserialize(stream);
}
