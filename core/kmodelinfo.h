#ifndef KMODELINFO_H
#define KMODELINFO_H

#include <QIcon>
#include <QMetaType>
#include "iserializable.h"

class KModelInfoPrivate;
class RADENV_API KModelInfo
{
public:
    KModelInfo();
    explicit KModelInfo(int serId);
    explicit KModelInfo(int serId, const QString& n, const QString& t, const QString& d, const QIcon& i);
    KModelInfo(const KModelInfo &);
    KModelInfo &operator=(const KModelInfo &);
    ~KModelInfo();

    QString name() const;
    void setName(const QString& n);
    QString text() const;
    void setText(const QString& t);
    QString description() const;
    void setDescription(const QString& d);
    QIcon icon() const;
    void setIcon(const QIcon& i);
    int serialId() const;

private:
    QSharedDataPointer<KModelInfoPrivate> dptr;
};

#endif // KMODELINFO_H
