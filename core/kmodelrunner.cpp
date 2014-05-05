#include "imodel.h"
#include "kmodelrunner.h"
#include "koutput.h"

KModelRunner::KModelRunner(const ModelList * vNodes, const KCalculationInfo & ci) :
    _vNodes(*vNodes), _ci(ci)
{
    connect(this, SIGNAL(calculationRequested(KCalculationInfo,IModel*,bool*)),
            this, SLOT(performCalculation(KCalculationInfo,IModel*,bool*)), Qt::BlockingQueuedConnection);
}
KModelRunner::~KModelRunner()
{
    //destroyed
}

void KModelRunner::stop()
{

}

void KModelRunner::pause()
{

}

void KModelRunner::resume()
{

}

void KModelRunner::performCalculation(const KCalculationInfo & ci, IModel * model, bool * result)
{
    Q_ASSERT(model);
    Q_ASSERT(result);

    *result = model->calculate(ci);
}
bool KModelRunner::requestCalculation(const KCalculationInfo & ci, IModel * model)
{
    //using direct function call or
    //run signal/slot for thread safety
    bool result = false;
    if (ci.isQueuedMode())
        emit calculationRequested(ci, model, &result);
    else
        result = model->calculate(ci);
    return result;
}

void KModelRunner::run()
{
    bool result = true;
    int msec = _ci.intervalMilisecond();
    int runId = _ci.runId();
    int runCnt = _ci.runCount();
    while (runId < runCnt || _ci.isContinuous()) {
        xInfo() << tr("Calculating dose...") << runId;
        foreach(IModel * md, _vNodes) {
            KCalculationInfo mci(md, _ci.intervalMilisecond(), runCnt, runId, _ci.isQueuedMode(), _ci.continueOnError());
            //result = md->run(mci) && result;
            result = requestCalculation(mci, md) && result;
            if (!result && !_ci.continueOnError())
                goto _end_;

            //set calculation results
            mci.setResult(md->result());

            //emit signals
            emit resultReady(mci);
        }

        runId ++;
        if (runId < runCnt && msec > 0)
            this->msleep(msec);
    }

_end_:
    xInfo() << "---";
    if (result) {
        xInfo() << tr("Calculation finished.");
        emit calculationFinished();
    }
    else {
        xError() << tr("Calculation not finished. Some error(s) found");
        emit calculationError();
    }
}
