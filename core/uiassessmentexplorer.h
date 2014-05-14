#ifndef UIASSESSMENTEXPLORER_H
#define UIASSESSMENTEXPLORER_H

#include <QTreeWidget>
#include "kassessment.h"

class QGraphicsView;
class UiAssessmentExplorerPrivate;
class RADENV_API UiAssessmentExplorer : public QTreeWidget
{
    Q_OBJECT
public:
    enum { Default = 0, Report, Map };

    explicit UiAssessmentExplorer(QWidget *parent = 0);
    virtual ~UiAssessmentExplorer();

    void addAssessment(KAssessment * aP);
    void addAssessments(const AssessmentList & aList);
    void removeAssessment(KAssessment * aP);
    KAssessment * currentAssessment() const;
    KScenario * currentScenario() const;
    void removeScenario(KScenario * scene);
    void setView(QGraphicsView * view);
    
signals:
    void currentAssessmentChanged(KAssessment * aP);
    void currentScenarioChanged(KScenario * scenario);
    void removingScenario(KScenario * scenario);
    void scenarioSelected(KScenario * scenario);
    void viewChanged(int id, void * data);
protected slots:
    void setDecoration(QTreeWidgetItem * item);
    void handleCurrentItem(QTreeWidgetItem * current, QTreeWidgetItem * previous);
    void displayContextMenu(const QPoint & pos);

public slots:
    void addAssessment();
    void removeAssessment();
    void saveAssessment();
    void saveAssessmentAs();
    void saveAllAssessment();
    void closeAllAssessment();
    void openAssessment();
    void closeAssessment();
    void addScenario();
    void removeScenario();
    void verifyScenario();
    void evaluateScenario();
    void generateReport();    
    void refreshScenario();

    void clearModel();
    void reannotateModel();

    void createModel(IModelFactory * factory, const KModelInfo & info);

protected:
    void addScenario(QTreeWidgetItem * aItem, KScenario * scene);
    void changeScenario(KScenario * scenario);
    void buildMenus();
    QStringList assessmentNames() const;
    KReport * report(KScenario * scenario) const;

private:
    Q_DISABLE_COPY(UiAssessmentExplorer)
    QSharedDataPointer<UiAssessmentExplorerPrivate> dptr;
};

#endif // UIASSESSMENTEXPLORER_H
