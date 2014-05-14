#include <QSharedData>
#include <QPluginLoader>
#include <QDir>
#include <QCoreApplication>
#include <QtDebug>
#include "kpluginmanager.h"
#include "imodelfactory.h"
#include "koutput.h"
#include "imodel.h"
#include "kstorage.h"


//global pointer
KPluginManager * KPluginManager::pMan = 0;
class KPluginManagerPrivate : public QSharedData {
private:
    KStorage * _storage;
    QMap<QString, IModelFactory *> _factories;
    QMap<const Quantity *, IModelFactory *> _quantityFactories;

    void detachStorage()
    {
        FactoryList factList = factories();
        for(int k = 0; k < factList.size(); k++) {
            IModelFactory * fact = factList.at(k);
            if (fact->storage() != 0)
                fact->detachStorage();
        }

        if (_storage != 0) {
            delete _storage;
            _storage = 0;
        }
    }

    void attachStorage(IModelFactory * factory)
    {
        Q_ASSERT(_storage != 0);

        //attach storage to the factory
        //this should force factory to reload any configuration,
        //quantity tables, etc
        factory->attachStorage(_storage);

        //create maps between quantity and factory
        ConstQuantityList qtyList = factory->availableQuantities();
        ConstQuantityList::const_iterator qty = qtyList.constBegin();
        ConstQuantityList::const_iterator end = qtyList.constEnd();
        while (qty != end) {
            _quantityFactories[*qty] = factory;
            qty++;
        }
    }

public:
    KPluginManagerPrivate(KPluginManager * mngr) : _storage(0)
    {
        qDebug() << "#Allocate plugin manager instance";
        loadPlugins(mngr, QString());
    }
    ~KPluginManagerPrivate()
    {
        qDebug() << "Destroying plugin manager instance";
        unloadPlugins();
    }

    inline bool isValid() const
    {
        return !_quantityFactories.isEmpty();
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

    void setStorage(const QString& name)
    {
        //detach existing storage
        detachStorage();
        _storage = new KStorage(name);

        FactoryList factories = _factories.values();
        _quantityFactories.clear();
        for(int k = factories.size() - 1; k >= 0; k--)
            attachStorage(factories.at(k));
    }

    inline KStorage * storage() const
    {
        return _storage;
    }

    int loadPlugins(KPluginManager * mngr, const QString& path)
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
                    factory->setManager(mngr);

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
        //detach storage
        detachStorage();

        FactoryList factList = factories();
        for(int k = 0; k < factList.size(); k++)
            factList.at(k)->finalize();

        //TODO
        //add unload

        _factories.clear();
        _quantityFactories.clear();

        qDebug() << "Unloading all plugins";

        return true;
    }
};

KPluginManager * KPluginManager::instance()
{
    //must be allocated elsewere
    return pMan;
}

KPluginManager::KPluginManager() : dptr(new KPluginManagerPrivate(this))
{
    //allocate manager elsewhere
    pMan = this;
}

KPluginManager::KPluginManager(const KPluginManager &rhs) : dptr(rhs.dptr)
{
}

KPluginManager &KPluginManager::operator=(const KPluginManager &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KPluginManager::~KPluginManager()
{
}

bool KPluginManager::isValid() const
{
    return dptr->isValid();
}

int KPluginManager::factoryCount() const
{
    return dptr->factoryCount();
}
FactoryList KPluginManager::factories() const
{
    return dptr->factories();
}
IModelFactory * KPluginManager::factory(const Quantity * qty) const
{
    return dptr->factory(qty);
}
IModelFactory * KPluginManager::factory(const QString & name) const
{
    return dptr->factory(name);
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
    IModelFactory * fact = dptr->factory(factName);
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
void KPluginManager::setStorage(const QString& name)
{
    dptr->setStorage(name);
}

KStorage * KPluginManager::storage() const
{
    return dptr->storage();
}
bool KPluginManager::hasStorage() const
{
    return dptr->storage() != 0;
}

const Quantity * KPluginManager::findQuantity(IModelFactory * fact, unsigned int qid) const
{
    return dptr->findQuantity(fact == 0 ? RAD_NULL_FACTORY : fact->name(), qid);
}
const Quantity * KPluginManager::findQuantity(const QString &factoryName, unsigned int qid) const
{
    return dptr->findQuantity(factoryName, qid);
}

int KPluginManager::loadPlugins(const QString& path)
{
    return dptr->loadPlugins(this, path);
}
bool KPluginManager::unloadPlugins()
{
    return dptr->unloadPlugins();
}

void KPluginManager::serialize(QDataStream & stream, const Quantity * qty) const
{
    if (qty == &Rad::EmptyQuantity) {
        stream << INV_QTY_ID;
    }
    else {
        IModelFactory * fact = dptr->factory(qty);
        if (fact != 0)
            stream << qty->id << fact->name();
        else
            stream << INV_QTY_ID;
    }
}

const Quantity * KPluginManager::deserialize(QDataStream & stream) const
{
    unsigned int id;
    stream >> id;
    if (id == INV_QTY_ID)
        return &Rad::EmptyQuantity;

    QString factName;
    stream >> factName;

    return dptr->findQuantity(factName, id);
}
