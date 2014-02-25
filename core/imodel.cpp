#include <QtDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLayout>
#include "kport.h"
#include "imodel.h"
#include "klocationport.h"
#include "kmodelscene.h"
#include "koutput.h"
#include "userinput.h"

/**
 * @brief Empty Ports list.
 */
static const PortList EmptyPorts;
SPadding IModel::Padding = {10, 25, 10, 26};

IModel::IModel(IModelFactory * fact, const KModelInfo& inf) : _tagId(0), _locPort(0), _repPort(0),
    _sourceDistance(0), _visitor(0), _factory(fact)
{
    this->setInfo(inf);
    //set flags
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
}
IModel::IModel(const IModel &model)
    : QGraphicsItem(model.parentItem()), _tagId(0), _locPort(0), _repPort(0), _sourceDistance(0), _visitor(0)
{
    this->setInfo(model._info);
    this->setFlags(model.flags());
}
IModel::~IModel()
{
    qDebug() << "Model destroyed";
}

void IModel::removeConnections()
{
    const PortList& inpList = inputs();
    foreach(KPort * p, inpList)
        p->removeConnections();

    const PortList& outList = outputs();
    foreach(KPort * p, outList)
        p->removeConnections();
}

int IModel::sourceDistance() const
{
    return _sourceDistance;
}
void IModel::visit(IModel * visitor)
{
    if (visitor == 0) {
        //this node must be source
        if (!isSource()) {
            xWarning() << tagName() << QObject::tr(" is not source but visited by NULL node. Set distance to 0.");
        }
        _sourceDistance = 0;
    }
    else if (visitor == _visitor) {
        //check for loop
        if (_sourceDistance < visitor->sourceDistance()) {
            xWarning() << tagName() << QObject::tr(" and ") << visitor->tagName()
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
    xInfo() << QObject::tr("Visiting ") << tagName() << QObject::tr(" by ")
            << (visitor == 0 ? "NULL" : visitor->tagName())
            << QObject::tr(", source distance is ") << sourceDistance();

    //visit all nodes that is connected to this model
    const PortList& outs = this->outputs();
    foreach(KPort * p, outs) {
        const PortList& conPorts = p->connectedPorts();
        foreach(KPort * cp, conPorts) {
            IModel * md = cp->model();
            md->visit(this);
        }
    }
}

IModel * IModel::copyTo(KModelScene * mscene) const
{
    /*
    int _tagId;
    KLocationPort * _locPort;
    KReportPort * _repPort;
    KModelInfo _info;
    int _sourceDistance;
    IModel * _visitor;
    IModelFactory * _factory;
    */

    // for visitors (will bi assigned by scene)
    IModel * model = mscene->createModel(_factory, _info);
    if (model) {
        //assign position
        model->setPos(this->pos());
        model->setZValue(this->zValue());

        //assign report and location
        if (model->_locPort && this->_locPort)
            model->_locPort->setLocation(this->_locPort->location());
        if (model->_repPort && this->_repPort)
            model->_repPort->setReport(this->_repPort->report());
    }

    return model;
}

const IModel * IModel::latestVisitor() const
{
    return _visitor;
}

bool IModel::allocateIoPorts()
{
    return true;
}

bool IModel::allocateReportPorts()
{
    if (needReport()) {
        _repPort = new KReportPort(this);
        _repPort->setZValue(zValue()+3);
    }
    return true;
}

bool IModel::allocateLocationPorts()
{
    //check wether this model need location
    if (needLocation()) {
        _locPort = new KLocationPort(this);
        _locPort->setZValue(zValue()+2);
    }
    return true;
}

void IModel::arrangePorts()
{
    //lokasi
    QRectF rect = modelRect();
    if (_locPort) {
        _locPort->setPos(rect.x() + KLocationPort::Margin.left, rect.y());
    }

    //report position
    if (_repPort) {
        _repPort->setPos(rect.right()-KReportPort::Margin.right, rect.bottom());
    }

    //input port position
    float dh, ph, base;
    const PortList inp = inputs();
    int np = inp.size();
    if (np > 0) {
        ph = inp.first()->boundingRect().height();
        base = rect.y() - ph/2;
        dh = ph + (rect.height() - np*ph) / (np+1);
        foreach(KPort * p, inp) {
            base += dh;
            p->setPos(rect.x(), base);
        }
    }

    //output port position
    const PortList out = outputs();
    np = out.size();
    if (np > 0) {
        ph = out.first()->boundingRect().height();
        base = rect.y() - ph/2;
        dh = ph + (rect.height() - np*ph) / (np+1);
        foreach(KPort * p, out) {
            base += dh;
            p->setPos(rect.right(), base);
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

    return ok;
}

void IModel::promptParameters()
{
    //create default user input dialogs
    IUserInput * wUserInp = createUserInputWidget();
    if (wUserInp != 0) {
        QDialog * dlg = new QDialog();
        QDialogButtonBox * bbox = new QDialogButtonBox(dlg);
        QVBoxLayout * vbox = new QVBoxLayout();

        bbox->addButton(QDialogButtonBox::Ok);
        bbox->addButton(QDialogButtonBox::Cancel);
        bbox->setCenterButtons(true);
        QObject::connect(bbox, SIGNAL(accepted()), dlg, SLOT(accept()));
        QObject::connect(bbox, SIGNAL(rejected()), dlg, SLOT(reject()));
        vbox->addWidget(wUserInp);
        vbox->addWidget(bbox);
        dlg->setLayout(vbox);
        dlg->setWindowTitle(QString("%1 - user input parameter(s)").arg(this->tagName()));
        if (dlg->exec() == QDialog::Accepted) {
            wUserInp->acceptValues();
        }

        delete dlg;
    }
}
void IModel::refresh()
{
    KLocationPort * lp = locationPort();
    if (lp)
        lp->refresh();
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

    KDataGroupArray * userInp = this->userInputs();
    if (userInp != 0)
        rep->addUserInputs(*userInp);
    rep->addResult(this->result());
}

const PortList & IModel::inputs() const
{
    return EmptyPorts;
}
const PortList & IModel::outputs() const
{
    return EmptyPorts;
}
QString IModel::displayText() const
{
    return "";
}

bool IModel::isSource() const
{
    return false;
}

int IModel::tagId() const
{
    return _tagId;
}
void IModel::setTagId(int id)
{
    _tagId = id;
}
QString IModel::tagName(const QString & post) const
{
    return info().name() + QString::number(tagId()) + post;
}
KModelInfo IModel::info() const
{
    return _info;
}
void IModel::setInfo(const KModelInfo& i)
{
    _info = i;
    this->setToolTip(i.text()+"\n"+i.description());
}
QList<KDataArray> IModel::inputResults() const
{
    QList<KDataArray> resultList;
    const PortList& inpList = inputs();
    foreach(KPort * p, inpList)
        resultList << p->result();
    return resultList;
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
    return _repPort;
}

KReport * IModel::report() const
{
    KReportPort * repPort = reportPort();
    if (repPort == 0)
        return 0;

    return repPort->report();
}

KLocationPort * IModel::locationPort() const
{
    return _locPort;
}
void IModel::setLocationPort(KLocationPort *port)
{
    _locPort = port;
}
KLocation IModel::location() const
{
    if (_locPort)
        return _locPort->location();
    return KLocation();
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
        return new UserInput(ga, parent);
    return 0;
}

QRectF IModel::modelRect() const
{
    return QRectF(-50, -30, 100, 60);
}

int IModel::type() const
{
    return Type;
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

    QRectF rect = modelRect();
    painter->drawRect(rect);

    QFont f = painter->font();
    f.setPointSize(9.5f);
    painter->setFont(f);
    painter->setBrush(Qt::darkGray);
    painter->drawText(rect.x(), rect.bottom(), rect.width(), Padding.bottom/2,
        Qt::AlignLeft|Qt::AlignTop, this->tagName(""));

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
    promptParameters();
    event->accept();
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
            foreach(KPort * p, inputs())
                p->rearrangeConnectors(oldPos, newPos);
            foreach(KPort * p, outputs())
                p->rearrangeConnectors(oldPos, newPos);

            return newPos;
        }
        else {
            //rearrange ports
            foreach(KPort * p, inputs())
                p->rearrangeConnectors(oldPos, newPos);
            foreach(KPort * p, outputs())
                p->rearrangeConnectors(oldPos, newPos);
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

