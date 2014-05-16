#ifndef DIALOGOPTION_H
#define DIALOGOPTION_H

#include <QDialog>

namespace Ui {
class DialogOption;
}

class DialogOption : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOption(QWidget *parent = 0);
    ~DialogOption();

public slots:
    void loadOptions();
    void applyOptions();

private:
    Ui::DialogOption *ui;
};

#endif // DIALOGOPTION_H
