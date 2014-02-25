#ifndef DIALOGCALCULATION_H
#define DIALOGCALCULATION_H

#include <QDialog>
#include "kcalculationinfo.h"

namespace Ui {
class DialogCalculation;
}

class DialogCalculation : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogCalculation(QWidget *parent = 0);
    ~DialogCalculation();

    KCalculationInfo calculationInfo() const;
    
private slots:
    void on_rbNTimes_toggled(bool checked);
    void on_rbContinuous_toggled(bool checked);
    void on_rbOnce_toggled(bool checked);

private:
    Ui::DialogCalculation *ui;
};

#endif // DIALOGCALCULATION_H
