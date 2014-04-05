#include <QtGui>
#include "uiuserinput.h"
#include "uiradionuclideitemtable.h"
#include "ksymbolcontrol.h"

UiUserInput::UiUserInput(KDataGroupArray * ga, QWidget *parent) :
    IUserInput(parent), dataArray(ga)
{
    createControls(ga);
    associateQuantityControllers(ga);
}
UiUserInput::~UiUserInput()
{
}

void UiUserInput::on_quantityController_toggled(bool checked)
{
    QCheckBox * chkBox = qobject_cast<QCheckBox *>(QObject::sender());
    if (chkBox != 0) {
        KData * field = controlMaps[chkBox];
        Q_ASSERT(field != 0);

        const Quantity * quantity = field->symbolPtr();
        controlQuantityInput(quantity, checked);
    }
}
void UiUserInput::controlQuantityInput(const Quantity * quantity, bool isSet)
{
    for(int k = 0; k < quantityControls.size(); k++) {
        const KSymbolControl & qc = quantityControls.at(k);
        const Quantity * ctrl = qc.controller();
        if (ctrl != quantity)
            continue;

        ConstSymbolList ctrlQuantity = qc.controlledSymbols();
        for(int n = 0; n < ctrlQuantity.size(); n++) {
            QWidget * w = quantityMaps[ctrlQuantity.at(n)];
            if (qc.isEnabledOnSet())
                w->setEnabled(isSet);
            else
                w->setEnabled(!isSet);
        }
    }
}

void UiUserInput::createControls(KDataGroupArray *ga)
{
    if (ga == 0)
        return;

    //controls
    KDataGroupArray::iterator beg = ga->begin();
    KDataGroupArray::iterator end = ga->end();

    QVBoxLayout * vbox = new QVBoxLayout();
    while (beg != end) {
        QString name = beg->name;
        int sz = beg->count();
        if (sz > 0) {
            QGroupBox * gb = new QGroupBox(name, this);
            QGridLayout * grid = new QGridLayout();
            grid->setVerticalSpacing(3);
            for (int k = 0; k < sz; k++) {
                KData * d = beg->pointerAt(k);

                //if array, allocate table
                if (d->contains(KData::Array)) {
                    UiRadionuclideItemTable * table = new UiRadionuclideItemTable(gb);
                    table->setSymbol(d->symbolPtr());
                    table->setData(*d);
                    grid->addWidget(table, k, 0, 1, 3);

                    //add to control and quantity maps
                    controlMaps[table] = d;
                    quantityMaps[d->symbolPtr()] = table;
                    continue;
                }

                //create controls according to symbol
                QLineEdit * ed;
                QSpinBox * inpInt;
                QCheckBox * chkBox;
                QDoubleSpinBox * inpReal;
                const Quantity & sym = d->symbol();

                //caption of the field
                QLabel * label;
                switch(sym.type) {
                case Rad::NumText:
                case Rad::Integer:
                case Rad::Real:
                case Rad::Text:
                    label = new QLabel(sym.displayText(), gb);
                    label->setMinimumWidth(170);
                    label->setWordWrap(true);
                    grid->addWidget(label, k, 0);
                    break;
                default:
                    break;
                }

                //add user input according to the type of symbol
                switch(sym.type)
                {
                case Rad::Text:
                    //create text input control
                    ed = new QLineEdit(gb);
                    ed->setToolTip(sym.description);
                    ed->setText(d->value().toString());
                    grid->addWidget(ed, k, 1);
                    controlMaps[ed] = d;
                    quantityMaps[d->symbolPtr()] = ed;
                    break;
                case Rad::NumText:
                    //create text input control for numeric
                    ed = new QLineEdit(gb);
                    ed->setToolTip(sym.description);
                    ed->setText(QString::number(d->numericValue()));
                    grid->addWidget(ed, k, 1);
                    controlMaps[ed] = d;
                    quantityMaps[d->symbolPtr()] = ed;
                    break;
                case Rad::Integer:
                    //create input control for integer value
                    inpInt = new QSpinBox(gb);
                    inpInt->setToolTip(sym.description);
                    inpInt->setMinimum(sym.minValue);
                    inpInt->setMaximum(sym.maxValue);
                    inpInt->setValue((int)d->numericValue());
                    grid->addWidget(inpInt, k, 1);
                    controlMaps[inpInt] = d;
                    quantityMaps[d->symbolPtr()] = inpInt;
                    break;
                case Rad::Boolean:
                    //create input control for boolean value
                    chkBox = new QCheckBox(sym.text, gb);
                    chkBox->setToolTip(sym.description);
                    chkBox->setChecked(d->value().toBool());
                    grid->addWidget(chkBox, k, 0, 1, 2);
                    controlMaps[chkBox] = d;
                    quantityMaps[d->symbolPtr()] = chkBox;
                    break;
                case Rad::Real:
                    //create input control for numeric value
                    inpReal = new QDoubleSpinBox(gb);
                    inpReal->setToolTip(sym.description);
                    inpReal->setMinimum(sym.minValue);
                    inpReal->setMaximum(sym.maxValue);
                    inpReal->setDecimals(sym.decimal);
                    inpReal->setValue(d->numericValue());
                    grid->addWidget(inpReal, k, 1);
                    controlMaps[inpReal] = d;
                    quantityMaps[d->symbolPtr()] = inpReal;
                    break;
                case Rad::Comment:
                    label = new QLabel(d->value().toString(), gb);
                    label->setWordWrap(true);
                    grid->addWidget(label, k, 1, 1, 2);
                    break;
                default:
                    break;
                }

                //if quantity has unit, then create label for displaying unit
                if (!sym.unit.isEmpty()) {
                    QLabel * unit = new QLabel(sym.unit, gb);
                    unit->setMinimumWidth(50);
                    unit->setMaximumWidth(120);
                    unit->setWordWrap(true);
                    grid->addWidget(unit, k, 2);
                }

                //TODO
                //add indicator for optional, conditional mandatory and mandatory fields
            }

            gb->setLayout(grid);
            vbox->addWidget(gb);
        }
        beg++;
    }

    //set main layout
    this->setLayout(vbox);
}

