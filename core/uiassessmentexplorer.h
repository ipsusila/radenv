#ifndef UIASSESSMENTEXPLORER_H
#define UIASSESSMENTEXPLORER_H

#include <QTreeWidget>
#include "kassessment.h"

class UiAssessmentExplorerPrivate;
class RADENV_API UiAssessmentExplorer : public QTreeWidget
{
    Q_OBJECT
public:
    explicit UiAssessmentExplorer(QWidget *parent = 0);
    virtual ~UiAssessmentExplorer();

    void addAssessment(const KAssessment & a);
    void addAssessments(const AssessmentList & aList);
    void removeAssessment(const KAssessment & a);
    KAssessment currentAssessment() const;
    KModelScene * currentScene() const;
    KModelScene * addScene();
    KModelScene * removeScene(KModelScene * scene);
    
signals:
    
protected slots:
    void setDecoration(QTreeWidgetItem * item);
    void handleCurrentItem(QTreeWidgetItem * current, QTreeWidgetItem * previous);

protected:
    void addScene(QTreeWidgetItem * aItem, KModelScene * scene);

private:
    Q_DISABLE_COPY(UiAssessmentExplorer)
    QSharedDataPointer<UiAssessmentExplorerPrivate> dptr;
};

#endif // UIASSESSMENTEXPLORER_H
