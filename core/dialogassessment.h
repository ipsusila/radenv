#ifndef DIALOGASSESSMENT_H
#define DIALOGASSESSMENT_H

#include <QDialog>
#include <QAbstractButton>
#include "radglobal.h"

namespace Ui {
class DialogAssessment;
}

class DialogAssessment : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAssessment(KAssessment * aP = 0, QWidget *parent = 0);
    ~DialogAssessment();

    KAssessment * assessment(QObject * parent) const;

protected slots:
    void onButtonAccepted();
    
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_btnBrowse_clicked();

private:
    Ui::DialogAssessment *ui;
    KAssessment * asPtr;
};

#endif // DIALOGASSESSMENT_H
