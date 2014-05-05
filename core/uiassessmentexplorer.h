#ifndef UIASSESSMENTEXPLORER_H
#define UIASSESSMENTEXPLORER_H

#include <QTreeWidget>
#include "kassessment.h"

class UiAssessmentExplorerPrivate;
class K_CORE_EXPORT UiAssessmentExplorer : public QTreeWidget
{
    Q_OBJECT
public:
    explicit UiAssessmentExplorer(QWidget *parent = 0);
    virtual ~UiAssessmentExplorer();

    void addAssessment(const KAssessment & a);
    void removeAssessment(const KAssessment & a);
    KAssessment currentAssessment() const;
    KModelScene * currentScene() const;
    KModelScene * addScene();
    KModelScene * removeScene(KModelScene * scene);
    
signals:
    
public slots:

private:
    Q_DISABLE_COPY(UiAssessmentExplorer)
    QSharedDataPointer<UiAssessmentExplorerPrivate> dptr;
};

#endif // UIASSESSMENTEXPLORER_H
