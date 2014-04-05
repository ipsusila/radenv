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
    ConstSymbolList syms = Srs19::availableSymbols();
    ui->cbSymbols->clear();
    foreach(const Quantity * sym, syms) {
        if (sym->isNumeric())
            ui->cbSymbols->addItem(sym->displaySymbolWithUnit(), qVariantFromValue((void*)sym));
    }

    //set data for constant table
    ui->tblValues->setData(*userParameter);

    //get radionuclide related values
    for(int k = 0; k < userParameter->size(); k++) {
        const KData& d = userParameter->at(k);
        if (d.contains(KData::Radionuclide)) {
            int idx = ui->cbSymbols->findText(d.symbol().displaySymbolWithUnit());
            if (idx >= 0)
                ui->cbSymbols->setCurrentIndex(idx);

            ui->tblRnValues->setVariable(d.symbolPtr());
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

void WidgetConstantValue::acceptValues()
{
    *userParameter = ui->tblValues->data();
    KData d = ui->tblRnValues->data();
    if (d.isValid())
        (*userParameter) << d;
}

void WidgetConstantValue::on_cbSymbols_currentIndexChanged(int index)
{
    QVariant vSym = ui->cbSymbols->itemData(index);
    const Quantity * sym = static_cast<const Quantity *>(vSym.value<void*>());
    if (sym != 0) {
        ui->tblRnValues->setVariable(sym);
    }
}
