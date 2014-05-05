#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include "uiuserinput.h"
#include "uiarrayitemtable.h"
#include "kquantitycontrol.h"
#include "koutput.h"
#include "imodelfactory.h"

UiUserInput::UiUserInput(IModelFactory *fact, KDataGroupArray * ga, QWidget *parent) :
    IUserInput(parent), factory(fact), dataArray(ga)
{
    createControls(ga);
    associateQuantityControllers(ga);
}
UiUserInput::~UiUserInput()
{
}
void UiUserInput::setEnabled(const Quantity * qty, bool en)
{
    QWidget * w = quantityMaps[qty];
    if (w != 0)
        w->setEnabled(en);
}

void UiUserInput::onQuantityChanged(const Quantity * qty, const QVariant & val)
{
    Q_UNUSED(qty);
    Q_UNUSED(val);
}

void UiUserInput::quantityChanged(const Quantity * qty, const QVariant & val)
{
    valueMaps[qty] = val;
    onQuantityChanged(qty, val);
}

void UiUserInput::on_textValue_changed(const QString & txt)
{
    QWidget * w = qobject_cast<QWidget *>(QObject::sender());
    KData * d = controlMaps[w];
    if (d != 0) {
        quantityChanged(d->quantityPtr(), QVariant(txt));
    }
}

void UiUserInput::on_intValue_changed(int val)
{
    QWidget * w = qobject_cast<QWidget *>(QObject::sender());
    KData * d = controlMaps[w];
    if (d != 0) {
        quantityChanged(d->quantityPtr(), QVariant(val));
    }
}

void UiUserInput::on_realValue_changed(double val)
{
    QWidget * w = qobject_cast<QWidget *>(QObject::sender());
    KData * d = controlMaps[w];
    if (d != 0) {
        quantityChanged(d->quantityPtr(), QVariant(val));
    }
}

void UiUserInput::on_boolValue_changed(int state)
{
    QWidget * w = qobject_cast<QWidget *>(QObject::sender());
    KData * d = controlMaps[w];
    if (d != 0) {
        quantityChanged(d->quantityPtr(), QVariant(state));
    }
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

void UiUserInput::createCaption(const KData * d, int row, int * width0,
                                QGridLayout * grid, QWidget * p)
{
    QLabel * label;
    const Quantity & qty = d->quantity();
    if (d->contains(KData::Mandatory))
        label = new QLabel(qty.displayText()+"**", p);
    else if (d->contains(KData::ConditionalMandatory))
        label = new QLabel(qty.displayText()+"*", p);
    else
        label = new QLabel(qty.displayText(), p);
    grid->addWidget(label, row, 0);

    label->adjustSize();
    *width0 = qMax(*width0, label->width());
}
void UiUserInput::createLineEdit(KData * d, int row, QGridLayout * grid, QWidget * p)
{
    const Quantity & qty = d->quantity();
    //create text input control
    QLineEdit * ed = new QLineEdit(p);
    ed->setToolTip(qty.description);
    ed->setText(d->value().toString());
    grid->addWidget(ed, row, 1);
    connect(ed, SIGNAL(textChanged(QString)), this, SLOT(on_textValue_changed(QString)));

    controlMaps[ed] = d;
    quantityMaps[d->quantityPtr()] = ed;
    valueMaps[d->quantityPtr()] = d->value();
}
void UiUserInput::createIntInput(KData * d, int row, QGridLayout * grid, QWidget * p)
{
    const Quantity & qty = d->quantity();
    //create input control for integer value
    QSpinBox * inpInt = new QSpinBox(p);
    inpInt->setToolTip(qty.description);
    inpInt->setMinimum(qty.minValue);
    inpInt->setMaximum(qty.maxValue);
    inpInt->setValue((int)d->numericValue());
    grid->addWidget(inpInt, row, 1);
    connect(inpInt, SIGNAL(valueChanged(int)), this, SLOT(on_intValue_changed(int)));

    controlMaps[inpInt] = d;
    quantityMaps[d->quantityPtr()] = inpInt;
    valueMaps[d->quantityPtr()] = d->value();
}

void UiUserInput::createRealInput(KData * d, int row, QGridLayout * grid, QWidget * p)
{
    const Quantity & qty = d->quantity();
    //create input control for numeric value
    QDoubleSpinBox * inpReal = new QDoubleSpinBox(p);
    inpReal->setToolTip(qty.description);
    inpReal->setMinimum(qty.minValue);
    inpReal->setMaximum(qty.maxValue);
    inpReal->setDecimals(qty.decimal);
    inpReal->setValue(d->numericValue());
    grid->addWidget(inpReal, row, 1);
    connect(inpReal, SIGNAL(valueChanged(double)), this, SLOT(on_realValue_changed(double)));

    controlMaps[inpReal] = d;
    quantityMaps[d->quantityPtr()] = inpReal;
    valueMaps[d->quantityPtr()] = d->value();
}
void UiUserInput::createCheckBox(KData * d, int row, QGridLayout * grid, QWidget * p)
{
    const Quantity & qty = d->quantity();
    //create input control for boolean value
    QCheckBox * chkBox = new QCheckBox(qty.text, p);
    chkBox->setToolTip(qty.description);
    chkBox->setChecked(d->value().toBool());
    grid->addWidget(chkBox, row, 0, 1, 2);
    connect(chkBox, SIGNAL(stateChanged(int)), this, SLOT(on_boolValue_changed(int)));

    controlMaps[chkBox] = d;
    quantityMaps[d->quantityPtr()] = chkBox;
    valueMaps[d->quantityPtr()] = d->value();
}
void UiUserInput::createComment(KData * d, int row, QGridLayout * grid, QWidget * p)
{
    QLabel * label = new QLabel(d->value().toString(), p);
    label->setWordWrap(true);
    if (d->quantity().type == Rad::LongComment)
        grid->addWidget(label, row, 0, 1, 3);
    else
        grid->addWidget(label, row, 1, 1, 2);
}
void UiUserInput::createUnit(const KData * d, int row, int * width2,
                             QGridLayout * grid, QWidget * p)
{
    //if quantity has unit, then create label for displaying unit
    const Quantity & qty = d->quantity();
    if (!qty.unit.isEmpty()) {
        QLabel * unit = new QLabel(qty.unit, p);
        unit->setWordWrap(false);
        grid->addWidget(unit, row, 2);
        unit->adjustSize();

        //remember size
        *width2 = qMax(*width2, unit->width());
    }
}
bool UiUserInput::createTableInput(KData *d, int row, QGridLayout *grid, QWidget *p)
{
    if (d->contains(KData::Array)) {
        UiArrayItemTable * table = new UiArrayItemTable(factory->storage(), p);
        table->setQuantity(d->quantityPtr(), d->contentTypes());
        table->setData(*d);
        grid->addWidget(table, row, 0, 1, 3);

        //add to control and quantity maps
        controlMaps[table] = d;
        quantityMaps[d->quantityPtr()] = table;
        return true;
    }
    return false;
}

QWidget * UiUserInput::createInputControl(const KData * d, int row, QGridLayout * grid, QWidget * p)
{
    Q_UNUSED(d);
    Q_UNUSED(row);
    Q_UNUSED(grid);
    Q_UNUSED(p);

    //how to associate quantity changed

    return 0;
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
                //get data
                KData * d = beg->pointerAt(k);

                //call create widget (overridable)
                QWidget * w = createInputControl(d, k, grid, gb);
                if (w != 0) {
                    controlMaps[w] = d;
                    quantityMaps[d->quantityPtr()] = w;
                    valueMaps[d->quantityPtr()] = d->value();
                    continue;
                }

                //if array, allocate table
                if (createTableInput(d, k, grid, gb))
                    continue;

                //add user input according to the type of quantity
                switch(d->quantity().type)
                {
                case Rad::Text:
                case Rad::NumText:
                    createCaption(d, k, &width0, grid, gb);
                    createLineEdit(d, k, grid, gb);
                    break;
                case Rad::Integer:
                    createCaption(d, k, &width0, grid, gb);
                    createIntInput(d, k, grid, gb);
                    break;
                case Rad::Boolean:
                    createCheckBox(d, k, grid, gb);
                    break;
                case Rad::Real:
                    createCaption(d, k, &width0, grid, gb);
                    createRealInput(d, k, grid, gb);
                    break;
                case Rad::Comment:
                case Rad::LongComment:
                    createComment(d, k, grid, gb);
                    break;
                default:
                    break;
                }
                //create unit
                createUnit(d, k, &width2, grid, gb);
            }

            gb->setLayout(grid);
            vbox->addWidget(gb);
        }
        beg++;
    }

    //resize caption and unit column
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
bool UiUserInput::assignData(KData * d, QWidget * wInput) const
{
    Q_UNUSED(d);
    Q_UNUSED(wInput);
    return false;
}

