#ifndef KDBCONNECTOR_H
#define KDBCONNECTOR_H

#include "radglobal.h"

class KDbConnectorPrivate;

class RADENV_API KDbConnector
{
public:
    KDbConnector(const QString& database);
    KDbConnector(const KDbConnector &);
    KDbConnector &operator=(const KDbConnector &);
    ~KDbConnector();

private:
    QSharedDataPointer<KDbConnectorPrivate> dptr;
};

#endif // KDBCONNECTOR_H
