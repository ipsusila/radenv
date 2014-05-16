#include <QSharedData>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QGraphicsView>
#include <QMenu>
#include <QMessageBox>
#include "uiassessmentexplorer.h"
#include "kscenario.h"
#include "dialogscenario.h"
#include "dialogassessment.h"
#include "dialogassessmentbrowser.h"
#include "kapplication.h"
#include "kstorage.h"
#include "koutput.h"
#include "khtmlreport.h"

/**
 * @brief The ExplorerItem class (internal use)
 */
class ExplorerItem : public QTreeWidgetItem
{
public:
    enum ExplorerItemType { Root = 1, Assessment, Scenario, Reports, ReportItem, Log, Result, Document, Map };
private:
    KAssessment * _assessment;
    KScenario * _scenario;
    KReport * _report;
    ExplorerItemType _type;

public:
    ExplorerItem(const QString & str, QTreeWidget * parent)
        : QTreeWidgetItem(parent), _assessment(0), _scenario(0), _report(0), _type(Root)
    {
        setText(0, str);
    }
    ExplorerItem(KAssessment * aP, QTreeWidgetItem * pItem)
        : QTreeWidgetItem(pItem), _assessment(aP), _scenario(0), _report(0), _type(Assessment)
    {
        setText(0, aP->name());
    }
    ExplorerItem(KScenario * scP, QTreeWidgetItem * pItem)
        : QTreeWidgetItem(pItem), _assessment(scP->assessment()), _scenario(scP), _report(0), _type(Scenario)
    {
        setText(0, scP->name());
    }
    ExplorerItem(const QString & str, KScenario * scP, ExplorerItemType t, QTreeWidgetItem * pItem)
        : QTreeWidgetItem(pItem), _assessment(scP->assessment()), _scenario(scP), _report(0), _type(t)
    {
        setText(0, str);
    }
    ExplorerItem(const QString & str, KReport * rep, KScenario * scP, QTreeWidgetItem * pItem)
        : QTreeWidgetItem(pItem), _assessment(scP->assessment()), _scenario(scP), _report(rep), _type(Reports)
    {
        setText(0, str);
    }
    ~ExplorerItem() {
        qDebug() << "##Item removed --> " << this->text(0);
        if (_report != 0)
            delete _report;
    }

    inline KAssessment * assessment() const {
        return _assessment;
    }
    inline KScenario * scenario() const {
        return _scenario;
    }
    inline KReport * report() const {
        return _report;
    }
    inline ExplorerItemType itemType() const {
        return _type;
    }
    inline bool isRootItem() const {
        return (_type == Root && _assessment == 0);
    }
    inline bool isAssessmentItem() const {
        return (_type == Assessment && _assessment != 0);
    }
    inline bool isScenarioItem() const {
        return (_type == Scenario && _scenario != 0);
    }
    inline bool isScenarioSubItem() const {
        return (_assessment != 0 && _scenario != 0 && _type != Scenario);
    }
};

class UiAssessmentExplorerPrivate : public QSharedData
{
    ExplorerItem * _root;
    KAssessment * _curAssessment;
    KScenario * _curScenario;

    QMenu * _rootMenu;
    QMenu * _assessmentMenu;
    QMenu * _scenarioMenu;
public:
    UiAssessmentExplorerPrivate() : _root(0), _curAssessment(0),
        _curScenario(0), _rootMenu(0), _assessmentMenu(0), _scenarioMenu(0) {}
    inline ExplorerItem * root(QTreeWidget * parent)
    {
        if (_root == 0) {
            _root = new ExplorerItem(QObject::tr("List of Assessment(s)"), parent);
            parent->addTopLevelItem(_root);
            _root->setExpanded(true);
        }
        return _root;
    }
    inline ExplorerItem * root() const {
        return _root;
    }
    inline void setCurrentAssessment(KAssessment * aP) {
        _curAssessment = aP;
    }
    inline KAssessment * currentAssessment() const {
        return _curAssessment;
    }
    inline bool setCurrentScenario(KScenario * sP) {
        if (_curScenario != sP) {
            _curScenario = sP;
            return true;
        }
        return false;
    }
    inline KScenario * currentScenario() const {
        return _curScenario;
    }
    inline void clearCurrentAssessment(KAssessment * aP) {
        if (_curAssessment == aP) {
            _curAssessment = 0;
            if (aP->contains(_curScenario))
                _curScenario = 0;
        }
    }

