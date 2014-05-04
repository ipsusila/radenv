#ifndef SRS19FACTORY_H
#define SRS19FACTORY_H

#include "srs19.h"
#include "imodelfactory.h"
#include "kgroupinfo.h"

class SRS19_SHARED_EXPORT Srs19Factory : public QObject, public IModelFactory
{
    Q_OBJECT
    Q_INTERFACES(IModelFactory)
public:
    Srs19Factory();
    virtual ~Srs19Factory();

    void setManager(KPluginManager * mngr);
    KPluginManager * manager() const;
    KStorage * storage() const;

    bool isTopLevel() const;
    bool initialize();
    void finalize();
    void attachStorage(KStorage * stg);
    void detachStorage();
    IModel * createModel(const KModelInfo & info);
    const GroupInfoList & groups() const;
    KModelInfo modelInfo(int serId) const;
    ConstQuantityList availableQuantities() const;
    QuantityList editableQuantities() const;
    QString name() const;
    QString author() const;
    QString version() const;
    QString description() const;

    IParamsEditor * paramsEditor();
    KSettingManager * settingManager();
private:
    GroupInfoList _groupInfos;
    IParamsEditor * _paramsEditor;
    KSettingManager * _settingManager;
    KPluginManager * _manager;
    KStorage * _storage;
};

#endif // SRS19FACTORY_H
