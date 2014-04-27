#ifndef IUSERINPUT_H
#define IUSERINPUT_H

#include <QWidget>

class IUserInput : public QWidget
{
protected:
    IUserInput(QWidget * parent) : QWidget(parent) {}
public :
    virtual ~IUserInput() {}
    virtual void acceptValues() = 0;
    virtual bool validate() = 0;
    virtual bool isEmpty() const = 0;
};

#endif // IUSERINPUT_H
