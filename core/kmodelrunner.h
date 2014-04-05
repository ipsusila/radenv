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

public slots:

protected:
    void run();

private:
    ModelList _vNodes;
    KCalculationInfo _ci;
};

#endif // KMODELRUNNER_H
