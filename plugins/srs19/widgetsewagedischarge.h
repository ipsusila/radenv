#ifndef WIDGETSEWAGEDISCHARGE_H
#define WIDGETSEWAGEDISCHARGE_H

#include "iuserinput.h"
#include "kdata.h"

namespace Ui {
class WidgetSewageDischarge;
}

class WidgetSewageDischarge : public IUserInput
{
    Q_OBJECT

public:
    explicit WidgetSewageDischarge(KDataArray * userParam, QWidget *parent = 0);
    ~WidgetSewageDischarge();

    virtual void acceptValues();
    QSize sizeHint() const;
    
private:
    Ui::WidgetSewageDischarge *ui;
    KDataArray * userParameter;
};

#endif // WIDGETSEWAGEDISCHARGE_H
