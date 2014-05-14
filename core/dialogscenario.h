#ifndef DIALOGSCENARIO_H
#define DIALOGSCENARIO_H

#include <QDialog>
#include "radglobal.h"

namespace Ui {
class DialogScenario;
}

class DialogScenario : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogScenario(KAssessment * aP, QWidget *parent = 0);
    explicit DialogScenario(KScenario * scP, QWidget *parent = 0);
    ~DialogScenario();
    KScenario * scenario() const;

protected slots:
    void onButtonAccept();
    
private:
    Ui::DialogScenario *ui;
    KAssessment * asPtr;
    KScenario * scPtr;
};

#endif // DIALOGSCENARIO_H
