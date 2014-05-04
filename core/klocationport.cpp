#include <QtDebug>
#include <QPainter>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include "klocationport.h"
#include "imodel.h"
#include "dialoglocation.h"
#include "kstorage.h"
#include "imodelfactory.h"

SMargin KLocationPort::Margin = {20, 0, 0, 0};

KLocationPort::KLocationPort(IModel * p) : QGraphicsItem(p)
{
    setFlags(QGraphicsItem::ItemIsSelectable);
}
KLocationPort::~KLocationPort()
{
    qDebug() << "Location Port Destroyed";
}

int KLocationPort::type() const
{
    return Type;
}
QRectF KLocationPort::boundingRect () const
{
    return QRectF(-9, -9, 18, 18);
}
void KLocationPort::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect = boundingRect();
    QRectF cRect = rect.adjusted(2,2,-2,-2);

    painter->save();
    if (this->isSelected())
        painter->setPen(QPen(Qt::red, 1, Qt::DotLine));
    else
        painter->setPen(QPen(Qt::darkGray, 1));

    if (hasLocation()) {
        //draw location
        painter->setBrush(Qt::lightGray);
        painter->drawEllipse(cRect);
        painter->setPen(QPen(Qt::darkGray, 1));
        QString txt = location().name();
        painter->drawText(-Margin.left, rect.y(), txt);
    }
    // (location not yet selected)
    else {
        //draw information
        painter->setBrush(QColor(200,0,0));
        painter->drawEllipse(cRect);
        painter->setPen(Qt::darkRed);
        QString txt = QObject::tr("<Undefined>");
        painter->drawText(-Margin.left, rect.y(), txt);
    }
    painter->restore();
}

void KLocationPort::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    promptLocation();
    event->accept();
}

KLocation KLocationPort::location() const
{
    return _location;
}
void KLocationPort::setLocation(const KLocation& loc)
{
    Q_ASSERT(this->parentItem() != 0);
    _location = loc;
    this->setToolTip(loc.detailDisplayText());
    this->parentItem()->update();
}

bool KLocationPort::hasLocation() const
{
    return _location.isValid();
}

void KLocationPort::promptLocation()
{
    IModel * md = qgraphicsitem_cast<IModel*>(this->parentItem());
    if (md != 0) {
        DialogLocation dlg(md->factory()->storage());
        if (dlg.exec() == QDialog::Accepted) {
            this->setLocation(dlg.selectedLocation());
        }
    }
}
void KLocationPort::refresh()
{
    //reassign location
    IModel * md = qgraphicsitem_cast<IModel*>(this->parentItem());
    if (md != 0) {
        qDebug() << "Location port: " << md->tagName();
        _location = md->factory()->storage()->location(_location.code());
    }
}
