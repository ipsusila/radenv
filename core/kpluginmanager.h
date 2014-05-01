#ifndef KPLUGINMANAGER_H
#define KPLUGINMANAGER_H

#include "radglobal.h"

class KPluginManagerPrivate;
class K_CORE_EXPORT KPluginManager
{
public:
    KPluginManager();
    ~KPluginManager();

    int loadPlugins();
    int loadPlugins(const QString& path);
    bool unloadPlugins();
    FactoryList factories() const;
    int factoryCount() const;

    ConstQuantityList coreQuantities() const;
    const Quantity * findQuantity(IModelFactory * factory, unsigned int qid) const;
    const Quantity * findQuantity(const QString &factoryName, unsigned int qid) const;

    static KPluginManager * instance();
    
private:
    Q_DISABLE_COPY(KPluginManager)
    QExplicitlySharedDataPointer<KPluginManagerPrivate> data;
};

#endif // KPLUGINMANAGER_H
