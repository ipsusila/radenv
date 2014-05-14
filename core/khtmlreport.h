#ifndef KHTMLREPORT_H
#define KHTMLREPORT_H

#include "kreport.h"

class RADENV_API KHtmlReport : public KReport
{
public:
    KHtmlReport();
    ~KHtmlReport();

    virtual void add(const KAssessment& asObj);
    virtual void add(const KScenario& scenario);
    virtual void add(IModel * model);
    virtual void add(const KLocation& loc);
    virtual void add(const KDataGroupArray & dga);
    virtual void add(const KDataArray & da);
    virtual void add(const KDataTable & table);
    virtual void beginReport();
    virtual void endReport();
    virtual void clear();

private:
    Q_DISABLE_COPY(KHtmlReport)
};

#endif // KHTMLREPORT_H
