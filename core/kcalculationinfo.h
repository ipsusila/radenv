#ifndef KCALCULATIONINFO_H
#define KCALCULATIONINFO_H

#include <QSharedDataPointer>
#include "radglobal.h"

class KCalculationInfoPrivate;
class K_CORE_EXPORT KCalculationInfo
{
public:
    enum {
        Continuous = 0,
        Once = 1
    };
    explicit KCalculationInfo(int interval, int runCnt, int runId = 0, bool contOnError = false);
    explicit KCalculationInfo(IModel * model, int interval, int runCnt, int runId = 0, bool contOnError = false);
    KCalculationInfo(const KCalculationInfo& ci);
    KCalculationInfo& operator=(const KCalculationInfo& ci);
    virtual ~KCalculationInfo();

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
    QSharedDataPointer<KCalculationInfoPrivate> data;
};

#endif // KCALCULATIONINFO_H
