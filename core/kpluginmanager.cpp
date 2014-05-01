#include <QSharedData>
#include <QPluginLoader>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include "kpluginmanager.h"
#include "imodelfactory.h"
#include "koutput.h"


//default instance
static KPluginManager __pluginManager;

class KPluginManagerPrivate : public QSharedData {
private:
    QMap<QString, IModelFactory *> _factories;
    ConstQuantityList _coreQuantities;
public:

    KPluginManagerPrivate()
    {
        _coreQuantities
            << &Rad::EmptyQuantity
            << &Rad::ScalarInput
            << &Rad::ScalarInput1
            << &Rad::ScalarInput2
            << &Rad::ScalarInput3
            << &Rad::ScalarInput4
            << &Rad::ScalarInput5
            << &Rad::ScalarInput6
            << &Rad::ScalarInput7
            << &Rad::ScalarInput8
            << &Rad::ScalarInput9

            << &Rad::ScalarOutput
            << &Rad::ScalarOutput1
            << &Rad::ScalarOutput2
            << &Rad::ScalarOutput3
            << &Rad::ScalarOutput4
            << &Rad::ScalarOutput5

            << &Rad::CommentQuantity
            << &Rad::NameQuantity
            << &Rad::UseDefaultValue
            << &Rad::LongCommentQuantity
            << &Rad::UseDefaultValue2;

        qDebug() << "#Allocate plugin manager instance";
    }
    ~KPluginManagerPrivate()
    {
        qDebug() << "Destroying plugin manager instance";
    }

    inline FactoryList factories() const
    {
        return _factories.values();
    }
    inline int factoryCount() const
    {
        return _factories.count();
    }
    inline const ConstQuantityList & coreQuantities() const
    {
        return _coreQuantities;
    }
    const Quantity * findQuantity(const QString& factoryName, unsigned int qid) const
    {
        IModelFactory * factory;
        if (factoryName == RAD_NULL_FACTORY)
            return findQuantity(_coreQuantities, qid);
        else if ((factory = _factories[factoryName]) != 0)
            return findQuantity(factory->availableQuantities(), qid);
        return 0;
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
        return 0;
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

FactoryList KPluginManager::factories() const
{
    return data->factories();
}
int KPluginManager::factoryCount() const
{
    return data->factoryCount();
}

ConstQuantityList KPluginManager::coreQuantities() const
{
    return data->coreQuantities();
}

const Quantity * KPluginManager::findQuantity(IModelFactory * factory, unsigned int qid) const
{
    return data->findQuantity(factory == 0 ? RAD_NULL_FACTORY : factory->name(), qid);
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
