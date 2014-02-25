#ifndef DIALOGWATERDISCHARGE_H
#define DIALOGWATERDISCHARGE_H

#include <QDialog>
#include "kdata.h"

namespace Ui {
class DialogWaterDischarge;
}

class DialogWaterDischarge : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWaterDischarge(const KDataArray& d, QWidget *parent = 0);
    ~DialogWaterDischarge();

    KDataArray dataList() const;
    void setDataList(const KDataArray& d);
    
private:
    Ui::DialogWaterDischarge *ui;
};

#endif // DIALOGWATERDISCHARGE_H
