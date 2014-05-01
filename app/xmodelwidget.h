#ifndef XMODELWIDGET_H
#define XMODELWIDGET_H

#include <QWidget>
#include <QGridLayout>

class XModelGroup : public QWidget
{
    Q_OBJECT
public:
    explicit XModelGroup(const QString& cap, QWidget * p = 0);
    void addAction(QAction * act);
    void setColCount(int count);
    int colCount() const;

private:
    QGridLayout * _mainLayout;
    int _nactions;
    int _colCount;
};

class XModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XModelWidget(QWidget *parent = 0);

    XModelGroup * createGroup(const QString& nm);
    void addModelAction(XModelGroup * group, QAction * act);

private:
    QVBoxLayout * mainLayout;
    
signals:
    
public slots:
    
};

#endif // XMODELWIDGET_H
