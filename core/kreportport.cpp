#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "kreportport.h"
#include "imodel.h"

SMargin KReportPort::Margin = {0, 0, 20, 0};
static KReport __defaultReport;

KReportPort::KReportPort(IModel *parent) : QGraphicsItem(parent), _report(0)
{
    setFlags(QGraphicsItem::ItemIsSelectable);
}
KReportPort::~KReportPort()
{
}
int KReportPort::type() const
{
    return Type;
}
QRectF KReportPort::boundingRect () const
{
    return QRectF(-12, -10, 24, 20);
}
void KReportPort::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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

    qreal w = 200;
    if (hasReport()) {
        //draw report
        painter->setBrush(Qt::lightGray);
        painter->drawEllipse(cRect);
        painter->setPen(QPen(Qt::darkGray, 1));
        QString txt = report()->name();
        painter->drawText(Margin.right-w, rect.bottom(), w, rect.height(),
            Qt::AlignRight|Qt::AlignTop, txt);
    }
    // (report not yet selected)
    else {
        //draw information
        painter->setBrush(Qt::yellow);
         painter->drawEllipse(cRect);
        painter->setPen(Qt::darkRed);
        QString txt = QObject::tr("[None]");
        painter->drawText(Margin.right-w, rect.bottom(), w, rect.height(),
            Qt::AlignRight|Qt::AlignTop, txt);
    }
    painter->restore();
}

void KReportPort::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    promptReport();
    event->accept();
}

KReport * KReportPort::report() const
{
    return _report;
}
void KReportPort::setReport(KReport * rep)
{
    Q_ASSERT(this->parentItem() != 0);

    _report = rep;
    setToolTip("[TODO: report]");
    this->parentItem()->update();
}

bool KReportPort::hasReport() const
{
    return _report != 0;
}
void KReportPort::promptReport()
{
    //TODO
    setReport(&__defaultReport);
}
