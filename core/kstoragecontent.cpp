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

KStorageContent::KStorageContent() : data(new KStorageContentPrivate)
{
}
KStorageContent::KStorageContent(const QDateTime &c) : data(new KStorageContentPrivate(c))
{
}

KStorageContent::KStorageContent(const KStorageContent &rhs) : QByteArray(rhs), data(rhs.data)
{
}

KStorageContent &KStorageContent::operator=(const KStorageContent &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
        QByteArray::operator=(rhs);
    }
    return *this;
}

KStorageContent::~KStorageContent()
{
}
QString KStorageContent::name() const
{
    return data->name;
}
void KStorageContent::setName(const QString& nm)
{
    data->name = nm;
}
QString KStorageContent::factoryName() const
{
    return data->factory == 0 ? RAD_NULL_FACTORY : data->factory->name();
}
const IModelFactory * KStorageContent::factory() const
{
    return data->factory;
}
void KStorageContent::setFactory(const IModelFactory *f)
{
    data->factory = f;
}

QString KStorageContent::description() const
{
    return data->description;
}
void KStorageContent::setDescription(const QString& desc)
{
    data->description = desc;
}

QDateTime KStorageContent::created() const
{
    return data->created;
}
