#ifndef USERINPUT_H
#define USERINPUT_H

#include "iuserinput.h"
#include "kdata.h"

class K_CORE_EXPORT UserInput : public IUserInput
{
    Q_OBJECT
public:
    explicit UserInput(KDataGroupArray * ga, QWidget *parent = 0);
    virtual ~UserInput();
    void acceptValues();
    
private:
    void createControls(KDataGroupArray * ga);

    KDataGroupArray * dataArray;
    QMap<QWidget *, KData *> controlMaps;
};

#endif // USERINPUT_H
