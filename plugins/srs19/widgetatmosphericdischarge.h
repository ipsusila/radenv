#ifndef WIDGETATMOSPHERICDISCHARGE_H
#define WIDGETATMOSPHERICDISCHARGE_H

#include "iuserinput.h"
#include "kdata.h"

namespace Ui {
class WidgetAtmosphericDischarge;
}

class WidgetAtmosphericDischarge : public IUserInput
{
    Q_OBJECT
    
public:
    explicit WidgetAtmosphericDischarge(KStorage *storage, KDataArray * userParam, QWidget *parent = 0);
    ~WidgetAtmosphericDischarge();

    virtual void acceptValues();
    bool validate();
    bool isEmpty() const;
    QSize sizeHint() const;
    
private:
    Ui::WidgetAtmosphericDischarge *ui;
    KDataArray * userParameter;
};

#endif // WIDGETATMOSPHERICDISCHARGE_H
