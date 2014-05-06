#ifndef KCALCULATIONINFO_H
#define KCALCULATIONINFO_H

#include "radglobal.h"

class KCalculationInfoPrivate;
class RADENV_API KCalculationInfo
{
public:
    enum {
        Continuous = 0,
        Once = 1
    };
    explicit KCalculationInfo(int interval, int runCnt, int runId, bool queueMode, bool contOnError);
    explicit KCalculationInfo(IModel * model, int interval, int runCnt, int runId, bool queueMode, bool contOnError);
    KCalculationInfo(const KCalculationInfo& ci);
    KCalculationInfo& operator=(const KCalculationInfo& ci);
    virtual ~KCalculationInfo();

    bool isQueuedMode() const;
    int intervalMilisecond() const;
    bool isContinuous() const;
    int runCount() const;
    bool continueOnError() const;
    const IModel * model() const;
    void setModel(IModel * model);
    int runId() const;
    QDateTime timestamp() const;
    const KDataArray& input() const;
    const KDataArray& inputAt(int idx) const;
    const KDataArray& result() const;
    void setResult(const KDataArray & da);

private:
    QSharedDataPointer<KCalculationInfoPrivate> dptr;
};

#endif // KCALCULATIONINFO_H
