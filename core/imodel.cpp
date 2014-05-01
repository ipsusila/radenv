#include <QtDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLayout>
#include <QMessageBox>
#include "dialoguserinput.h"
#include "kport.h"
#include "imodel.h"
#include "klocationport.h"
#include "kmodelscene.h"
#include "koutput.h"
#include "uiuserinput.h"
#include "ksettingmanager.h"
#include "kconnector.h"

/**
 * @brief Empty Ports list.
 */
static const KPortList EmptyPorts;
SPadding IModel::Padding = {10, 25, 10, 26};

class IModelPrivate : public QSharedData
{
public:
    int _tagId;
    KLocationPort * _locPort;
    KReportPort * _repPort;
    KModelInfo _info;
    int _sourceDistance;
    IModel * _visitor;
    IModelFactory * _factory;

    IModelPrivate(IModelFactory * fact) : _tagId(0), _locPort(0), _repPort(0),
          _sourceDistance(0), _visitor(0), _factory(fact)
    {
    }

    inline void setReport(KReport * rep)
    {
        if (_repPort != 0)
            _repPort->setReport(rep);
    }

    inline void setLocation(const KLocation& loc)
    {
        if (_locPort != 0)
            _locPort->setLocation(loc);
    }
    inline KLocation location() const
    {
        if (_locPort != 0)
            return _locPort->location();
        return KLocation();
    }

    inline KReport * report() const
    {
        if (_repPort == 0)
            return 0;

        return _repPort->report();
    }

    void visit(IModel * self, IModel * visitor)
    {
        if (visitor == 0) {
            //this node must be source
            if (!self->isSource()) {
                xWarning() << *self << QObject::tr(" is not source but visited by NULL node. Set distance to 0.");
            }
            _sourceDistance = 0;
        }
        else if (visitor == _visitor) {
            //check for loop
            if (_sourceDistance < visitor->sourceDistance()) {
                xWarning() << *self << QObject::tr(" and ") << visitor->tagName()
                           << QObject::tr(" are connected as loop. Could not be evaluated properly.");
            }
        }
        else {
            //update distance if equal or greater than the latest distance
            if (_sourceDistance <= visitor->sourceDistance()) {
                _sourceDistance = visitor->sourceDistance() + 1;
                _visitor = visitor;
            }
        }

        //display in output window
        xInfo() << QObject::tr("Visiting ") << *self << QObject::tr(" by ")
                << (visitor == 0 ? "NULL" : visitor->tagName())
                << QObject::tr(", source distance is ") << _sourceDistance;

        //visit all nodes that is connected to this model
        const KPortList& outs = self->outputs();
        foreach(KPort * p, outs) {
            const KPortList& conPorts = p->connectedPorts();
            foreach(KPort * cp, conPorts) {
                IModel * md = cp->model();
                md->visit(self);
            }
        }
    }
};

IModel::IModel(IModelFactory * fact, const KModelInfo& inf) : dptr(new IModelPrivate(fact))
{
    this->setZValue(1);
    this->setInfo(inf);
    this->setAcceptHoverEvents(true);
    //set flags
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
}

IModel::~IModel()
{
    qDebug() << "Model destroyed";
}

void IModel::removeConnections()
{
    const KPortList& inpList = inputs();
    foreach(KPort * p, inpList)
        p->removeConnections();

    const KPortList& outList = outputs();
    foreach(KPort * p, outList)
        p->removeConnections();
}

int IModel::sourceDistance() const
{
    return dptr->_sourceDistance;
}
void IModel::visit(IModel * visitor)
{
    dptr->visit(this, visitor);
}

void IModel::copyDataTo(IModel * model) const
{
    Q_ASSERT(model->info().serialId() == this->info().serialId());
    Q_UNUSED(model);
}

IModel * IModel::copyTo(KModelScene * mscene) const
{
    // for visitors (will bi assigned by scene)
    IModel * model = mscene->createModel(factory(), info());
    if (model) {
        //assign position
        model->setPos(this->pos());
        model->setZValue(this->zValue());
        model->setLocation(location());
        model->setReport(report());
        copyDataTo(model);
    }

    return model;
}

