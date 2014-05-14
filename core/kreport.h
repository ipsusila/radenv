#ifndef KREPORT_H
#define KREPORT_H

#include "kdata.h"

class QTextStream;
class RADENV_API KReport
{
public:
    KReport();
    KReport(const KReport&);
    KReport &operator=(const KReport&);
    virtual ~KReport();

    QString name() const;
    void setName(const QString& n);

    virtual void add(const KAssessment& caseObj);
    virtual void add(const KScenario& scenario);
    virtual void add(IModel * model);
    virtual void add(const KLocation& loc);
    virtual void add(const KDataGroupArray & dga);
    virtual void add(const KDataArray & da);
    virtual void add(const KDataTable & table);
    virtual QString toString();
    virtual void beginReport();
    virtual void endReport();
    virtual void clear();

protected:
    QTextStream * textStream() const;
    void releaseStream();

private:
    QString _name;
    QTextStream * _stream;
    QByteArray _buffer;
};

#endif // KREPORT_H
