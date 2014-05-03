#include <QSharedData>
#include <QPluginLoader>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include "kpluginmanager.h"
#include "imodelfactory.h"
#include "koutput.h"
#include "imodel.h"


//default instance
static KPluginManager __pluginManager;

class KPluginManagerPrivate : public QSharedData {
private:
    QMap<QString, IModelFactory *> _factories;
    QMap<const Quantity *, IModelFactory *> _quantityFactories;
public:

    KPluginManagerPrivate()
    {
        qDebug() << "#Allocate plugin manager instance";
    }
    ~KPluginManagerPrivate()
    {
        qDebug() << "Destroying plugin manager instance";
    }

    inline int factoryCount() const
    {
        return _factories.count();
    }
    inline FactoryList factories() const
    {
        return _factories.values();
    }
    inline IModelFactory * factory(const Quantity * qty) const
    {
        return _quantityFactories[qty];
    }
    inline IModelFactory * factory(const QString& name) const
    {
        return _factories[name];
    }

    const Quantity * findQuantity(const QString& factoryName, unsigned int qid) const
    {
        IModelFactory * factory = _factories[factoryName];
        if (factory != 0)
            return findQuantity(factory->availableQuantities(), qid);
        return &Rad::EmptyQuantity;
    }

    const Quantity * findQuantity(const ConstQuantityList & qtyList, unsigned int qid) const
    {
        ConstQuantityList::const_iterator qty = qtyList.constBegin();
        ConstQuantityList::const_iterator end = qtyList.constEnd();
        while(qty != end) {
            if ((*qty)->id == qid)
                return (*qty);
            qty++;
        }
        return &Rad::EmptyQuantity;
    }

    void mapQuantityFactories(IModelFactory * factory)
    {
        ConstQuantityList qtyList = factory->availableQuantities();
        ConstQuantityList::const_iterator qty = qtyList.constBegin();
        ConstQuantityList::const_iterator end = qtyList.constEnd();
        while (qty != end) {
            _quantityFactories[*qty] = factory;
            qty++;
        }
    }

    int loadPlugins(const QString& path)
    {
        QDir pluginsDir;
        if (path.isEmpty()) {
            //default plugin dirs
            pluginsDir = QDir(QCoreApplication::instance()->applicationDirPath());
        #if defined(Q_OS_WIN)
            if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
                pluginsDir.cdUp();
        #elif defined(Q_OS_MAC)
            if (pluginsDir.dirName() == "MacOS") {
                pluginsDir.cdUp();
                pluginsDir.cdUp();
                pluginsDir.cdUp();
            }
        #endif
            pluginsDir.cd("plugins");
        }
        else {
            pluginsDir = QDir(path);
        }

        //load available plugins
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IModelFactory * factory = qobject_cast<IModelFactory *>(plugin);
                if (factory) {
                    factory->initialize();
                    _factories[factory->name()] = factory;
                    mapQuantityFactories(factory);

                    xTrace() << "Plugin directory: " << pluginsDir.absoluteFilePath(fileName);
                    xInfo() << "Name:" << factory->name() << ",Author:" << factory->author()
                             << "Version: " << factory->version();
                }
            }
            else {
                qWarning() << "Error: " << pluginLoader.errorString();
                pluginLoader.unload();
            }
        }

        return _factories.count();
    }

    bool unloadPlugins()
    {
        FactoryList factList = factories();
        for(int k = 0; k < factList.size(); k++)
            factList.at(k)->onFinalized();

        //TODO
        //add unload

        _factories.clear();
        return true;
    }

};

KPluginManager * KPluginManager::instance()
{
    return &__pluginManager;
}

KPluginManager::KPluginManager() : data(new KPluginManagerPrivate)
{
}

KPluginManager::KPluginManager(const KPluginManager &rhs) : data(rhs.data)
{
}

KPluginManager &KPluginManager::operator=(const KPluginManager &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KPluginManager::~KPluginManager()
{
}

int KPluginManager::factoryCount() const
{
    return data->factoryCount();
}
FactoryList KPluginManager::factories() const
{
    return data->factories();
}
IModelFactory * KPluginManager::factory(const Quantity * qty) const
{
    return data->factory(qty);
}
IModelFactory * KPluginManager::factory(const QString & name) const
{
    return data->factory(name);
}
IModel * KPluginManager::createModel(IModelFactory * fact, const KModelInfo & mi) const
{
    IModel * model = fact->createModel(mi);
    if (model != 0) {
        if (!model->initialize()) {
            delete model;
            return 0;
        }
    }
    return model;
}
IModel * KPluginManager::createModel(const QString &factName, int infoId) const
{
    IModelFactory * fact = data->factory(factName);
    if (fact == 0)
        return 0;
    KModelInfo mi = fact->modelInfo(infoId);
    IModel * model = fact->createModel(mi);
    if (model != 0) {
        if (!model->initialize()) {
            delete model;
            return 0;
        }
    }
    return model;
}
IModel * KPluginManager::createModel(QDataStream &stream) const
{
    int serId;
    QString factName;

    //Important
    //This deserialization order is same as Serialization order
    //See IModel::serialize
    stream >> factName >> serId;
    return createModel(factName, serId);
}
const Quantity * KPluginManager::findQuantity(IModelFactory * fact, unsigned int qid) const
{
    return data->findQuantity(fact == 0 ? RAD_NULL_FACTORY : fact->name(), qid);
}
const Quantity * KPluginManager::findQuantity(const QString &factoryName, unsigned int qid) const
{
    return data->findQuantity(factoryName, qid);
}

int KPluginManager::loadPlugins()
{
    return data->loadPlugins(QString());
}

int KPluginManager::loadPlugins(const QString& path)
{
    return data->loadPlugins(path);
}
bool KPluginManager::unloadPlugins()
{
    return data->unloadPlugins();
}
