#include <QLabel>
#include <QToolButton>
#include <QAction>
#include <QtDebug>
#include "xmodelwidget.h"
#include "xactionbutton.h"

XModelGroup::XModelGroup(const QString& cap, QWidget * p)
    : QWidget(p), _nactions(0), _colCount(3)
{
    //setStyleSheet("border: 1px solid gray;");

    _mainLayout = new QGridLayout(this);
    _mainLayout->setHorizontalSpacing(2);
    _mainLayout->setVerticalSpacing(2);
    this->setLayout(_mainLayout);

    //add label
    QLabel * label = new QLabel(cap, this);
    _mainLayout->addWidget(label, 0, 0, 1, _colCount, Qt::AlignTop);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(220,220,220));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}
void XModelGroup::setColCount(int count)
{
    _colCount = count;
    if (_colCount < 0)
        _colCount = 2;
}
int XModelGroup::colCount() const
{
    return _colCount;
}

void XModelGroup::addAction(QAction * act)
{
    //XActionButton * tb = new XActionButton(this->parentWidget());
    //tb->setAction(act);
    QToolButton * tb = new QToolButton(this);
    tb->setDefaultAction(act);

    int nr = _nactions / _colCount;
    int nc = _nactions % _colCount;
    _mainLayout->addWidget(tb, 1+nr, nc, Qt::AlignTop);

    _nactions++;
}

XModelWidget::XModelWidget(QWidget *parent) :
    QWidget(parent)
{
    //create layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(1);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

XModelGroup * XModelWidget::createGroup(const QString& nm)
{
    int idx = mainLayout->count() - 1;
    if (idx < 0)
        idx = 0;

    XModelGroup * mg = new XModelGroup(nm,this);
    mainLayout->insertWidget(idx, mg, 0, Qt::AlignTop);

    return mg;
}

void XModelWidget::addModelAction(XModelGroup * group, QAction * act)
{
    group->addAction(act);
}
