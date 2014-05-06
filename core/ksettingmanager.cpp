#include <QSharedData>
#include <QMap>
#include "ksettingmanager.h"
#include "imodel.h"
#include "kmodelinfo.h"
#include "kstorage.h"

static const char * __sizeSectionName = "Model Geometry";
static const char * __sizeSectionDescription = "The geometry of user input for each model";

class KSettingManagerPrivate : public QSharedData {
public:
    IModelFactory * _factory;
    QMap<int, QRect> _rectMaps;

    KSettingManagerPrivate(IModelFactory * f) : _factory(f)
    {
        Q_ASSERT(f != 0);
    }
    inline void saveGeometry(IModel * model, const QRect& sz)
    {
        Q_ASSERT(model != 0);
        _rectMaps[model->info().serialId()] = sz;
    }
    inline QRect geometry(IModel * model) const
    {
        Q_ASSERT(model != 0);
        return _rectMaps[model->info().serialId()];
    }

    void save()
    {
        Q_ASSERT(_factory != 0);

        KStorage * storage = _factory->storage();
        KStorageContent content(QDateTime::currentDateTime());
        content.setFactory(_factory);
        content.setName(__sizeSectionName);
        content.setDescription(__sizeSectionDescription);

        //append content
        QMap<int, QRect>::const_iterator iter = _rectMaps.constBegin();
        QMap<int, QRect>::const_iterator end = _rectMaps.constEnd();
        QDataStream stream(&content, QIODevice::WriteOnly);
        while(iter != end) {
            stream << iter.key() << iter.value();
            iter++;
        }
        storage->save(content);
    }
    void load()
    {
        Q_ASSERT(_factory != 0);

        KStorage * storage = _factory->storage();
        KStorageContent  content = storage->load(__sizeSectionName, _factory);
        if (!content.isEmpty()) {
            int serId;
            QRect rect;
            QDataStream stream(content);
            while(!stream.atEnd()) {
                stream >> serId >> rect;
                _rectMaps[serId] = rect;
            }
        }
    }
};

KSettingManager::KSettingManager(IModelFactory * factory)
    : dptr(new KSettingManagerPrivate(factory))
{
}

KSettingManager::KSettingManager(const KSettingManager &rhs) : dptr(rhs.dptr)
{
}

KSettingManager &KSettingManager::operator=(const KSettingManager &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KSettingManager::~KSettingManager()
{
}

bool KSettingManager::save()
{
    dptr->save();
    return doSave();
}

bool KSettingManager::load()
{
    dptr->load();
    return doLoad();
}
bool KSettingManager::doSave()
{
    return true;
}

bool KSettingManager::doLoad()
{
    return true;
}
void KSettingManager::saveGeometry(IModel * model, const QRect& rect)
{
    dptr->saveGeometry(model, rect);
}

QRect KSettingManager::geometry(IModel * model) const
{
    return dptr->geometry(model);
}
