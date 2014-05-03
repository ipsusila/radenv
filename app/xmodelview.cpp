#include <QtDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QGraphicsItem>
#include "xmodelview.h"
#include "imodel.h"
#include "kconnector.h"

XModelView::XModelView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene,parent)
{
    _zoomFactor = 1.0;
    scene->setParent(this);
    this->setFont(QFont("monospace", 10));
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setRenderHint(QPainter::TextAntialiasing, true);
}

void XModelView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
#if defined(Q_OS_SYMBIAN)
    fitInView(sceneRect(), Qt::KeepAspectRatio);
#endif
}

void XModelView::drawBackground(QPainter * painter, const QRectF & rect)
{
    QGraphicsView::drawBackground(painter, rect);

    //add grid
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc != 0);
    if (mc->displayGrid()) {
        QRectF arect = mc->sceneRect();
        int grid = mc->grid();

        //major and minor grid pen
        QPen minPen(QColor(230,230,230), 1, Qt::DotLine);
        QPen majPen(QColor(230,230,230), 1);

        //starting position, normalized to grid size
        int x = grid * ((int)arect.x() / grid);
        int y = grid * ((int)arect.y() / grid);
        int r = grid * ((int)arect.right() / grid);
        int b = grid * ((int)arect.bottom() / grid);

        //draw vertical line
        int dg = grid * 5;
        painter->setPen(minPen);
        while (x < r) {
            if (x % dg == 0) {
                painter->setPen(majPen);
                painter->drawLine(x, arect.y(), x, arect.bottom());
                painter->setPen(minPen);
            }
            else {
                painter->drawLine(x, arect.y(), x, arect.bottom());
            }

            //next grid line
            x += grid;
        }

        //draw horizontal line
        painter->setPen(minPen);
        while (y < b) {
            if (y % dg == 0) {
                painter->setPen(majPen);
                painter->drawLine(arect.x(), y, arect.right(), y);
                painter->setPen(minPen);
            }
            else {
                painter->drawLine(arect.x(), y, arect.right(), y);
            }

            //next grid line
            y += grid;
        }
    }

    //draw the frame
    painter->setPen(QPen(Qt::darkGray, 1));
    painter->drawRect(mc->sceneRect());
}
void XModelView::mousePressEvent(QMouseEvent * event)
{
    //handle event
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc != 0);

    if (mc->editMode() == KModelScene::Connect && event->button() == Qt::RightButton) {
        mc->cancelConnection();
        event->accept();
        return;
    }

    if (mc == 0 || event->button() != Qt::LeftButton || mc->editMode() == KModelScene::None) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    QPointF pos = this->mapToScene(event->pos());
    QGraphicsItem * item = mc->itemAt(pos, this->transform());
    switch(mc->editMode()) {
    case KModelScene::Connect:
        mc->connectItem(item, pos);
        event->accept();
        break;
    case KModelScene::RemoveConnection:
        removeConnection(item);
        event->accept();
        break;
    case KModelScene::RemoveModel:
        removeItem(item);
        event->accept();
        break;
    default:
        QGraphicsView::mousePressEvent(event);
        break;
    }
}
void XModelView::mouseMoveEvent(QMouseEvent * event)
{
    //qDebug() << "Mouse move event " << event->pos();
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc != 0);
    if (mc->editMode() == KModelScene::Connect) {
        mc->tryDrawConnector(mapToScene(event->pos()));
    }
    QGraphicsView::mouseMoveEvent(event);
}
void XModelView::setEditMode(KModelScene::EditMode mode)
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc != 0);
    KModelScene::EditMode oldMode = mc->editMode();
    if (oldMode == mode)
        return;

    if (oldMode == KModelScene::Connect)
        setMouseTracking(false);
    else if (mode == KModelScene::Connect)
        setMouseTracking(true);
    mc->setEditMode(mode);
}

void XModelView::removeItem(QGraphicsItem *item)
{
    if (item == 0)
        return;

    //get clicked model
    IModel * model = 0;
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (item->parentItem() == 0)
        model = qgraphicsitem_cast<IModel *>(item);
    else
        model = qgraphicsitem_cast<IModel *>(item->parentItem());

    //remove clicked model
    if (model != 0) {
        int ret = QMessageBox::question(this, tr("Remove model?"),
            QString(tr("Do you really want to remove model %1?")).arg(model->tagName()),
            QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes)
            mc->removeModel(model, true);
    }
}

void XModelView::removeConnection(QGraphicsItem * selItem)
{
    if (selItem == 0)
        return;

    //is this connector? if yes, remove connection
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    KConnector * con = qgraphicsitem_cast<KConnector *>(selItem);
    if (con != 0) {
        mc->removeConnector(con);
    }
}

void XModelView::report()
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc);

    mc->generateReport();
}

void XModelView::snapToGrid(bool v)
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    mc->setSnapToGrid(v);
    mc->update();
}

void XModelView::displayGrid(bool v)
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    mc->setDisplayGrid(v);
    mc->update();
}
void XModelView::zoomFit()
{
    this->fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    //_zoomFactor = 1.0;
}
void XModelView::zoomIn()
{
    //_zoomFactor *= 1.2;
    scale(1.2, 1.2);
}
void XModelView::zoomOut()
{
    //_zoomFactor /= 1.2;
    scale(1.0/1.2, 1.0/1.2);
}
void XModelView::zoomOriginal()
{
    //scale(1.0/_zoomFactor, 1.0/_zoomFactor);
    //_zoomFactor = 1.0;
    this->resetTransform();
}

void XModelView::clearModel()
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc != 0);
    if (mc->items().isEmpty())
        return;

    //ask
    int ret = QMessageBox::question(this, tr("Clear model(s)?"),
        tr("Remove all models from scene?"), QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::Yes)
        mc->clearModels();
}

void XModelView::reannotateModel()
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    Q_ASSERT(mc != 0);
    mc->reannotateModels();
}

bool XModelView::isDisplayGrid() const
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (mc != 0)
        return mc->displayGrid();
    return false;
}
bool XModelView::isSnapToGrid() const
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (mc != 0)
        return mc->snapToGrid();
    return false;
}
void XModelView::createModel(IModelFactory * factory, const KModelInfo & info)
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (mc != 0)
        mc->createModel(factory, info);
}

void XModelView::refreshModels()
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (mc != 0)
        mc->refresh();
}

void XModelView::verifyModels()
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (mc != 0)
        mc->verify();
}

void XModelView::calculate()
{
    KModelScene * mc = reinterpret_cast<KModelScene *>(scene());
    if (mc != 0)
        mc->calculate();
}
