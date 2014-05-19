#include <QSettings>
#include <QVariant>
#include "dialogpicklocation.h"
#include "ui_dialogpicklocation.h"
#include "kapplication.h"
#include "klocation.h"
#include "kstorage.h"

DialogPickLocation::DialogPickLocation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPickLocation)
{
    ui->setupUi(this);

    KApplication * app = KApplication::selfInstance();
    if (app->hasStorage())
    {
        const LocationList * locs = app->storage()->locations();
        ui->cbSourceLoc->addItem(" ");
        for(int i = 0; i < locs->count(); i++)
        {
            const KLocation & loc = locs->at(i);
            ui->cbSourceLoc->addItem(loc.name(), QVariant(loc.code()));
        }
        ui->cbSourceLoc->setCurrentIndex(0);
    }

    app->setupValidGeometry("picklocdialog/geometry", this);
}

DialogPickLocation::~DialogPickLocation()
{
    QSettings settings;
    settings.setValue("picklocdialog/geometry", this->saveGeometry());
    delete ui;
}
KLocation DialogPickLocation::location() const
{
    KLocation loc(ui->inpCode->text());
    loc.setName(ui->inpName->text());
    loc.setDescription(ui->inpDescription->toPlainText());
    loc.setMarker(ui->inpMarker->text());

    return loc;
}
KLocation DialogPickLocation::sourceLocation() const
{
    if (KApplication::selfInstance()->hasStorage())
    {
        QString code = ui->cbSourceLoc->itemData(ui->cbSourceLoc->currentIndex()).toString();
        if (!code.isEmpty())
            return KApplication::selfInstance()->storage()->location(code);
    }
    return KLocation();
}
