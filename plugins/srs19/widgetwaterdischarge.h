#ifndef WIDGETWATERDISCHARGE_H
#define WIDGETWATERDISCHARGE_H

#include "iuserinput.h"
#include "kdata.h"

namespace Ui {
class WidgetWaterDischarge;
}

class WidgetWaterDischarge : public IUserInput
{
    Q_OBJECT
    
public:
    explicit WidgetWaterDischarge(KStorage * storage, KDataArray * userParam, QWidget *parent = 0);
    ~WidgetWaterDischarge();

    bool isEmpty() const;
    virtual void acceptValues();
    bool validate();
    QSize sizeHint() const;
    
private:
    Ui::WidgetWaterDischarge *ui;
    KDataArray * userParameter;
};

#endif // WIDGETWATERDISCHARGE_H
