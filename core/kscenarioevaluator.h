#ifndef KSCENARIOEVALUATOR_H
#define KSCENARIOEVALUATOR_H

#include "kcalculationinfo.h"
#include <QThread>

class RADENV_API KScenarioEvaluator : public QThread
{
    Q_OBJECT
public:
    KScenarioEvaluator(const ModelList * vNodes, const KCalculationInfo & ci);
    virtual ~KScenarioEvaluator();

    void stop();
    void pause();
    void resume();

signals:
    void resultReady(const KCalculationInfo & ci);
    void calculationError();
    void calculationFinished();
    void calculationRequested(const KCalculationInfo & ci, IModel * model, bool * result);

public slots:
    void performCalculation(const KCalculationInfo & ci, IModel * model, bool * result);
protected:
    void run();
    bool requestCalculation(const KCalculationInfo & ci, IModel * model);

private:
    ModelList _vNodes;
    KCalculationInfo _ci;
};

#endif // KSCENARIOEVALUATOR_H
