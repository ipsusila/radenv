#include <QtDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QPrinter>
#include <QPixmap>
#include "xmodelview.h"
#include "imodel.h"
#include "kconnector.h"
#include "kassessment.h"

XModelView::XModelView(QWidget * parent) :
    QGraphicsView(parent)
{
    this->_isFitInView = false;
    this->setFont(QFont("monospace", 10));
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setRenderHint(QPainter::TextAntialiasing, true);
}

void XModelView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
#if defined(Q_OS_SYMBIAN)
    fitInView(sceneRect(), Qt::KeepAspectRatio);
#else
    if (_isFitInView)
        fitInView(sceneRect(), Qt::KeepAspectRatio);
#endif

}

void XModelView::drawGrid(QPainter * painter, const QRectF & rect)
{
    //add grid add frame
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc == 0)
        return;

    if (mc->displayGrid()) {
        //QRectF rect = mc->sceneRect();
        int grid = mc->grid();

        //major and minor grid pen
        QPen minPen(QColor(230,230,230), 1, Qt::DotLine);
        QPen majPen(QColor(230,230,230), 1);
        //QPen majPen(QColor(Qt::black), 1);

        //starting position, normalized to grid size
        int x = grid * ((int)rect.x() / grid);
        int y = grid * ((int)rect.y() / grid);
        int r = grid * ((int)rect.right() / grid);
        int b = grid * ((int)rect.bottom() / grid);

        //draw vertical line
        int dg = grid * 5;
        painter->setPen(minPen);
        while (x < r) {
            if (x % dg == 0) {
                painter->setPen(majPen);
                painter->drawLine(x, rect.y(), x, rect.bottom());
                painter->setPen(minPen);
            }
            else {
                painter->drawLine(x, rect.y(), x, rect.bottom());
            }

            //next grid line
            x += grid;
        }

        //draw horizontal line
        painter->setPen(minPen);
        while (y < b) {
            if (y % dg == 0) {
                painter->setPen(majPen);
                painter->drawLine(rect.x(), y, rect.right(), y);
                painter->setPen(minPen);
            }
            else {
                painter->drawLine(rect.x(), y, rect.right(), y);
            }

            //next grid line
            y += grid;
        }

        //draw
    }

    //draw the frame
    painter->setPen(QPen(Qt::darkGray, 1));
    painter->drawRect(mc->sceneRect());

    //display name
    QString txt = mc->name() + " (" + mc->assessment()->name() + ")";
    painter->drawText(mc->sceneRect(), Qt::AlignTop | Qt::AlignLeft, txt);
}

void XModelView::drawBackground(QPainter * painter, const QRectF & rect)
{
    QGraphicsView::drawBackground(painter, rect);
    QGraphicsScene * sc = scene();
    drawGrid(painter, sc == 0 ? QRectF() : sc->sceneRect());
}
void XModelView::mousePressEvent(QMouseEvent * event)
{
    //handle event
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc == 0)
        return;

    if (mc->editMode() == KScenario::Connect && event->button() == Qt::RightButton) {
        mc->cancelConnection();
        event->accept();
        return;
    }

    if (mc == 0 || event->button() != Qt::LeftButton || mc->editMode() == KScenario::None) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    QPointF pos = this->mapToScene(event->pos());
    QGraphicsItem * item = mc->itemAt(pos, this->transform());
    switch(mc->editMode()) {
    case KScenario::Connect:
        mc->connectItem(item, pos);
        event->accept();
        break;
    case KScenario::RemoveConnection:
        removeConnection(item);
        event->accept();
        break;
    case KScenario::RemoveModel:
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
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc != 0 && mc->editMode() == KScenario::Connect) {
        mc->tryDrawConnector(mapToScene(event->pos()));
    }
    QGraphicsView::mouseMoveEvent(event);
}
void XModelView::setEditMode(KScenario::EditMode mode)
{
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc == 0)
        return;

    KScenario::EditMode oldMode = mc->editMode();
    if (oldMode == mode)
        return;

    if (oldMode == KScenario::Connect)
        setMouseTracking(false);
    else if (mode == KScenario::Connect)
        setMouseTracking(true);
    mc->setEditMode(mode);
}

