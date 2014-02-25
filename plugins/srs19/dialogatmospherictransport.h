#ifndef DIALOGATMOSPHERICTRANSPORT_H
#define DIALOGATMOSPHERICTRANSPORT_H

#include <QDialog>
#include "kdata.h"

namespace Ui {
class DialogAtmosphericTransport;
}

class DialogAtmosphericTransport : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAtmosphericTransport(const KDataArray& dataList, QWidget *parent = 0);
    ~DialogAtmosphericTransport();

    KDataArray dataList() const;
    void setDataList(const KDataArray& d);
    
private:
    Ui::DialogAtmosphericTransport *ui;
};

#endif // DIALOGATMOSPHERICTRANSPORT_H
