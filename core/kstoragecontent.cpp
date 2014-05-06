#include <QSharedData>
#include "kstoragecontent.h"
#include "imodelfactory.h"

class KStorageContentPrivate : public QSharedData {
public:
    const IModelFactory * factory;
    QDateTime created;
    QString name;
    QString description;

    KStorageContentPrivate() : factory(0), created(QDateTime::currentDateTime()) {
    }
    KStorageContentPrivate(const QDateTime& c) : factory(0), created(c) {
    }
};

KStorageContent::KStorageContent() : dptr(new KStorageContentPrivate)
{
}
KStorageContent::KStorageContent(const QDateTime &c) : dptr(new KStorageContentPrivate(c))
{
}

KStorageContent::KStorageContent(const KStorageContent &rhs) : QByteArray(rhs), dptr(rhs.dptr)
{
}

KStorageContent &KStorageContent::operator=(const KStorageContent &rhs)
{
    if (this != &rhs) {
        dptr.operator=(rhs.dptr);
        QByteArray::operator=(rhs);
    }
    return *this;
}

KStorageContent::~KStorageContent()
{
}
QString KStorageContent::name() const
{
    return dptr->name;
}
void KStorageContent::setName(const QString& nm)
{
    dptr->name = nm;
}
QString KStorageContent::factoryName() const
{
    return dptr->factory == 0 ? RAD_NULL_FACTORY : dptr->factory->name();
}
const IModelFactory * KStorageContent::factory() const
{
    return dptr->factory;
}
void KStorageContent::setFactory(const IModelFactory *f)
{
    dptr->factory = f;
}

QString KStorageContent::description() const
{
    return dptr->description;
}
void KStorageContent::setDescription(const QString& desc)
{
    dptr->description = desc;
}

QDateTime KStorageContent::created() const
{
    return dptr->created;
}
