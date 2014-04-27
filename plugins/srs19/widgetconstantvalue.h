#ifndef WIDGETCONSTANTVALUE_H
#define WIDGETCONSTANTVALUE_H

#include "iuserinput.h"
#include "kdata.h"

namespace Ui {
class WidgetConstantValue;
}

class WidgetConstantValue : public IUserInput
{
    Q_OBJECT
    
public:
    explicit WidgetConstantValue(KDataArray * up, QWidget *parent = 0);
    ~WidgetConstantValue();

    bool validate();
    void acceptValues();
    bool isEmpty() const;
    
private slots:

    void on_cbQuantities_currentIndexChanged(int index);

private:
    Ui::WidgetConstantValue *ui;
    KDataArray * userParameter;
};

#endif // WIDGETCONSTANTVALUE_H
