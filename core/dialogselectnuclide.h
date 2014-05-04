#ifndef DIALOGSELECTNUCLIDE_H
#define DIALOGSELECTNUCLIDE_H

#include <QDialog>
#include "radglobal.h"

namespace Ui {
class DialogSelectNuclide;
}

class K_CORE_EXPORT DialogSelectNuclide : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSelectNuclide(KStorage * storage, const QStringList & selectedList = QStringList(),  QWidget *parent = 0);
    ~DialogSelectNuclide();
    QStringList selectedRadionuclides() const;
    
private slots:
    void on_cmdSelect_clicked();
    void on_cmdSelectAll_clicked();
    void on_cmdDeselect_clicked();
    void on_cmdDeselectAll_clicked();

private:
    void setupRadionuclides(KStorage * storage, const QStringList & selectedList);

    Ui::DialogSelectNuclide *ui;
};

#endif // DIALOGSELECTNUCLIDE_H
