#ifndef DIALOGSEWAGEDISCHARGE_H
#define DIALOGSEWAGEDISCHARGE_H

#include <QDialog>
#include "kdata.h"

namespace Ui {
class DialogSewageDischarge;
}

class DialogSewageDischarge : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSewageDischarge(const KDataArray& d, QWidget *parent = 0);
    ~DialogSewageDischarge();

    KDataArray dataList() const;
    void setDataList(const KDataArray& d);

private:
    Ui::DialogSewageDischarge *ui;
};

#endif // DIALOGSEWAGEDISCHARGE_H