const IModel * IModel::latestVisitor() const
{
    return dptr->_visitor;
}

void IModel::defineParameters()
{
}

bool IModel::allocateIoPorts()
{
    return true;
}

bool IModel::allocateReportPorts()
{
    if (needReport()) {
        dptr->_repPort = new KReportPort(this);
        dptr->_repPort->setZValue(zValue()+3);
    }
    return true;
}

bool IModel::allocateLocationPorts()
{
    //check wether this model need location
    if (needLocation()) {
        dptr->_locPort = new KLocationPort(this);
        dptr->_locPort->setZValue(zValue()+2);
    }
    return true;
}

void IModel::arrangePorts()
{
    //lokasi
    QRectF rect = modelRect();
    if (dptr->_locPort) {
        dptr->_locPort->setPos(rect.x() + KLocationPort::Margin.left, rect.y());
    }

    //report position
    if (dptr->_repPort) {
        dptr->_repPort->setPos(rect.right()-KReportPort::Margin.right, rect.bottom());
    }

    //input port position
    int index;
    float dh, ph, base;
    const KPortList inp = inputs();
    int np = inp.size();
    if (np > 0) {
        ph = inp.first()->boundingRect().height();  //port height
        base = rect.y() - ph/2;                     //base position
        dh = ph + (rect.height() - np*ph) / (np+1); //distance between port

        index = 0;
        foreach(KPort * p, inp) {
            base += dh;
            p->setPos(rect.x(), base);
            p->setIndex(index++);
        }
    }

    //output port position
    const KPortList out = outputs();
    np = out.size();
    if (np > 0) {
        ph = out.first()->boundingRect().height();
        base = rect.y() - ph/2;
        dh = ph + (rect.height() - np*ph) / (np+1);

        index = 0;
        foreach(KPort * p, out) {
            base += dh;
            p->setPos(rect.right(), base);
            p->setIndex(index++);
        }
    }
}

bool IModel::initialize()
{
    //allocate ports
    bool ok = allocateIoPorts();

    //allocate location ports
    ok = allocateLocationPorts() && ok;

    //allocate report ports
    ok = allocateReportPorts() && ok;

    //arrange ports
    arrangePorts();

    //define user inputs
    defineParameters();

    return ok;
}

void IModel::userInputsFinished(bool accepted)
{
    Q_UNUSED(accepted);
}

bool IModel::promptUserInputs()
{
    bool accepted = false;

    //create default user input dialogs
    IUserInput * wUserInp = createUserInputWidget();
    if (wUserInp != 0) {
        if (wUserInp->isEmpty()) {
            //QMessageBox::information (
            //  QWidget * parent,
            //  const QString & title,
            //  const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton )
            QMessageBox::information(0, QObject::tr("User parameters"),
                                     QObject::tr("User input parameters not defined!"));
        }
        else {
            QDialog * dlg = new DialogUserInput(this, wUserInp);
            if (dlg->exec() == QDialog::Accepted) {
                wUserInp->acceptValues();
                accepted = true;
            }
            delete dlg;
        }
    }

    return accepted;
}

void IModel::askUserParameters()
{
    bool accepted = promptUserInputs();
    userInputsFinished(accepted);
}
void IModel::refresh()
{
    KLocationPort * lp = locationPort();
    if (lp)
        lp->refresh();
}
KData IModel::data(const Quantity & qty) const
{
    //get from this model
    KData d = modelData(qty);
    if (d.isValid())
        return d;

    //if data not found,
    //ask from connected model
    return inputs().data(qty);
}
void IModel::connectionModified(KPort * port, KConnector * con, bool connected)
{
    Q_UNUSED(port);
    Q_UNUSED(con);
    Q_UNUSED(connected);
}

