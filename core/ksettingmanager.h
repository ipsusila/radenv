#ifndef KSETTINGMANAGER_H
#define KSETTINGMANAGER_H

#include <QSharedDataPointer>
#include <QRect>
#include "imodelfactory.h"

class KSettingManagerPrivate;
class K_CORE_EXPORT KSettingManager
{
public:
    explicit KSettingManager(IModelFactory * factory);
    KSettingManager(const KSettingManager &);
    KSettingManager &operator=(const KSettingManager &);
    virtual ~KSettingManager();

    bool save();
    bool load();

    void saveGeometry(IModel * model, const QRect& rect);
    QRect geometry(IModel * model) const;

protected:
    virtual bool doSave();
    virtual bool doLoad();
    
private:
    QSharedDataPointer<KSettingManagerPrivate> data;
};

#endif // KSETTINGMANAGER_H