void XModelView::removeItem(QGraphicsItem *item)
{
    if (item == 0)
        return;

    //get clicked model
    IModel * model = 0;
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (item->parentItem() == 0)
        model = qgraphicsitem_cast<IModel *>(item);
    else
        model = qgraphicsitem_cast<IModel *>(item->parentItem());

    //remove clicked model
    if (model != 0 && mc != 0) {
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
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    KConnector * con = qgraphicsitem_cast<KConnector *>(selItem);
    if (mc != 0 && con != 0) {
        mc->removeConnector(con);
    }
}


void XModelView::snapToGrid(bool v)
{
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc != 0) {
        mc->setSnapToGrid(v);
        mc->update();
    }
}

void XModelView::displayGrid(bool v)
{
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc != 0) {
        mc->setDisplayGrid(v);
        mc->update();
    }
}
void XModelView::zoomFit()
{
    this->fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    this->_isFitInView = true;
}
void XModelView::zoomIn()
{
    this->_isFitInView = false;
    scale(1.2, 1.2);
}
void XModelView::zoomOut()
{
    this->_isFitInView = false;
    scale(1.0/1.2, 1.0/1.2);
}
void XModelView::zoomOriginal()
{
    this->_isFitInView = false;
    this->resetTransform();
}
void XModelView::detachScenario(KScenario * scenario)
{
    if (scenario == scene()) {
        setScene(0);
        this->_isFitInView = false;
    }
}

void XModelView::attachScenario(KScenario * scenario)
{
    if (scenario != scene()) {
        setScene(scenario);
        zoomOriginal();
        ensureVisible(scenario->itemsBoundingRect());
    }
}

void XModelView::printDocument(QPrinter * printer)
{
    if (this->scene() != 0) {
        KScenario * scenario = reinterpret_cast<KScenario *>(scene());
        printer->setDocName(scenario->name());

        /*
        QRect scRect = scene()->sceneRect().toRect();
        QRect prRect = printer->pageRect();
        float sx = (float)prRect.width() / scRect.width();
        float sy = (float)prRect.height() / scRect.height();
        float ss = qMin(sx, sy);
        QRect imgRect = QRect(0, 0, scRect.width() * ss, scRect.height() * ss);
        QPixmap pixmap(imgRect.size());
        pixmap.fill();
        QPainter painter(&pixmap);
        painter.setWindow(scRect);
        imgRect.moveCenter(QPoint(0, 0));
        //painter.setViewport(imgRect);
        displayGrid(&painter, scRect);
        //painter.translate(scRect.left(), scRect.top());
        scenario->render(&painter, imgRect, scRect);

        QPainter painterPrint(printer);
        painterPrint.drawPixmap(printer->margins().left, printer->margins().top,
                                pixmap.width(), pixmap.height(), pixmap);
        pixmap.save("e:/pix.jpg");
        */

        /*
        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setWindow(scRect);
        displayGrid(&painter, scRect);
        painter.translate(scRect.left(), scRect.top());
        scenario->render(&painter, prRect, scRect, Qt::KeepAspectRatio);
        */


        //TODO
        //drawing without using pixmap
        QRect scRect = scene()->sceneRect().toRect();
        QRect prRect = printer->pageRect();

        //draw to pixmap
        QPixmap pixmap(scRect.size());
        pixmap.fill();

        //create painter
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setWindow(scRect);
        drawGrid(&painter, scRect);
        painter.translate(scRect.left(), scRect.top());
        scene()->render(&painter);

        //draw to printer
        QPixmap scPixmap = pixmap.scaled(prRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainter painterPrint(printer);
        painterPrint.drawPixmap(QPoint(0,0), scPixmap, scPixmap.rect());

    }
}

bool XModelView::isDisplayGrid() const
{
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc != 0)
        return mc->displayGrid();
    return false;
}
bool XModelView::isSnapToGrid() const
{
    KScenario * mc = reinterpret_cast<KScenario *>(scene());
    if (mc != 0)
        return mc->snapToGrid();
    return false;
}