void IModel::generateReport()
{
    KReport * rep = report();
    if (rep == 0)
        return;

    rep->addModel(this);
    KLocationPort * lp = locationPort();
    if (lp) {
        rep->addLocation(lp->location());
    }

    KDataTable table;
    KDataGroupArray * userInp = this->userInputs();
    if (userInp != 0) {
        KDataGroupArray dga;
        userInp->separateTo(&dga, &table);
        rep->addUserInputs(dga);
    }

    KDataArray da;
    result().separateTo(&da, &table);
    rep->addResult(da);

    table.transpose();
    rep->addResult(table);
}

const KPortList & IModel::inputs() const
{
    return EmptyPorts;
}
const KPortList & IModel::outputs() const
{
    return EmptyPorts;
}
QString IModel::displayText() const
{
    return "";
}

IModelFactory * IModel::factory() const
{
    return dptr->_factory;
}

bool IModel::isSource() const
{
    return inputs().isEmpty();
}
bool IModel::isSink() const
{
    return outputs().isEmpty();
}

int IModel::tagId() const
{
    return dptr->_tagId;
}
void IModel::setTagId(int id)
{
    int oldId = dptr->_tagId;
    if (oldId != id)
        tagIdChange(oldId, id);
    dptr->_tagId = id;
    if (oldId != id)
        tagIdChanged(oldId, id);
}
void IModel::tagIdChange(int oldId, int newId)
{
    Q_UNUSED(oldId);
    Q_UNUSED(newId);

    //disconnect
    notifyConnectionsChanged(false);
}

void IModel::tagIdChanged(int oldId, int newId)
{
    Q_UNUSED(oldId);
    Q_UNUSED(newId);

    //reconnect
    notifyConnectionsChanged(true);
}

void IModel::notifyConnectionsChanged(bool connected)
{
    //notify connected models
    //remove connection or reconnect with this model
    const KPortList & outs = outputs();
    foreach(KPort * p, outs) {
        ConnectorList connectors = p->connectors();
        foreach(KConnector * con, connectors) {
            KPort * inp = con->inputPort();
            IModel * model = inp->model();
            if (model != 0)
                model->connectionModified(inp, con, connected);
        }
    }
}

QString IModel::tagName(const QString & post) const
{
    return info().name() + QString::number(tagId()) + post;
}
KModelInfo IModel::info() const
{
    return dptr->_info;
}
void IModel::setInfo(const KModelInfo& i)
{
    dptr->_info = i;
    this->setToolTip(i.text()+"\n"+i.description());
}
DataArrayList IModel::inputResults() const
{
    DataArrayList resultList;
    const KPortList& inpList = inputs();
    foreach(KPort * p, inpList)
        resultList << p->data();
    return resultList;
}
ModelList IModel::connectedOutputModels() const
{
    ModelList list;
    const KPortList & outs = outputs();
    foreach(KPort * p, outs) {
        ConnectorList connectors = p->connectors();
        foreach(KConnector * con, connectors) {
            KPort * inp = con->inputPort();
            if (inp->model() != 0)
                list.append(inp->model());
        }
    }

    return list;
}
ConnectorList IModel::outputConnectors() const
{
    ConnectorList connectors;
    const KPortList & outs = outputs();
    foreach(KPort * p, outs) {
        connectors.append(p->connectors());
    }
    return connectors;
}

bool IModel::needLocation() const
{
    return true;
}
bool IModel::needReport() const
{
    return true;
}

KReportPort * IModel::reportPort() const
{
    return dptr->_repPort;
}

KReport * IModel::report() const
{
    KReportPort * repPort = reportPort();
    if (repPort == 0)
        return 0;

    return repPort->report();
}
void IModel::setReport(KReport * rep)
{
    dptr->setReport(rep);
}

KLocationPort * IModel::locationPort() const
{
    return dptr->_locPort;
}
void IModel::setLocationPort(KLocationPort *port)
{
    dptr->_locPort = port;
}
void IModel::setLocation(const KLocation & loc)
{
    dptr->setLocation(loc);
}
KLocation IModel::location() const
{
    return dptr->location();
}
KDataGroupArray * IModel::userInputs()
{
    return 0;
}

