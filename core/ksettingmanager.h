#ifndef KSETTINGMANAGER_H
#define KSETTINGMANAGER_H

#include <QSharedDataPointer>
#include "imodelfactory.h"

class KSettingManagerPrivate;
class K_CORE_EXPORT KSettingManager
{
public:
    explicit KSettingManager(IModelFactory * factory);
    KSettingManager(const KSettingManager &);
    KSettingManager &operator=(const KSettingManager &);
    virtual ~KSettingManager();

    virtual bool save();
    virtual bool load();
    
private:
    QSharedDataPointer<KSettingManagerPrivate> data;
};

#endif // KSETTINGMANAGER_H