void UiUserInput::associateQuantityControllers(KDataGroupArray *ga)
{
    quantityControls = ga->quantityControls();
    for(int k = 0; k < quantityControls.size(); k++) {
        const KSymbolControl & qc = quantityControls.at(k);
        const Quantity * ctrl = qc.controller();

        //connect controller signal
        QCheckBox * chkBox = qobject_cast<QCheckBox *>(quantityMaps[ctrl]);
        if (chkBox != 0) {
            connect(chkBox, SIGNAL(toggled(bool)), this, SLOT(on_quantityController_toggled(bool)));

            //set initial values
            controlQuantityInput(ctrl, chkBox->isChecked());
        }
    }
}

void UiUserInput::acceptValues()
{
    QMap<QWidget *, KData *>::iterator beg = controlMaps.begin();
    QMap<QWidget *, KData *>::iterator end = controlMaps.end();
    while (beg != end) {
        KData * d = beg.value();
        if (!d->isValid()) {
            beg++;
            continue;
        }

        QWidget * wInput = beg.key();
        beg++;

        QLineEdit * ed = qobject_cast<QLineEdit *>(wInput);
        if (ed != 0) {
            if (d->symbol().type == Rad::NumText)
                d->setValue(ed->text().toDouble());
            else
                d->setValue(ed->text());
            continue;
        }

        QSpinBox * sb = qobject_cast<QSpinBox *>(wInput);
        if (sb != 0) {
            d->setValue(QVariant(sb->value()));
            continue;
        }

        QDoubleSpinBox * dsb = qobject_cast<QDoubleSpinBox *>(wInput);
        if (dsb != 0) {
            d->setValue(dsb->value());
            continue;
        }

        QCheckBox * chk = qobject_cast<QCheckBox *>(wInput);
        if (chk != 0) {
            d->setValue(QVariant(chk->isChecked()));
            continue;
        }

        UiRadionuclideItemTable * table = qobject_cast<UiRadionuclideItemTable *>(wInput);
        if (table != 0) {
            KData td = table->data();
            if (td.isValid())
                *d = td;

            continue;
        }
    }
}
