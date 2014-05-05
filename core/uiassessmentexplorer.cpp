#include <QSharedData>
#include "uiassessmentexplorer.h"

class UiAssessmentExplorerPrivate : public QSharedData
{

};

UiAssessmentExplorer::UiAssessmentExplorer(QWidget *parent) :
    QTreeWidget(parent), dptr(new UiAssessmentExplorerPrivate)
{
}
UiAssessmentExplorer::~UiAssessmentExplorer()
{

}

void UiAssessmentExplorer::addAssessment(const KAssessment & a)
{

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

KModelScene * UiAssessmentExplorer::removeScene(KModelScene * scene)
{
    return scene;
}
