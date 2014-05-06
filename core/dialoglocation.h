#ifndef DIALOGLOCATION_H
#define DIALOGLOCATION_H

#include <QDialog>
#include <QModelIndex>
#include "klocation.h"

class LocationModel;

namespace Ui {
class DialogLocation;
}

class RADENV_API DialogLocation : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogLocation(KStorage * storage, QWidget *parent = 0);
    ~DialogLocation();

    KLocation selectedLocation() const;
    
private slots:
    void on_cmdNew_clicked();
    void on_cmdSave_clicked();
    void on_cmdRemove_clicked();
    void onSelectedRowChanged(const QModelIndex & current, const QModelIndex & previous );

    void on_cmdBrowse_clicked();

    void on_tblLocation_doubleClicked(const QModelIndex &index);

private:
    Ui::DialogLocation *ui;
    LocationModel * _model;

    void setupLocations(KStorage * stg);
    void displayMarker(const QPixmap& pic);
};

#endif // DIALOGLOCATION_H
