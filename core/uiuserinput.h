#ifndef UIUSERINPUT_H
#define UIUSERINPUT_H

#include <QGridLayout>
#include "iuserinput.h"
#include "kdata.h"

class K_CORE_EXPORT UiUserInput : public IUserInput
{
    Q_OBJECT
public:
    explicit UiUserInput(KDataGroupArray * ga, QWidget *parent = 0);
    virtual ~UiUserInput();
    virtual void acceptValues();
    virtual bool validate();

protected slots:
    void on_quantityController_toggled(bool checked);
    void on_textValue_changed(const QString & txt);
    void on_intValue_changed(int val);
    void on_realValue_changed(double val);
    void on_boolValue_changed(int state);

protected:
    void setEnabled(const Quantity * qty, bool en);
    void quantityChanged(const Quantity * qty, const QVariant & val);
    KData data(const Quantity * qty) const;
    QVariant value(const Quantity * qty) const;

    virtual void onQuantityChanged(const Quantity * qty, const QVariant & val);

    virtual bool validate(KData * d, QWidget * wInput);
    virtual QWidget * createInputControl(const KData * d, int row, QGridLayout * grid, QWidget * p);
    virtual bool assignData(KData * d, QWidget * wInput) const;

private:
    void createControls(KDataGroupArray * ga);
    void createCaption(const KData * d, int row, int * width0, QGridLayout * grid, QWidget * p);
    void createLineEdit(KData * d, int row, QGridLayout * grid, QWidget * p);
    void createIntInput(KData * d, int row, QGridLayout * grid, QWidget * p);
    void createRealInput(KData * d, int row, QGridLayout * grid, QWidget * p);
    void createCheckBox(KData * d, int row, QGridLayout * grid, QWidget * p);
    void createComment(KData * d, int row, QGridLayout * grid, QWidget * p);
    void createUnit(const KData * d, int row, int * width2, QGridLayout * grid, QWidget * p);
    bool createTableInput(KData * d, int row, QGridLayout * grid, QWidget * p);
    void associateQuantityControllers(KDataGroupArray * ga);
    void controlQuantityInput(const Quantity * quantity, bool isSet);

    KDataGroupArray * dataArray;
    QMap<QWidget *, KData *> controlMaps;
    QMap<const Quantity *, QWidget *> quantityMaps;
    QMap<const Quantity *, QVariant> valueMaps;
    QuantityControlList quantityControls;
};

#endif // UIUSERINPUT_H