KData UiUserInput::data(const Quantity * qty) const
{
    QWidget * w = quantityMaps[qty];
    if (w != 0) {
        KData * d = controlMaps[w];
        if (d != 0) {
            KData dout = *d;
            if (assignData(&dout, w)) {
                return dout;
            }
            else if (d->contains(KData::Array)) {
                UiArrayItemTable * table =
                        qobject_cast<UiArrayItemTable *>(w);
                return table->data();
            }
            else {
                return KData(qty, d->contentTypes(), valueMaps[qty]);
            }
        }
    }
    return KData();
}
QVariant UiUserInput::value(const Quantity * qty) const
{
    QWidget * w = quantityMaps[qty];
    if (w != 0) {
        KData * d = controlMaps[w];
        if (d != 0) {
            KData dout = *d;
            if (assignData(&dout, w)) {
                return dout.value();
            }
            else if (d->contains(KData::Array)) {
                return QVariant();
            }
            else {
                return valueMaps[qty];
            }
        }
    }
    return QVariant();
}
bool UiUserInput::isEmpty() const
{
    return controlMaps.isEmpty();
}
bool UiUserInput::validate()
{
    bool result = true;

    //add validation for mandatory, min-max values
    QMap<QWidget *, KData *>::iterator beg = controlMaps.begin();
    QMap<QWidget *, KData *>::iterator end = controlMaps.end();
    while (beg != end) {
        KData * d = beg.value();
        QWidget * wInput = beg.key();
        beg++;

        result = validate(d, wInput) && result;
    }

    return result;
}
bool UiUserInput::validate(KData * d, QWidget * wInput)
{
    Q_UNUSED(d);
    Q_UNUSED(wInput);

    //TODO
    return true;
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

        //call overrideable data setter
        if (assignData(d, wInput))
            continue;

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

        UiArrayItemTable * table = qobject_cast<UiArrayItemTable *>(wInput);
        if (table != 0) {
            KData td = table->data();
            if (td.isValid())
                *d = td;

            continue;
        }
    }
}
