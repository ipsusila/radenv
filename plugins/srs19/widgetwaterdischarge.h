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
    explicit WidgetWaterDischarge(KDataArray * userParam, QWidget *parent = 0);
    ~WidgetWaterDischarge();

    virtual void acceptValues();
    bool validate();
    QSize sizeHint() const;
    
private:
    Ui::WidgetWaterDischarge *ui;
    KDataArray * userParameter;
};

#endif // WIDGETWATERDISCHARGE_H
