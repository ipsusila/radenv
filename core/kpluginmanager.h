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

    bool isInitialized() const;
    int factoryCount() const;
    FactoryList factories() const;
    IModelFactory * factory(const Quantity * qty) const;
    IModelFactory * factory(const QString & name) const;
    IModel * createModel(IModelFactory * fact, const KModelInfo & mi) const;
    IModel * createModel(const QString& factName, int infoId) const;
    IModel * createModel(QDataStream &stream) const;
    void setStorage(const QString& name);
    KStorage * storage() const;

    const Quantity * findQuantity(IModelFactory * fact, unsigned int qid) const;
    const Quantity * findQuantity(const QString &factoryName, unsigned int qid) const;

    void serialize(QDataStream & stream, const Quantity * qty) const;
    const Quantity * deserialize(QDataStream & stream) const;

    //global pointer and method
    static KPluginManager * instance();
    static KPluginManager * pMan;
    
private:
    Q_DISABLE_COPY(KPluginManager)
    QSharedDataPointer<KPluginManagerPrivate> data;
};

#endif // KPLUGINMANAGER_H
