#ifndef DIALOGATMOSPHERICDISCHARGE_H
#define DIALOGATMOSPHERICDISCHARGE_H

#include <QDialog>
#include "kdata.h"

namespace Ui {
class DialogAtmosphericDischarge;
}

class DialogAtmosphericDischarge : public QDialog
{
    Q_OBJECT    
public:
    explicit DialogAtmosphericDischarge(const KDataArray& d, QWidget *parent = 0);
    ~DialogAtmosphericDischarge();

    KDataArray dataList() const;
    void setDataList(const KDataArray& d);
    
private:
    Ui::DialogAtmosphericDischarge *ui;
};

#endif // DIALOGATMOSPHERICDISCHARGE_H
