#include <QSharedData>
#include "kcalculationinfo.h"
#include "imodel.h"

static const KDataArray __nullInput;
class KCalculationInfoPrivate : public QSharedData
{
public:
    KCalculationInfoPrivate(IModel * m, int msec, int rcnt, int rid, bool c)
        : model(m), interval(msec), runCount(rcnt), runId(rid), continueOnError(c),
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
    bool continueOnError;
    QDateTime timestamp;
    DataArrayList inputs;
    KDataArray result;
};

KCalculationInfo::KCalculationInfo(IModel * model, int interval, int runCnt, int runId, bool contOnError)
    : data(new KCalculationInfoPrivate(model, interval, runCnt, runId, contOnError))
{
}
KCalculationInfo::KCalculationInfo(int interval, int runCnt, int runId, bool contOnError)
    : data(new KCalculationInfoPrivate(0, interval, runCnt, runId, contOnError))
{
}
KCalculationInfo::KCalculationInfo(const KCalculationInfo& ci) : data(ci.data)
{
}

KCalculationInfo& KCalculationInfo::operator=(const KCalculationInfo& ci)
{
    if (this != &ci) {
        data.operator = (ci.data);
    }

    return *this;
}

KCalculationInfo::~KCalculationInfo()
{
}

int KCalculationInfo::intervalMilisecond() const
{
    return data->interval;
}

bool KCalculationInfo::isContinuous() const
{
    return (data->runCount <= 0);
}
int KCalculationInfo::runCount() const
{
    return data->runCount;
}
const IModel * KCalculationInfo::model() const
{
    return data->model;
}
void KCalculationInfo::setModel(IModel *model)
{
    data->setModel(model);
}

int KCalculationInfo::runId() const
{
    return data->runId;
}
QDateTime KCalculationInfo::timestamp() const
{
    return data->timestamp;
}
bool KCalculationInfo::continueOnError() const
{
    return data->continueOnError;
}

const KDataArray& KCalculationInfo::input() const
{
    if (data->inputs.isEmpty())
        return __nullInput;
    return data->inputs.first();
}

const KDataArray& KCalculationInfo::inputAt(int idx) const
{
    if (idx < 0 || idx >= data->inputs.size())
        return __nullInput;
    return data->inputs.at(idx);
}
const KDataArray& KCalculationInfo::result() const
{
    return data->result;
}
void KCalculationInfo::setResult(const KDataArray & da)
{
    data->result = da;
}

