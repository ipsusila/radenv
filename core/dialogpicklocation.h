#ifndef DIALOGPICKLOCATION_H
#define DIALOGPICKLOCATION_H

#include <QDialog>
#include "radglobal.h"

namespace Ui {
class DialogPickLocation;
}

class RADENV_API DialogPickLocation : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPickLocation(QWidget *parent = 0);
    ~DialogPickLocation();
    KLocation location() const;
    KLocation sourceLocation() const;
private:
    Ui::DialogPickLocation *ui;
};

#endif // DIALOGPICKLOCATION_H
