#ifndef DIALOGUSERINPUT_H
#define DIALOGUSERINPUT_H

#include <QDialog>
#include "iuserinput.h"
#include "imodel.h"

class K_CORE_EXPORT DialogUserInput : public QDialog
{
    Q_OBJECT
public:
    explicit DialogUserInput(IModel * m, IUserInput * ui, QWidget *parent = 0);
    ~DialogUserInput();
    
signals:
    
public slots:
    virtual void accept();
    void saveSettings();

private:
    IModel * model;
    IUserInput * userInput;
};

#endif // DIALOGUSERINPUT_H
