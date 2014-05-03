#ifndef XMODELVIEW_H
#define XMODELVIEW_H

#include <QGraphicsView>
#include "kmodelscene.h"

class XModelView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit XModelView(QGraphicsScene * scene, QWidget * parent = 0);
    void setEditMode(KModelScene::EditMode mode);

    bool isDisplayGrid() const;
    bool isSnapToGrid() const;
    void createModel(IModelFactory * factory, const KModelInfo & info);
    void refreshModels();
    void verifyModels();
    void calculate();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void drawBackground(QPainter * painter, const QRectF & rect);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);

    void removeItem(QGraphicsItem * item);
    void removeConnection(QGraphicsItem * selItem);

signals:
    
public slots:
    void clearModel();
    void reannotateModel();
    void report();

    void snapToGrid(bool v);
    void displayGrid(bool v);
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomOriginal();

private:
    qreal _zoomFactor;
    
};

#endif // XMODELVIEW_H
