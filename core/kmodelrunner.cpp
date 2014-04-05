#include "imodel.h"
#include "kmodelrunner.h"
#include "koutput.h"

KModelRunner::KModelRunner(const ModelList * vNodes, const KCalculationInfo & ci) :
    _vNodes(*vNodes), _ci(ci)
{
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

void KModelRunner::run()
{
    bool result = true;
    int msec = _ci.intervalMilisecond();
    int runId = _ci.runId();
    int runCnt = _ci.runCount();
    while (runId < runCnt || _ci.isContinuous()) {
        xInfo() << tr("Calculating dose...") << runId;
        foreach(IModel * md, _vNodes) {
            KCalculationInfo mci(md, _ci.intervalMilisecond(), runCnt, runId, _ci.continueOnError());
            result = md->calculate(mci) && result;
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
