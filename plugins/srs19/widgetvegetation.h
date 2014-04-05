#ifndef WIDGETVEGETATION_H
#define WIDGETVEGETATION_H

#include "iuserinput.h"
#include "kdata.h"

namespace Ui {
class WidgetVegetation;
}

class WidgetVegetation : public IUserInput
{
    Q_OBJECT
    
public:
    explicit WidgetVegetation(KDataGroupArray * userParam, QWidget *parent = 0);
    ~WidgetVegetation();

    void acceptValues();
    
private:
    Ui::WidgetVegetation *ui;
    KDataGroupArray * userParameter;
};

#endif // WIDGETVEGETATION_H
