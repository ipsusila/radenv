#ifndef XMODELVIEW_H
#define XMODELVIEW_H

#include <QGraphicsView>
#include "kscenario.h"

class XModelView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit XModelView(QWidget * parent = 0);
    void setEditMode(KScenario::EditMode mode);

    bool isDisplayGrid() const;
    bool isSnapToGrid() const;

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void drawBackground(QPainter * painter, const QRectF & rect);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);

    void removeItem(QGraphicsItem * item);
    void removeConnection(QGraphicsItem * selItem);

signals:
    
public slots:
    void snapToGrid(bool v);
    void displayGrid(bool v);
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomOriginal();

    void detachScenario(KScenario * scenario);
    void attachScenario(KScenario * scenario);

private:
    bool _isFitInView;
    
};

#endif // XMODELVIEW_H
