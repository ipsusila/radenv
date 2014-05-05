#ifndef KMODELRUNNER_H
#define KMODELRUNNER_H

#include "kcalculationinfo.h"
#include <QThread>

class K_CORE_EXPORT KModelRunner : public QThread
{
    Q_OBJECT
public:
    KModelRunner(const ModelList * vNodes, const KCalculationInfo & ci);
    virtual ~KModelRunner();

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

#endif // KMODELRUNNER_H
