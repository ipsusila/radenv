#include <QSharedData>
#include <QPluginLoader>
#include <QDir>
#include <QCoreApplication>
#include <QtDebug>
#include <QDesktopWidget>
#include <QSettings>
#include "kapplication.h"
#include "imodelfactory.h"
#include "koutput.h"
#include "imodel.h"
#include "kstorage.h"
#include "koptions.h"

class KApplicationPrivate : public QSharedData {
private:
    KStorage * _storage;
    QMap<QString, IModelFactory *> _factories;
    QMap<const Quantity *, IModelFactory *> _quantityFactories;
    KOptions * _options;

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
    KApplicationPrivate(KApplication * mngr) : _storage(0)
    {
        qDebug() << "#Allocate application instance";
        loadPlugins(mngr, QString());
        _options = new KOptions();
    }
    ~KApplicationPrivate()
    {
        qDebug() << "Destroying application instance";
        unloadPlugins();
        delete _options;
    }
    inline KOptions * options() const
    {
        return _options;
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
    inline void closeStorage() {
        detachStorage();
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

    int loadPlugins(KApplication * mngr, const QString& path)
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
            //TODO
            //move to options
            pluginsDir.cd("plugins/radenv");
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
                else {
                    xTrace() << "Can not create factory instance, " << pluginLoader.errorString();
                    pluginLoader.unload();
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
        //add unload dll (if possible)

        _factories.clear();
        _quantityFactories.clear();

        qDebug() << "Unloading all plugins";

        return true;
    }
};

KApplication * KApplication::selfInstance()
{
    //cast instance
    KApplication * obj = reinterpret_cast<KApplication*>(instance());
    return obj;
}

KApplication::KApplication(int &argc, char **argv)
    : QApplication(argc, argv), dptr(new KApplicationPrivate(this))
{
}


KApplication::~KApplication()
{
}

void KApplication::toValidDialogGeometry(QRect *rGeometry, QSize *maxSize) const
{
    //desktop
    QDesktopWidget * desktop = QApplication::desktop();
    QSize dSize = desktop->size();
    if (maxSize != 0)
        *maxSize = 0.9 * dSize;

    //set size
    const int defMargin = 20;
    QRect defGeo(100, 100, 300, 400);

    if (rGeometry->width() > dSize.width())
        rGeometry->setWidth(dSize.width()/2);
    if (rGeometry->width() > dSize.height())
        rGeometry->setHeight(dSize.height()/2);

    //reset geometry
    if (rGeometry->isValid()) {
        if (rGeometry->top() > (dSize.height() - defMargin) || rGeometry->left() > (dSize.width() - defMargin))
            *rGeometry = defGeo;
    }
    else
        *rGeometry = defGeo;
}
void KApplication::setupValidGeometry(const QString &settingName, QWidget *widget)
{
    //QSize mSize;
    QSettings settings;
    QByteArray geom = settings.value(settingName).toByteArray();
    if(!geom.isEmpty())
        widget->restoreGeometry(geom);

    QPoint pos = widget->pos();
    if (pos.x() <= 0)
        pos.setX(50);
    if (pos.y() <= 0)
        pos.setY(50);
    widget->move(pos);

    qDebug() << "Setting path: " << settings.fileName();
}

bool KApplication::isValid() const
{
    return dptr->isValid();
}

int KApplication::factoryCount() const
{
    return dptr->factoryCount();
}
FactoryList KApplication::factories() const
{
    return dptr->factories();
}
IModelFactory * KApplication::factory(const Quantity * qty) const
{
    return dptr->factory(qty);
}
IModelFactory * KApplication::factory(const QString & name) const
{
    return dptr->factory(name);
}
IModel * KApplication::createModel(IModelFactory * fact, const KModelInfo & mi) const
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
IModel * KApplication::createModel(const QString &factName, int infoId) const
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
IModel * KApplication::createModel(QDataStream &stream) const
{
    int serId;
    QString factName;

    //Important
    //This deserialization order is same as Serialization order
    //See IModel::serialize
    stream >> factName >> serId;
    return createModel(factName, serId);
}
void KApplication::setStorage(const QString& name)
{
    dptr->setStorage(name);
}
void KApplication::closeStorage()
{
    dptr->closeStorage();
}

KStorage * KApplication::storage() const
{
    return dptr->storage();
}
bool KApplication::hasStorage() const
{
    return dptr->storage() != 0;
}
KOptions * KApplication::options() const
{
    return dptr->options();
}

const Quantity * KApplication::findQuantity(IModelFactory * fact, unsigned int qid) const
{
    return dptr->findQuantity(fact == 0 ? RAD_NULL_FACTORY : fact->name(), qid);
}
const Quantity * KApplication::findQuantity(const QString &factoryName, unsigned int qid) const
{
    return dptr->findQuantity(factoryName, qid);
}

int KApplication::loadPlugins(const QString& path)
{
    return dptr->loadPlugins(this, path);
}
bool KApplication::unloadPlugins()
{
    return dptr->unloadPlugins();
}

void KApplication::serialize(QDataStream & stream, const Quantity * qty) const
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

const Quantity * KApplication::deserialize(QDataStream & stream) const
{
    unsigned int id;
    stream >> id;
    if (id == INV_QTY_ID)
        return &Rad::EmptyQuantity;

    QString factName;
    stream >> factName;

    return dptr->findQuantity(factName, id);
}
