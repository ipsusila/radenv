#ifndef KREPORT_H
#define KREPORT_H

#include "kdata.h"

class QTextStream;
class K_CORE_EXPORT KReport
{
public:
    KReport();
    KReport(const KReport&);
    KReport &operator=(const KReport&);
    virtual ~KReport();

    QString name() const;
    void setName(const QString& n);

    virtual void addCase(const KCase& caseObj);
    virtual void addModel(IModel * model);
    virtual void addLocation(const KLocation& loc);
    virtual void addUserInputs(const KDataGroupArray & dga);
    virtual void addResult(const KDataArray & da);
    virtual void addResult(const KDataTable & table);
    virtual QString toPlainText();
    virtual void beginReport();
    virtual void endReport();

private:
    QString _name;
    QTextStream * _stream;
    QByteArray _buffer;
};

#endif // KREPORT_H
