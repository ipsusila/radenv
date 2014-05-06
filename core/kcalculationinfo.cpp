#include <QSharedData>
#include "kcalculationinfo.h"
#include "imodel.h"

static const KDataArray __nullInput;
class KCalculationInfoPrivate : public QSharedData
{
public:
    KCalculationInfoPrivate(IModel * m, int msec, int rcnt, int rid, bool q, bool c)
        : model(m), interval(msec), runCount(rcnt), runId(rid), queueMode(q), continueOnError(c),
          timestamp(QDateTime::currentDateTime())
    {
        //now get input and stored to this object
        if (model != 0)
            inputs = model->inputResults();
    }

    inline void setModel(IModel * m) {
        model = m;
        if (model != 0)
            inputs = model->inputResults();
    }

    IModel * model;
    int interval;
    int runCount;
    int runId;
    bool queueMode;
    bool continueOnError;
    QDateTime timestamp;
    DataArrayList inputs;
    KDataArray result;
};

KCalculationInfo::KCalculationInfo(IModel * model, int interval, int runCnt, int runId, bool queueMode, bool contOnError)
    : dptr(new KCalculationInfoPrivate(model, interval, runCnt, runId, queueMode, contOnError))
{
}
KCalculationInfo::KCalculationInfo(int interval, int runCnt, int runId, bool queueMode, bool contOnError)
    : dptr(new KCalculationInfoPrivate(0, interval, runCnt, runId, queueMode, contOnError))
{
}
KCalculationInfo::KCalculationInfo(const KCalculationInfo& ci) : dptr(ci.dptr)
{
}

KCalculationInfo& KCalculationInfo::operator=(const KCalculationInfo& ci)
{
    if (this != &ci) {
        dptr.operator = (ci.dptr);
    }

    return *this;
}

KCalculationInfo::~KCalculationInfo()
{
}

bool KCalculationInfo::isQueuedMode() const
{
    return dptr->queueMode;
}

int KCalculationInfo::intervalMilisecond() const
{
    return dptr->interval;
}

bool KCalculationInfo::isContinuous() const
{
    return (dptr->runCount <= 0);
}
int KCalculationInfo::runCount() const
{
    return dptr->runCount;
}
const IModel * KCalculationInfo::model() const
{
    return dptr->model;
}
void KCalculationInfo::setModel(IModel *model)
{
    dptr->setModel(model);
}

int KCalculationInfo::runId() const
{
    return dptr->runId;
}
QDateTime KCalculationInfo::timestamp() const
{
    return dptr->timestamp;
}
bool KCalculationInfo::continueOnError() const
{
    return dptr->continueOnError;
}

const KDataArray& KCalculationInfo::input() const
{
    if (dptr->inputs.isEmpty())
        return __nullInput;
    return dptr->inputs.first();
}

const KDataArray& KCalculationInfo::inputAt(int idx) const
{
    if (idx < 0 || idx >= dptr->inputs.size())
        return __nullInput;
    return dptr->inputs.at(idx);
}
const KDataArray& KCalculationInfo::result() const
{
    return dptr->result;
}
void KCalculationInfo::setResult(const KDataArray & da)
{
    dptr->result = da;
}

