#include "widgetvegetation.h"
#include "ui_widgetvegetation.h"

WidgetVegetation::WidgetVegetation(KDataGroupArray * userParam, QWidget *parent) :
    IUserInput(parent),
    ui(new Ui::WidgetVegetation),
    userParameter(userParam)
{
    ui->setupUi(this);
}

WidgetVegetation::~WidgetVegetation()
{
    delete ui;
}
void WidgetVegetation::acceptValues()
{

}
