#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "dialoguserinput.h"
#include "ksettingmanager.h"
#include "kapplication.h"

DialogUserInput::DialogUserInput(IModel * m, IUserInput * ui, QWidget *parent) :
    QDialog(parent), model(m), userInput(ui)
{
    QDialogButtonBox * bbox = new QDialogButtonBox(this);
    QVBoxLayout * vbox = new QVBoxLayout();

    bbox->addButton(QDialogButtonBox::Ok);
    bbox->addButton(QDialogButtonBox::Cancel);
    bbox->setCenterButtons(true);
    connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
    vbox->addWidget(ui);
    vbox->addWidget(bbox);
    setLayout(vbox);
    setWindowTitle(QString(tr("Parameter(s) for %1 - %2"))
                        .arg(model->tagName())
                        .arg(model->info().text()));

    QRect rect;
    QSize mSize;
    KSettingManager * setting = model->factory()->settingManager();
    if (setting != 0)
        rect = setting->geometry(model);
    else
        rect = this->geometry();

    //convert to valid geometry/size
    KApplication::selfInstance()->toValidDialogGeometry(&rect, &mSize);

    //this maksimum size
    setMaximumSize(mSize);

    //resize and move
    resize(rect.size());
    move(rect.topLeft());

}
DialogUserInput::~DialogUserInput()
{
    saveSettings();
}

void DialogUserInput::accept()
{
    if (userInput->validate())
        QDialog::accept();
}
void DialogUserInput::saveSettings()
{
    //save size
    KSettingManager * setting = model->factory()->settingManager();
    if (setting != 0) {
        QRect rect = QRect(pos(), size());
        setting->saveGeometry(model, rect);
    }
}
