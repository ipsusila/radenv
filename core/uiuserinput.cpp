#include <QtGui>
#include "uiuserinput.h"
#include "uiradionuclideitemtable.h"
#include "kquantitycontrol.h"

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

        const Quantity * quantity = field->quantityPtr();
        controlQuantityInput(quantity, checked);
    }
}
void UiUserInput::controlQuantityInput(const Quantity * quantity, bool isSet)
{
    for(int k = 0; k < quantityControls.size(); k++) {
        const KQuantityControl & qc = quantityControls.at(k);
        const Quantity * ctrl = qc.controller();
        if (ctrl != quantity)
            continue;

        ConstQuantityList ctrlQuantity = qc.controlledQuantities();
        for(int n = 0; n < ctrlQuantity.size(); n++) {
            //get control associated with quantity
            QWidget * w = quantityMaps[ctrlQuantity.at(n)];
            if (w == 0)
                continue;

            //enable/disable control
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

    int width0 = 0, width2 = 0; //width of column 2
    QList<QGridLayout *> gridList;
    QVBoxLayout * vbox = new QVBoxLayout();
    while (beg != end) {
        QString name = beg->name;
        int sz = beg->count();
        if (sz > 0) {
            QGroupBox * gb = new QGroupBox(name, this);
            QGridLayout * grid = new QGridLayout();
            grid->setVerticalSpacing(3);
            grid->setColumnStretch(0, 1);
            grid->setColumnStretch(1, 2);
            grid->setColumnStretch(2, 0);

            gridList.append(grid);
            for (int k = 0; k < sz; k++) {
                KData * d = beg->pointerAt(k);

                //if array, allocate table
                if (d->contains(KData::Array)) {
                    UiRadionuclideItemTable * table = new UiRadionuclideItemTable(gb);
                    table->setQuantity(d->quantityPtr());
                    table->setData(*d);
                    grid->addWidget(table, k, 0, 1, 3);

                    //add to control and quantity maps
                    controlMaps[table] = d;
                    quantityMaps[d->quantityPtr()] = table;
                    continue;
                }

                //create controls according to symbol
                QLineEdit * ed;
                QSpinBox * inpInt;
                QCheckBox * chkBox;
                QDoubleSpinBox * inpReal;
                const Quantity & qty = d->quantity();

                //caption of the field
                QLabel * label;
                switch(qty.type) {
                case Rad::NumText:
                case Rad::Integer:
                case Rad::Real:
                case Rad::Text:
                    label = new QLabel(qty.displayText(), gb);
                    grid->addWidget(label, k, 0);
                    label->adjustSize();

                    width0 = qMax(width0, label->width());
                    break;
                default:
                    break;
                }

                //add user input according to the type of symbol
                switch(qty.type)
                {
                case Rad::Text:
                    //create text input control
                    ed = new QLineEdit(gb);
                    ed->setToolTip(qty.description);
                    ed->setText(d->value().toString());
                    grid->addWidget(ed, k, 1);

                    controlMaps[ed] = d;
                    quantityMaps[d->quantityPtr()] = ed;
                    break;
                case Rad::NumText:
                    //create text input control for numeric
                    ed = new QLineEdit(gb);
                    ed->setToolTip(qty.description);
                    ed->setText(QString::number(d->numericValue()));
                    grid->addWidget(ed, k, 1);

                    controlMaps[ed] = d;
                    quantityMaps[d->quantityPtr()] = ed;
                    break;
                case Rad::Integer:
                    //create input control for integer value
                    inpInt = new QSpinBox(gb);
                    inpInt->setToolTip(qty.description);
                    inpInt->setMinimum(qty.minValue);
                    inpInt->setMaximum(qty.maxValue);
                    inpInt->setValue((int)d->numericValue());
                    grid->addWidget(inpInt, k, 1);

                    controlMaps[inpInt] = d;
                    quantityMaps[d->quantityPtr()] = inpInt;
                    break;
                case Rad::Boolean:
                    //create input control for boolean value
                    chkBox = new QCheckBox(qty.text, gb);
                    chkBox->setToolTip(qty.description);
                    chkBox->setChecked(d->value().toBool());
                    grid->addWidget(chkBox, k, 0, 1, 2);

                    controlMaps[chkBox] = d;
                    quantityMaps[d->quantityPtr()] = chkBox;
                    break;
                case Rad::Real:
                    //create input control for numeric value
                    inpReal = new QDoubleSpinBox(gb);
                    inpReal->setToolTip(qty.description);
                    inpReal->setMinimum(qty.minValue);
                    inpReal->setMaximum(qty.maxValue);
                    inpReal->setDecimals(qty.decimal);
                    inpReal->setValue(d->numericValue());
                    grid->addWidget(inpReal, k, 1);

                    controlMaps[inpReal] = d;
                    quantityMaps[d->quantityPtr()] = inpReal;
                    break;
                case Rad::Comment:
                case Rad::LongComment:
                    label = new QLabel(d->value().toString(), gb);
                    label->setWordWrap(true);
                    if (qty.type == Rad::LongComment)
                        grid->addWidget(label, k, 0, 1, 3);
                    else
                        grid->addWidget(label, k, 1, 1, 2);
                    break;
                default:
                    break;
                }

                //if quantity has unit, then create label for displaying unit
                if (!qty.unit.isEmpty()) {
                    QLabel * unit = new QLabel(qty.unit, gb);
                    unit->setWordWrap(false);
                    grid->addWidget(unit, k, 2);
                    unit->adjustSize();

                    //remember size
                    width2 = qMax(width2, unit->width());
                }

                //TODO
                //add indicator for optional, conditional mandatory and mandatory fields
            }

            gb->setLayout(grid);
            vbox->addWidget(gb);
        }
        beg++;
    }

    //resize unit column
    for(int k = 0; k < gridList.size(); k++) {
        QGridLayout * grid = gridList.at(k);
        grid->setColumnMinimumWidth(0, width0);
        grid->setColumnMinimumWidth(2, width2);
    }

    //set main layout
    this->setLayout(vbox);
}

void UiUserInput::associateQuantityControllers(KDataGroupArray *ga)
{
    quantityControls = ga->quantityControls();
    for(int k = 0; k < quantityControls.size(); k++) {
        const KQuantityControl & qc = quantityControls.at(k);
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
            if (d->quantity().type == Rad::NumText)
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
