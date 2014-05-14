#include <QListWidgetItem>
#include <QCheckBox>
#include "dialogassessmentbrowser.h"
#include "ui_dialogassessmentbrowser.h"
#include "kassessment.h"
#include "kpluginmanager.h"
#include "kstorage.h"

class CheckBoxItem : public QCheckBox
{
private:
    KAssessment *asPtr;
public:
    CheckBoxItem(KAssessment * aP, QListWidgetItem * item, QWidget * parent)
        : QCheckBox(aP->name(), parent), asPtr(aP)
    {
        QListWidget * lw = item->listWidget();
        if (lw != 0)
            lw->setItemWidget(item, this);
        item->setData(Qt::UserRole, QVariant::fromValue((void*)this));
    }
    inline KAssessment * assessment() const {
        return asPtr;
    }
};

DialogAssessmentBrowser::DialogAssessmentBrowser(const QStringList & excludes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAssessmentBrowser)
{
    ui->setupUi(this);
    loadAllAssessments(excludes);
}

DialogAssessmentBrowser::~DialogAssessmentBrowser()
{
    delete ui;
}
AssessmentList DialogAssessmentBrowser::assessments(QObject * parent) const
{
    //get names
    QStringList names;
    QListWidget * lbItems = ui->lbAssessment;
    for(int k = 0; k < lbItems->count(); k++) {
        QListWidgetItem * item = lbItems->item(k);
        CheckBoxItem * cbi = reinterpret_cast<CheckBoxItem *>(lbItems->itemWidget(item));
        if (cbi != 0 && cbi->isChecked()) {
            names.append(cbi->assessment()->name());
        }
    }

    AssessmentList list = KPluginManager::instance()
            ->storage()->loadAssessments(names, parent);

    return list;
}

void DialogAssessmentBrowser::loadAllAssessments(const QStringList &excludes)
{
    AssessmentList list = KPluginManager::instance()
            ->storage()->loadAssessmentPreviews(excludes, this);
    AssessmentList::const_iterator ait = list.constBegin();
    AssessmentList::const_iterator end = list.constEnd();
    while (ait != end) {
        KAssessment * aP = *(ait);
        QListWidgetItem * item = new QListWidgetItem(ui->lbAssessment);
        CheckBoxItem * chk = new CheckBoxItem(aP, item, ui->lbAssessment);
        connect(chk, SIGNAL(clicked()), this, SLOT(onItemClicked()));

        ait++;
    }
}

void DialogAssessmentBrowser::displayAssessment(KAssessment *aP)
{
    if (aP == 0)
        return;
    ui->inpName->setText(aP->name());
    ui->inpAuthor->setText(aP->author());
    ui->inpDescription->setPlainText(aP->description());
    ui->inpRemark->setPlainText(aP->remark());

    //TODO
    //add document
}

void DialogAssessmentBrowser::on_lbAssessment_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current == 0)
        return;

    QVariant d = current->data(Qt::UserRole);
    if (d.isValid()) {
        CheckBoxItem * cbi = reinterpret_cast<CheckBoxItem *>(d.value<void*>());
        displayAssessment(cbi->assessment());
    }
}
void DialogAssessmentBrowser::onItemClicked()
{
    QCheckBox * sender = qobject_cast<QCheckBox *>(QObject::sender());
    if (sender != 0) {
        CheckBoxItem * cbi = reinterpret_cast<CheckBoxItem *>(sender);
        displayAssessment(cbi->assessment());
    }

}