    inline void setMenu(QMenu * rMenu, QMenu * aMenu, QMenu * sMenu)
    {
        _rootMenu = rMenu;
        _assessmentMenu = aMenu;
        _scenarioMenu = sMenu;
    }

    inline QMenu * assessmentMenu() const {
        return _assessmentMenu;
    }
    inline void setAssessmentMenu(QMenu * menu) {
        _assessmentMenu = menu;
    }
    inline QMenu * scenarioMenu() const {
        return _scenarioMenu;
    }
    inline void setScenarioMenu(QMenu * menu) {
        _scenarioMenu = menu;
    }
    inline QMenu * rootMenu() const {
        return _rootMenu;
    }
    inline void setRootMenu(QMenu * menu) {
        _rootMenu = menu;
    }
    inline QMenu * contextMenu(QTreeWidgetItem * tItem) {
        ExplorerItem * item = reinterpret_cast<ExplorerItem *>(tItem);
        if (item != 0) {
            if (item->isRootItem())
                return _rootMenu;
            else if (item->isAssessmentItem())
                return _assessmentMenu;
            else if (item->isScenarioItem())
                return _scenarioMenu;
        }
        return 0;
    }
};

UiAssessmentExplorer::UiAssessmentExplorer(QWidget *parent) :
    QTreeWidget(parent), dptr(new UiAssessmentExplorerPrivate)
{
    setColumnCount(1);
    setIconSize(QSize(12,12));
    setHeaderHidden(true);
    setDecoration(dptr->root(this));
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(setDecoration(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(setDecoration(QTreeWidgetItem*)));
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this,
            SLOT(handleCurrentItem(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayContextMenu(QPoint)));

    buildMenus();
}
UiAssessmentExplorer::~UiAssessmentExplorer()
{

}

void UiAssessmentExplorer::buildMenus()
{
    /*
    void addAssessment();
    void openAssessment();
    void saveAllAssessment();
    void closeAllAssessment();

    void removeAssessment();
    void saveAssessment();
    void closeAssessment();
    void addScenario();

    void removeScenario();
    void verifyScenario();
    void evaluateScenario();
    void generateReport();
    */

    QMenu * rootMenu = new QMenu(this);
    QAction * actAddAssessment = new QAction(tr("New Assessment..."), rootMenu);
    connect(actAddAssessment, SIGNAL(triggered()), this, SLOT(addAssessment()));
    rootMenu->addAction(actAddAssessment);

    QAction * actOpenAssessment = new QAction(tr("Open Assessment..."), rootMenu);
    connect(actOpenAssessment, SIGNAL(triggered()), this, SLOT(openAssessment()));
    rootMenu->addAction(actOpenAssessment);
    rootMenu->addSeparator();

    QAction * actSaveAllAssessment = new QAction(tr("Save All"), rootMenu);
    connect(actSaveAllAssessment, SIGNAL(triggered()), this, SLOT(saveAllAssessment()));
    rootMenu->addAction(actSaveAllAssessment);
    rootMenu->addSeparator();

    QAction * actCloseAllAssessment = new QAction(tr("Close All..."), rootMenu);
    connect(actCloseAllAssessment, SIGNAL(triggered()), this, SLOT(closeAllAssessment()));
    rootMenu->addAction(actCloseAllAssessment);

    QMenu * asMenu = new QMenu(this);
    QAction * actAddScenario = new QAction(tr("Add Scenario..."), asMenu);
    connect(actAddScenario, SIGNAL(triggered()), this, SLOT(addScenario()));
    asMenu->addAction(actAddScenario);
    asMenu->addSeparator();

    QAction * actSaveAssessment = new QAction(tr("Save Assessment"), asMenu);
    connect(actSaveAssessment, SIGNAL(triggered()), this, SLOT(saveAssessment()));
    asMenu->addAction(actOpenAssessment);
    asMenu->addSeparator();
    asMenu->addAction(actSaveAssessment);

    QAction * actSaveAssessmentAs = new QAction(tr("Save Assessment As..."), asMenu);
    connect(actSaveAssessmentAs, SIGNAL(triggered()), this, SLOT(saveAssessmentAs()));
    asMenu->addAction(actSaveAssessmentAs);
    asMenu->addAction(actSaveAllAssessment);
    asMenu->addSeparator();

    QAction * actCloseAssessment = new QAction(tr("Close Assessment"), asMenu);
    connect(actCloseAssessment, SIGNAL(triggered()), this, SLOT(closeAssessment()));
    asMenu->addAction(actCloseAssessment);
    asMenu->addAction(actCloseAllAssessment);
    asMenu->addSeparator();

    QAction * actRemoveAssessment = new QAction(tr("Remove Assessment..."), asMenu);
    connect(actRemoveAssessment, SIGNAL(triggered()), this, SLOT(removeAssessment()));
    asMenu->addAction(actRemoveAssessment);

    QMenu * scMenu = new QMenu(this);
    QAction * actVerifyScenario = new QAction(tr("Verify Scenario"), scMenu);
    connect(actVerifyScenario, SIGNAL(triggered()), this, SLOT(verifyScenario()));
    scMenu->addAction(actVerifyScenario);

    QAction * actEvalScenario = new QAction(tr("Evaluate..."), scMenu);
    connect(actEvalScenario, SIGNAL(triggered()), this, SLOT(evaluateScenario()));
    scMenu->addAction(actEvalScenario);

    QAction * actGenReport = new QAction(tr("Generate Report"), scMenu);
    connect(actGenReport, SIGNAL(triggered()), this, SLOT(generateReport()));
    scMenu->addAction(actGenReport);
    scMenu->addSeparator();

    QAction * actCopyScenario = new QAction(tr("Copy Scenario"), scMenu);
    connect(actCopyScenario, SIGNAL(triggered()), this, SLOT(copyScenario()));
    scMenu->addAction(actCopyScenario);

    QAction * actEditScenario = new QAction(tr("Edit Scenario"), scMenu);
    connect(actEditScenario, SIGNAL(triggered()), this, SLOT(editScenario()));
    scMenu->addAction(actEditScenario);
    scMenu->addSeparator();

    QAction * actRemoveScenario = new QAction(tr("Remove Scenario..."), scMenu);
    connect(actRemoveScenario, SIGNAL(triggered()), this, SLOT(removeScenario()));
    scMenu->addAction(actRemoveScenario);

    dptr->setMenu(rootMenu, asMenu, scMenu);
}

QStringList UiAssessmentExplorer::assessmentNames() const
{
    QStringList names;
    ExplorerItem * root = dptr->root();
    if (root != 0) {
        for(int k = 0; k < root->childCount(); k++) {
            ExplorerItem * child = reinterpret_cast<ExplorerItem *>(root->child(k));
            if (child->isAssessmentItem())
                names.append(child->assessment()->name());
        }
    }
    return names;
}

void UiAssessmentExplorer::setDecoration(QTreeWidgetItem * item)
{
    Q_ASSERT(item != 0);
    ExplorerItem * eItem = reinterpret_cast<ExplorerItem *>(item);
    if (eItem->isRootItem()) {
        if (eItem->isExpanded())
            eItem->setIcon(0, QIcon(":/core/root-expanded.png"));
        else
            eItem->setIcon(0, QIcon(":/core/root-collapsed.png"));
    }
    else if (eItem->isAssessmentItem()) {
        if (eItem->isExpanded())
            eItem->setIcon(0, QIcon(":/core/assessment-expanded.png"));
        else
            eItem->setIcon(0, QIcon(":/core/assessment-collapsed.png"));
    }
    else if (eItem->isScenarioItem()) {
        if (eItem->isExpanded())
            eItem->setIcon(0, QIcon(":/core/scenario-expanded.png"));
        else
            eItem->setIcon(0, QIcon(":/core/scenario-collapsed.png"));
    }
}
void UiAssessmentExplorer::handleCurrentItem(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    if (current != 0) {
        ExplorerItem * cItem = reinterpret_cast<ExplorerItem *>(current);

        //assessment
        KAssessment * aP = cItem->assessment();
        if (aP != 0 && dptr->currentAssessment() != aP) {
            dptr->setCurrentAssessment(aP);
            emit currentAssessmentChanged(aP);

            //get first scenario
            changeScenario(aP->firstScenario());

            qDebug() << "Assessment changed";
        }

        //assign scenario
        KScenario * scenario = cItem->scenario();
        if (scenario != 0) {
            if (cItem->isScenarioItem()) {
                emit scenarioSelected(scenario);
                qDebug() << "Scenario selected";
            }
            changeScenario(scenario);
        }

        qDebug() << "Item : " << cItem->text(0) << ",rep: " << cItem->report();
        if (cItem->itemType() == ExplorerItem::Reports)
            emit viewChanged(UiAssessmentExplorer::Report, cItem->report());
        else if (cItem->itemType() == ExplorerItem::Map)
            emit viewChanged(UiAssessmentExplorer::Map, cItem->scenario());
        else
            emit viewChanged(UiAssessmentExplorer::Default, cItem->scenario());
    }
    else
        qDebug() << "@Current item is NULL, prev: " << previous;
}

void UiAssessmentExplorer::addAssessment(KAssessment *aP)
{
    Q_ASSERT(aP != 0);

    //setup parent
    aP->setParent(this);

    //add item
    QTreeWidgetItem * root = dptr->root(this);
    ExplorerItem * aItem = new ExplorerItem(aP, root);
    setDecoration(aItem);

    //now adding scenarios
    ScenarioList cases = aP->scenarios();
    ScenarioList::const_iterator cit = cases.constBegin();
    ScenarioList::const_iterator end = cases.constEnd();
    while(cit != end) {
        addScenario(aItem, *cit);
        cit++;
    }
}
void UiAssessmentExplorer::addAssessments(const AssessmentList & aList)
{
    AssessmentList::const_iterator ait = aList.constBegin();
    AssessmentList::const_iterator end = aList.constEnd();
    while(ait != end) {
        addAssessment(*ait);
        ait++;
    }
}

void UiAssessmentExplorer::removeAssessment(KAssessment *aP)
{
    if (aP == 0)
        return;

    //remove parent/child relations
    aP->setParent(0);

    //remove items
    QTreeWidgetItem * root = dptr->root(this);
    for(int k = 0; k < root->childCount(); k++) {
        ExplorerItem * child = reinterpret_cast<ExplorerItem*>(root->child(k));
        if (child->isAssessmentItem() && child->assessment() == aP) {
            root->removeChild(child);
            break;
        }
    }

    //remove assessment
    dptr->clearCurrentAssessment(aP);
    ScenarioList scenarios = aP->scenarios();
    for(int k = 0; k < scenarios.size(); k++)
        this->removeScenario(scenarios.at(k));

    delete aP;
}

KAssessment * UiAssessmentExplorer::currentAssessment() const
{
    return dptr->currentAssessment();
}
KScenario * UiAssessmentExplorer::currentScenario() const
{
    return dptr->currentScenario();
}
void UiAssessmentExplorer::changeScenario(KScenario * scenario)
{
    if (dptr->setCurrentScenario(scenario)) {
        emit currentScenarioChanged(scenario);
        qDebug() << "Scenario changed";
    }
}
void UiAssessmentExplorer::addScenario(KScenario * scenario)
{
    //search for assessment item
    ExplorerItem * root = dptr->root();
    for(int k = 0; k < root->childCount(); k++) {
        ExplorerItem * asItem = reinterpret_cast<ExplorerItem *>(root->child(k));
        if (asItem->assessment() == scenario->assessment()) {
            addScenario(asItem, scenario);
            break;
        }
    }
}

void UiAssessmentExplorer::addScenario(QTreeWidgetItem *aItem, KScenario *scene)
{
    /*
    static QString subItems[] = {
        tr("Result Views"), tr("Reports"), tr("Maps"), tr("Execution Logs"), tr("Additional Documents")
    };
    static ExplorerItem::ExplorerItemType subTypes[] = {
        ExplorerItem::Result, ExplorerItem::Reports, ExplorerItem::Map, ExplorerItem::Log, ExplorerItem::Document
    };
    */
    static QString subItems[] = {
        tr("Reports"), tr("Maps")
    };
    static ExplorerItem::ExplorerItemType subTypes[] = {
        ExplorerItem::Reports, ExplorerItem::Map
    };

    ExplorerItem * scItem = new ExplorerItem(scene, aItem);
    setDecoration(scItem);

    //subitems
    int nz = sizeof(subItems)/sizeof(QString);
    for(int k = 0; k < nz; k++) {
        ExplorerItem * subItem;
        if ((int)subTypes[k] == ExplorerItem::Reports)
            subItem = new ExplorerItem(subItems[k], new KHtmlReport(), scene, scItem);
        else
            subItem = new ExplorerItem(subItems[k], scene, subTypes[k], scItem);
        setDecoration(subItem);
    }
}

void UiAssessmentExplorer::removeScenario(KScenario * scene)
{
    emit removingScenario(scene);

    //remove scenario
    KAssessment * aP = scene->assessment();
    if (aP != 0)
        aP->remove(scene);
}
void UiAssessmentExplorer::displayContextMenu(const QPoint & pos)
{
    qDebug() << "Menu requested at" << pos;
    QMenu * menu = dptr->contextMenu(itemAt(pos));
    if (menu != 0) {
        menu->popup(mapToGlobal(pos));
        qDebug() << "Pop up menu: " << menu;
    }
}

QTreeWidgetItem * UiAssessmentExplorer::assessmentItem(KAssessment *aP, QTreeWidgetItem * parent) const
{
    if (parent == 0)
        parent = dptr->root();

    for(int k = 0; k < parent->childCount(); k++) {
        ExplorerItem * child = reinterpret_cast<ExplorerItem *>(parent->child(k));
        if (child->isAssessmentItem() && child->assessment() == aP) {
            return child;
        }
        else {
            QTreeWidgetItem * aItem = assessmentItem(aP, child);
            if (aItem != 0)
                return aItem;
        }
    }
    return 0;
}


QTreeWidgetItem * UiAssessmentExplorer::scenarioItem(KScenario * scenario, QTreeWidgetItem * parent) const
{
    if (parent == 0)
        parent = dptr->root();

    for(int k = 0; k < parent->childCount(); k++) {
        ExplorerItem * child = reinterpret_cast<ExplorerItem *>(parent->child(k));
        if (child->isScenarioItem() && child->scenario() == scenario) {
            return child;
        }
        else {
            QTreeWidgetItem * scItem = scenarioItem(scenario, child);
            if (scItem != 0)
                return scItem;
        }
    }
    return 0;
}

QTreeWidgetItem * UiAssessmentExplorer::reportItem(KScenario * scenario,
                                                  QTreeWidgetItem *parent) const
{
    if (parent == 0)
        parent = dptr->root();

    for(int k = 0; k < parent->childCount(); k++) {
        ExplorerItem * child = reinterpret_cast<ExplorerItem *>(parent->child(k));
        if (child->scenario() == scenario && child->report() != 0) {
            return child;
        }
        else {
            QTreeWidgetItem * repItem = reportItem(scenario, child);
            if (repItem != 0)
                return repItem;
        }
    }
    return 0;
}

void UiAssessmentExplorer::addAssessment()
{
    if (!KApplication::selfInstance()->hasStorage())
        return;

    //add new assessment
    DialogAssessment dlg;
    dlg.setWindowTitle(tr("Add New Assessment"));
    if (dlg.exec() == QDialog::Accepted) {
        addAssessment(dlg.assessment(this));
    }
}

void UiAssessmentExplorer::removeAssessment()
{
    KAssessment * aP;
    ExplorerItem * cItem = reinterpret_cast<ExplorerItem *>(currentItem());
    if (cItem != 0 && (aP = cItem->assessment()) != 0) {
        //ask
        int ret = QMessageBox::question(this, tr("Remove Assessment?"),
            tr("Do you really want to remove Assessment: ") + aP->name() + " from database?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            //remove from storage
            KApplication::selfInstance()->storage()->remove(*aP);

            //delete item
            delete cItem;
            removeAssessment(aP);
        }
    }
}

void UiAssessmentExplorer::saveAssessment()
{
    KAssessment * aP;
    ExplorerItem * cItem = reinterpret_cast<ExplorerItem *>(currentItem());
    if (cItem != 0 && (aP = cItem->assessment()) != 0) {
        //save assessment
        KApplication::selfInstance()->storage()->save(*aP);
    }
}
void UiAssessmentExplorer::saveAssessmentAs()
{
    KAssessment * curAp = dptr->currentAssessment();
    if (curAp != 0) {
        DialogAssessment dlg(curAp);
        dlg.setWindowTitle(tr("Save Assessment As..."));
        if (dlg.exec() == QDialog::Accepted) {
            KAssessment * aP = dlg.assessment(this);
            if (aP != 0) {
                KApplication::selfInstance()->storage()->save(*aP);
                QTreeWidgetItem * aItem = assessmentItem(aP);
                if (aItem)
                    aItem->setText(0, aP->name());
                /*
                ExplorerItem * root = dptr->root();
                for(int k = 0; k < root->childCount(); k++) {
                    ExplorerItem * item = reinterpret_cast<ExplorerItem *>(root->child(k));
                    if (item->assessment() == aP) {
                        item->setText(0, aP->name());
                        break;
                    }
                }
                */
            }
        }
    }
}

void UiAssessmentExplorer::saveAllAssessment()
{
    ExplorerItem * root = dptr->root(this);
    for(int k = 0; k < root->childCount(); k++) {
        ExplorerItem * child = reinterpret_cast<ExplorerItem *>(root->child(k));
        KAssessment * aP = child->assessment();
        if (aP != 0) {
            //save assessment
            xTrace() << "Saving assessment " << aP->name();
            KApplication::selfInstance()->storage()->save(*aP);
        }
    }
}
void UiAssessmentExplorer::closeAllAssessment()
{
    //close all assessments
    ExplorerItem * root = dptr->root(this);
    if (root->childCount() == 0)
        return;

    int ret = QMessageBox::question(this, tr("Close Assessment?"),
        tr("Do you really want to close all assessment (s)?"), QMessageBox::Yes, QMessageBox::No);
    if (ret != QMessageBox::Yes)
        return;

    while(root->childCount() > 0) {
        ExplorerItem * child = reinterpret_cast<ExplorerItem *>(root->child(0));
        KAssessment * aP = child->assessment();
        if (aP != 0) {
            delete child;
            removeAssessment(aP);
        }
    }
}

void UiAssessmentExplorer::openAssessment()
{
    if (KApplication::selfInstance()->hasStorage()) {
        QStringList excludes = assessmentNames();
        DialogAssessmentBrowser dlg(excludes);
        if (dlg.exec() == QDialog::Accepted) {
            this->addAssessments(dlg.assessments(this));
        }
    }
}

void UiAssessmentExplorer::closeAssessment()
{
    KAssessment * aP;
    ExplorerItem * cItem = reinterpret_cast<ExplorerItem *>(currentItem());
    if (cItem != 0 && (aP = cItem->assessment()) != 0) {
        delete cItem;
        removeAssessment(aP);
    }
}

void UiAssessmentExplorer::addScenario()
{
    KAssessment * aP = dptr->currentAssessment();
    if (aP != 0) {
        DialogScenario dlg(aP);
        dlg.setWindowTitle(tr("Add New Scenario"));
        if (dlg.exec() == QDialog::Accepted) {
            KScenario * scP = dlg.scenario();
            addScenario(scP);
        }
    }
}
void UiAssessmentExplorer::editScenario()
{
    KScenario * scP = dptr->currentScenario();
    if (scP != 0) {
        DialogScenario dlg(scP);
        dlg.setWindowTitle(tr("Edit Scenario ") + scP->name());
        if (dlg.exec() == QDialog::Accepted) {
            QTreeWidgetItem * scItem = scenarioItem(scP);
            if (scItem != 0)
                scItem->setText(0, scP->name());
        }
    }
}

void UiAssessmentExplorer::removeScenario()
{
    ExplorerItem * sItem = reinterpret_cast<ExplorerItem *>(currentItem());
    if (sItem != 0) {
        KScenario * scP = sItem->scenario();
        if (scP != 0) {
            //ask
            int ret = QMessageBox::question(this, tr("Remove Scenario?"),
                tr("Do you really want to remove scenario: ") + scP->name() + "?", QMessageBox::Yes, QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                delete sItem;
                removeScenario(scP);
            }
        }
    }
}

void UiAssessmentExplorer::verifyScenario()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0)
        curScenario->verify();
}

