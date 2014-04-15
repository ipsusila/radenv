#include "widgetconstantvalue.h"
#include "ui_widgetconstantvalue.h"

WidgetConstantValue::WidgetConstantValue(KDataArray * up, QWidget *parent) :
    IUserInput(parent),
    ui(new Ui::WidgetConstantValue),
    userParameter(up)
{
    Q_ASSERT(up);

    ui->setupUi(this);

    //add items for combobox
    ConstQuantityList qtys = Srs19::availableQuantities();
    ui->cbQuantities->clear();
    foreach(const Quantity * qty, qtys) {
        if (qty->isNumeric())
            ui->cbQuantities->addItem(qty->displayQuantityWithUnit(), qVariantFromValue((void*)qty));
    }

    //set data for constant table
    ui->tblValues->setData(*userParameter);

    //get radionuclide related values
    for(int k = 0; k < userParameter->size(); k++) {
        const KData& d = userParameter->at(k);
        if (d.contains(KData::Radionuclide)) {
            int idx = ui->cbQuantities->findText(d.quantity().displayQuantityWithUnit());
            if (idx >= 0)
                ui->cbQuantities->setCurrentIndex(idx);

            ui->tblRnValues->setVariable(d.quantityPtr());
            ui->tblRnValues->setData(*userParameter);

            break;
        }
    }

#if !defined(Q_OS_SYMBIAN)
    this->setMinimumSize(450, 250);
#endif
}

WidgetConstantValue::~WidgetConstantValue()
{
    delete ui;
}
bool WidgetConstantValue::validate()
{
    return true;
}

void WidgetConstantValue::acceptValues()
{
    *userParameter = ui->tblValues->data();
    KData d = ui->tblRnValues->data();
    if (d.isValid())
        (*userParameter) << d;
}

void WidgetConstantValue::on_cbQuantities_currentIndexChanged(int index)
{
    QVariant vSym = ui->cbQuantities->itemData(index);
    const Quantity * qty = static_cast<const Quantity *>(vSym.value<void*>());
    if (qty != 0) {
        ui->tblRnValues->setVariable(qty);
    }
}
