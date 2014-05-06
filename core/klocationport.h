#ifndef KLOCATIONPORT_H
#define KLOCATIONPORT_H

#include <QGraphicsItem>
#include "klocation.h"

class RADENV_API KLocationPort : public QGraphicsItem
{
public:
    enum { Type = UserType + TYPE_LOCATION_PORT};
    static SMargin Margin;

    explicit KLocationPort(IModel * p = 0);
    virtual ~KLocationPort();

    int type() const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event);

    KLocation location() const;
    void setLocation(const KLocation& loc);
    bool hasLocation() const;
    void promptLocation();
    void refresh();
private:
    KLocation _location;
};

#endif // KLOCATIONPORT_H
