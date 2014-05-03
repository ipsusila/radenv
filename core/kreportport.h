#ifndef KREPORTPORT_H
#define KREPORTPORT_H

#include <QGraphicsItem>
#include "kreport.h"

class K_CORE_EXPORT KReportPort : public QGraphicsItem
{
public:
    enum { Type = UserType + TYPE_REPORT_PORT};
    static SMargin Margin;

    explicit KReportPort(IModel *p = 0);
    virtual ~KReportPort();

    int type() const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event);

    KReport * report() const;
    void setReport(KReport * rep);
    bool hasReport() const;
    void promptReport();
    void refresh();
private:
    KReport * _report;
};

#endif // KREPORTPORT_H
