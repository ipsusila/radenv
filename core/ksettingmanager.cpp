#include "ksettingmanager.h"
#include <QSharedData>

class KSettingManagerPrivate : public QSharedData {
public:
    IModelFactory * _factory;

    KSettingManagerPrivate(IModelFactory * f) : _factory(f)
    {
    }
};

KSettingManager::KSettingManager(IModelFactory * factory)
    : data(new KSettingManagerPrivate(factory))
{
}

KSettingManager::KSettingManager(const KSettingManager &rhs) : data(rhs.data)
{
}

KSettingManager &KSettingManager::operator=(const KSettingManager &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KSettingManager::~KSettingManager()
{
}

bool KSettingManager::save()
{
    return true;
}

bool KSettingManager::load()
{
    return true;
}
