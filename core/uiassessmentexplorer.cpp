#include <QSharedData>
#include <QTreeWidgetItem>
#include <QDebug>
#include "uiassessmentexplorer.h"
#include "kmodelscene.h"

namespace Aexp {
    enum ItemType { Root, Assessment, Case, Reports, ReportItem, Log, Result, Document, Map };
    typedef struct _tagItemData
    {
        ItemType type;
        void * data;
        KAssessment assessment;

        inline _tagItemData() : type(Root), data(0) {}
        inline _tagItemData(ItemType t) : type(t), data(0) {}
        inline _tagItemData(ItemType t, void * d) : type(t), data(d) {}
        inline _tagItemData(ItemType t, const KAssessment & a) : type(t), data(0), assessment(a) {}

        inline KModelScene * caseScene() const
        {
            if (type != Case || data == 0) return 0;
            return reinterpret_cast<KModelScene *>(data);
        }
        inline bool isAssessment() const
        {
            return type == Assessment;
        }
        inline bool isCaseScene() const
        {
            return type == Case;
        }
        inline bool isReportItem() const
        {
            return type == ReportItem;
        }
    } ItemData;
}
Q_DECLARE_METATYPE(Aexp::ItemData)

class UiAssessmentExplorerPrivate : public QSharedData
{
    QTreeWidgetItem * _root;
public:
    UiAssessmentExplorerPrivate() : _root(0) {}
    inline QTreeWidgetItem * root(QTreeWidget * parent)
    {
        if (_root == 0) {
            _root = new QTreeWidgetItem(parent);
            _root->setText(0, QObject::tr("List of Assessment(s)"));
            _root->setData(0, Qt::UserRole, QVariant());
            parent->addTopLevelItem(_root);
        }
        return _root;
    }
    inline bool isRoot(QTreeWidgetItem * item) const
    {
        return _root == item;
    }
};

UiAssessmentExplorer::UiAssessmentExplorer(QWidget *parent) :
    QTreeWidget(parent), dptr(new UiAssessmentExplorerPrivate)
{
    setColumnCount(1);
    setIconSize(QSize(12,12));
    setHeaderHidden(true);
    setDecoration(dptr->root(this));

    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(setDecoration(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(setDecoration(QTreeWidgetItem*)));
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this,
            SLOT(handleCurrentItem(QTreeWidgetItem*,QTreeWidgetItem*)));

}
UiAssessmentExplorer::~UiAssessmentExplorer()
{

}

void UiAssessmentExplorer::setDecoration(QTreeWidgetItem * item)
{
    Q_ASSERT(item != 0);
    if (dptr->isRoot(item)) {
        if (item->isExpanded())
            item->setIcon(0, QIcon(":/core/assessment-expanded.png"));
        else
            item->setIcon(0, QIcon(":/core/assessment-collapsed.png"));
    }
    else {

    }
}
void UiAssessmentExplorer::handleCurrentItem(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    if (current != 0) {
        QVariant v = current->data(0, Qt::UserRole);
        qDebug() << "@Item: " << current->text(0) << ", data: " << v;
    }
    else
        qDebug() << "@Current item is NULL, prev: " << previous;
}

void UiAssessmentExplorer::addAssessment(const KAssessment & a)
{
    QTreeWidgetItem * root = dptr->root(this);
    QTreeWidgetItem * aItem = new QTreeWidgetItem(root);
    aItem->setText(0, a.name());
    setDecoration(aItem);

    //set data
    Aexp::ItemData d(Aexp::Assessment, a);
    QVariant vd;
    vd.setValue(d);
    aItem->setData(0, Qt::UserRole, vd);

    //now adding case (scene)
    SceneList cases = a.scenes();
    SceneList::const_iterator cit = cases.constBegin();
    SceneList::const_iterator end = cases.constEnd();
    while(cit != end) {
        addScene(aItem, *cit);
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

void UiAssessmentExplorer::removeAssessment(const KAssessment & a)
{

}

KAssessment UiAssessmentExplorer::currentAssessment() const
{
    return KAssessment();
}
KModelScene * UiAssessmentExplorer::currentScene() const
{
    return 0;
}
KModelScene * UiAssessmentExplorer::addScene()
{
    return 0;
}
void UiAssessmentExplorer::addScene(QTreeWidgetItem *aItem, KModelScene *scene)
{
    static QString subItems[] = {
        tr("Result Views"), tr("Reports"), tr("Maps"), tr("Execution Logs"), tr("Additional Documents")
    };
    static Aexp::ItemType subTypes[] = {
        Aexp::Result, Aexp::Reports, Aexp::Map, Aexp::Log, Aexp::Document
    };

    QTreeWidgetItem * cItem = new QTreeWidgetItem(aItem);
    cItem->setText(0, scene->sceneName());
    setDecoration(cItem);

    //set data
    Aexp::ItemData sd(Aexp::Case, scene);
    QVariant sceneData;
    sceneData.setValue(sd);
    cItem->setData(0, Qt::UserRole, sceneData);

    //subitems
    int nz = sizeof(subItems)/sizeof(QString);
    for(int k = 0; k < nz; k++) {
        QTreeWidgetItem * sItem = new QTreeWidgetItem(cItem);
        sItem->setText(0, subItems[k]);
        setDecoration(sItem);

        //set data
        Aexp::ItemData ssd(subTypes[k]);
        QVariant ssvd;
        ssvd.setValue(ssd);
        sItem->setData(0, Qt::UserRole, ssvd);
    }
}

KModelScene * UiAssessmentExplorer::removeScene(KModelScene * scene)
{
    return scene;
}
