#ifndef UIUSERINPUT_H
#define UIUSERINPUT_H

#include "iuserinput.h"
#include "kdata.h"

class K_CORE_EXPORT UiUserInput : public IUserInput
{
    Q_OBJECT
public:
    explicit UiUserInput(KDataGroupArray * ga, QWidget *parent = 0);
    virtual ~UiUserInput();
    void acceptValues();

protected slots:
    void on_quantityController_toggled(bool checked);
    
private:
    void createControls(KDataGroupArray * ga);
    void associateQuantityControllers(KDataGroupArray * ga);
    void controlQuantityInput(const Quantity * quantity, bool isSet);

    KDataGroupArray * dataArray;
    QMap<QWidget *, KData *> controlMaps;
    QMap<const Quantity *, QWidget *> quantityMaps;
    QuantityControlList quantityControls;
};

#endif // UIUSERINPUT_H
