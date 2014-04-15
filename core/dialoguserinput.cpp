#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "dialoguserinput.h"
#include "ksettingmanager.h"

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

    //set size
    KSettingManager * setting = model->factory()->settingManager();
    if (setting != 0) {
        QRect rect = setting->geometry(model);
        if (rect.isValid())
            this->setGeometry(rect);
    }
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
        setting->saveGeometry(model, this->geometry());
    }
}
