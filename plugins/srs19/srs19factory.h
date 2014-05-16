#ifndef SRS19FACTORY_H
#define SRS19FACTORY_H

#include "srs19.h"
#include "imodelfactory.h"
#include "kgroupinfo.h"

class SRS19_API Srs19Factory : public QObject, public IModelFactory
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ips.rad.Plugin.IModelFactory" FILE "srs19.json")
#endif
    Q_INTERFACES(IModelFactory)
public:
    Srs19Factory();
    virtual ~Srs19Factory();

    void setManager(KApplication * mngr);
    KApplication * manager() const;
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
    KApplication * _manager;
    KStorage * _storage;
};

#endif // SRS19FACTORY_H
