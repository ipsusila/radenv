#include <QtGui>
#include "userinput.h"

UserInput::UserInput(KDataGroupArray * ga, QWidget *parent) :
    IUserInput(parent), dataArray(ga)
{
    createControls(ga);
}
UserInput::~UserInput()
{
}

void UserInput::createControls(KDataGroupArray *ga)
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
            for (int k = 0; k < sz; k++) {
                KData * d = beg->pointerAt(k);

                //create controls according to symbol
                QLineEdit * ed;
                QSpinBox * inpInt;
                QCheckBox * chkBox;
                QDoubleSpinBox * inpReal;
                const Symbol & sym = d->symbol();

                //caption
                if (sym.type == Rad::NumText || sym.type == Rad::Integer || sym.type == Rad::Real) {
                    QLabel * label = new QLabel(sym.displayText(), gb);
                    label->setMinimumWidth(170);
                    label->setWordWrap(true);
                    grid->addWidget(label, k, 0);
                }

                //add user input according to the type of symbol
                switch(sym.type)
                {
                case Rad::Text:
                    ed = new QLineEdit(gb);
                    ed->setToolTip(sym.description);
                    ed->setText(d->value().toString());
                    grid->addWidget(ed, k, 1);
                    controlMaps[ed] = d;
                case Rad::NumText:
                    ed = new QLineEdit(gb);
                    ed->setToolTip(sym.description);
                    ed->setText(QString::number(d->numericValue()));
                    grid->addWidget(ed, k, 1);
                    controlMaps[ed] = d;
                    break;
                case Rad::Integer:
                    inpInt = new QSpinBox(gb);
                    inpInt->setToolTip(sym.description);
                    inpInt->setMinimum(sym.minValue);
                    inpInt->setMaximum(sym.maxValue);
                    inpInt->setValue((int)d->numericValue());
                    grid->addWidget(inpInt, k, 1);
                    controlMaps[inpInt] = d;
                    break;
                case Rad::Boolean:
                    chkBox = new QCheckBox(sym.text, gb);
                    chkBox->setToolTip(sym.description);
                    chkBox->setChecked(d->value().toBool());
                    grid->addWidget(chkBox, k, 0, 1, 2);
                    controlMaps[chkBox] = d;
                    break;
                case Rad::Real:
                    inpReal = new QDoubleSpinBox(gb);
                    inpReal->setToolTip(sym.description);
                    inpReal->setMinimum(sym.minValue);
                    inpReal->setMaximum(sym.maxValue);
                    inpReal->setValue(d->numericValue());
                    grid->addWidget(inpReal, k, 1);
                    controlMaps[inpReal] = d;
                    break;
                default:
                    break;
                }

                if (!sym.unit.isEmpty()) {
                    QLabel * unit = new QLabel(sym.unit, gb);
                    unit->setMinimumWidth(50);
                    unit->setMaximumWidth(90);
                    unit->setWordWrap(true);
                    grid->addWidget(unit, k, 2);
                }
            }

            gb->setLayout(grid);
            vbox->addWidget(gb);
        }
        beg++;
    }

    //set main layout
    this->setLayout(vbox);
}

void UserInput::acceptValues()
{
    QMap<QWidget *, KData *>::iterator beg = controlMaps.begin();
    QMap<QWidget *, KData *>::iterator end = controlMaps.end();
    while (beg != end) {
        KData * d = beg.value();
        if (!d->isValid()) {
            beg++;
            continue;
        }

        //is text edit
        QLineEdit * ed = qobject_cast<QLineEdit *>(beg.key());
        if (ed != 0) {
            if (d->symbol().type == Rad::NumText)
                d->setValue(ed->text().toDouble());
            else
                d->setValue(ed->text());
        }
        else {
            QSpinBox * sb = qobject_cast<QSpinBox *>(beg.key());
            if (sb != 0) {
                d->setValue(QVariant(sb->value()));
            }
            else {
                QDoubleSpinBox * dsb = qobject_cast<QDoubleSpinBox *>(beg.key());
                if (dsb != 0) {
                    d->setValue(dsb->value());
                }
                else {
                    QCheckBox * chk = qobject_cast<QCheckBox *>(beg.key());
                    if (chk != 0) {
                        d->setValue(QVariant(chk->isChecked()));
                    }
                }
            }
        }

        beg++;
    }
}