IUserInput * IModel::createUserInputWidget(QWidget * parent)
{
    //generate widget
    KDataGroupArray * ga = userInputs();
    if (ga != 0)
        return new UiUserInput(ga, parent);
    return 0;
}

QRectF IModel::modelRect() const
{
    return QRectF(-37.5f, -25, 75, 50);
}

int IModel::type() const
{
    return Type;
}

QImage * IModel::image() const
{
    return 0;
}

QRectF IModel::boundingRect () const
{
    return modelRect().adjusted(-Padding.left, -Padding.top, Padding.right, Padding.bottom);
}

void IModel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->setBrush(QColor(225,225,225));
    painter->setPen(QPen(Qt::darkGray, 1));

    int penWidth = painter->pen().width();
    QRectF rect = modelRect();

    //draw model
    QImage * img = image();
    if (img == 0 || img->isNull())
        painter->drawRect(rect);
    else
        painter->drawImage(rect, *img);

    //draw tag name
    QFont f = painter->font();
    f.setPointSize(9.5f);
    painter->setFont(f);
    painter->setBrush(Qt::darkGray);
    painter->drawText(rect.x()+penWidth+2, rect.bottom(), rect.width(), Padding.bottom/2,
        Qt::AlignLeft|Qt::AlignTop, this->tagName(""));

    //display text
    QString dispTxt = displayText();
    if (!dispTxt.isEmpty()) {
        painter->setBrush(Qt::black);
        painter->drawText(rect.translated(penWidth+2,-penWidth-2),
                          Qt::AlignLeft | Qt::AlignBottom, dispTxt);
    }

    //bounding rect
    if (this->isSelected()) {
        painter->setPen(QPen(Qt::red, 1, Qt::DotLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }

    painter->restore();
}
void IModel::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    askUserParameters();
    event->accept();
}
void IModel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "+Enter hover event : " << this->tagName();
    Q_UNUSED(event);
    setPortsVisible(true);
}

void IModel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "-Leave hover event : " << this->tagName();
    Q_UNUSED(event);
    setPortsVisible(false);
}

void IModel::setPortsVisible(bool v)
{
    const KPortList & inp = inputs();
    for(int k = 0; k < inp.count(); k++) {
        KPort * p = inp.at(k);
        bool ah = p->isAutoHide();
        p->setVisible((ah && v) || !ah);
    }

    const KPortList & out = outputs();
    for(int k = 0; k < out.count(); k++) {
        KPort * p = out.at(k);
        bool ah = p->isAutoHide();
        p->setVisible((ah && v) || !ah);
    }

    //report port
    KReportPort * repPort = reportPort();
    if (repPort != 0)
        repPort->setVisible(v);

    this->update();
}

QVariant IModel::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
        QPointF newPos = value.toPointF();
        QPointF oldPos = this->pos();

        if (mc->snapToGrid()) {
            // value is the new position.
            float grid = mc->grid();
            float dx = newPos.x() - oldPos.x();
            float dy = newPos.y() - oldPos.y();

            int nx = qRound(dx/grid);
            int ny = qRound(dy/grid);
            newPos = QPointF(oldPos.x() + nx*grid, oldPos.y() + ny*grid);

            //rearrange ports
            if (oldPos != newPos) {
                foreach(KPort * p, inputs())
                    p->rearrangeConnectors(oldPos, newPos);
                foreach(KPort * p, outputs())
                    p->rearrangeConnectors(oldPos, newPos);

                //some times, the portion of text is erased after moving item
                //schedule redraw.
                mc->update();
            }

            return newPos;
        }
        else {
            //rearrange ports
            if (oldPos != newPos) {
                foreach(KPort * p, inputs())
                    p->rearrangeConnectors(oldPos, newPos);
                foreach(KPort * p, outputs())
                    p->rearrangeConnectors(oldPos, newPos);

                //some times, the portion of text is erased after moving item
                //schedule redraw.
                mc->update();
            }
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