void UiAssessmentExplorer::evaluateScenario()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0)
        curScenario->evaluate();
}

void UiAssessmentExplorer::generateReport()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0) {
        ExplorerItem * repItem =
                reinterpret_cast<ExplorerItem*>(reportItem(curScenario));
        if (repItem == 0)
            return;
        KReport * rep = repItem->report();
        if (rep != 0)
            curScenario->setReport(rep);
        curScenario->generateReport();
        setCurrentItem(repItem);
    }
}
void UiAssessmentExplorer::refreshScenario()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0)
        curScenario->refresh();
}
void UiAssessmentExplorer::copyScenario()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0) {
        //get scenarioname
        KAssessment * aP = curScenario->assessment();
        KScenario * copyScenario = aP->createScenario(curScenario->name());
        curScenario->copyModelsTo(copyScenario, true);
        copyScenario->setDescription(curScenario->description());
        addScenario(copyScenario);
    }
}

void UiAssessmentExplorer::createModel(IModelFactory * factory, const KModelInfo & info)
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0)
        curScenario->createModel(factory, info);
}

void UiAssessmentExplorer::clearModel()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario == 0 || curScenario->items().isEmpty())
        return;

    //ask
    int ret = QMessageBox::question(this, tr("Clear model(s)?"),
        tr("Remove all models from scene?"), QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::Yes)
        curScenario->clearModels();
}

void UiAssessmentExplorer::reannotateModel()
{
    KScenario * curScenario = dptr->currentScenario();
    if (curScenario != 0)
        curScenario->reannotateModels();
}

