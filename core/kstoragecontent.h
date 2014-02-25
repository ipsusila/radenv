#ifndef KSTORAGECONTENT_H
#define KSTORAGECONTENT_H

#include <QSharedDataPointer>
#include <QDataStream>
#include "radglobal.h"

class KStorageContentPrivate;
class K_CORE_EXPORT KStorageContent : public QByteArray
{
public:
    KStorageContent();
    KStorageContent(const QDateTime& c);
    KStorageContent(const KStorageContent &);
    KStorageContent &operator=(const KStorageContent &);
    ~KStorageContent();

    QString name() const;
    void setName(const QString& nm);
    QString factoryName() const;
    const IModelFactory * factory() const;
    void setFactory(const IModelFactory *f);
    QString description() const;
    void setDescription(const QString& desc);
    QDateTime created() const;

private:
    QSharedDataPointer<KStorageContentPrivate> data;
};

#endif // KSTORAGECONTENT_H
