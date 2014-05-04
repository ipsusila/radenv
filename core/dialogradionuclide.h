#ifndef DIALOGRADIONUCLIDE_H
#define DIALOGRADIONUCLIDE_H

#include <QDialog>
#include <QModelIndex>
#include "kradionuclide.h"

class RadionuclideModel;

namespace Ui {
class DialogRadionuclide;
}

class K_CORE_EXPORT DialogRadionuclide : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogRadionuclide(KStorage * stg, QWidget *parent = 0);
    ~DialogRadionuclide();

    KRadionuclide selectedRadionuclide() const;
    
private slots:
    void on_cmdNew_clicked();
    void on_cmdSave_clicked();
    void on_cmdRemove_clicked();
    void onSelectedRowChanged(const QModelIndex & current, const QModelIndex & previous );

private:
    void setupRadionuclides(KStorage * stg);

    Ui::DialogRadionuclide *ui;
    RadionuclideModel * _model;
};

#endif // DIALOGRADIONUCLIDE_H
