#ifndef IMODELFACTORY_H
#define IMODELFACTORY_H

#include <QtPlugin>
#include "radglobal.h"

/**
 * @brief The interface for model factory class. This should be implemented in plugin.
 *
 */
class IModelFactory
{
public:
    virtual ~IModelFactory() {}

    /**
     * @brief Initialize the factory. This method is called after plugin instance is created.
     * @return Return true if initialization success, otherwise false.
     */
    virtual bool initialize() = 0;

    /**
     * @brief Do something before destroying this object
     * @return
     */
    virtual void onFinalized() = 0;

    /**
     * @brief Tell the factory to create new Model.
     * @param info
     * @return
     */
    virtual bool isTopLevel() const = 0;

    /**
     * @brief List all available quantities in the plugins.
     * @return
     */
    virtual ConstQuantityList availableQuantities() const = 0;

    virtual QuantityList editableQuantities() const = 0;

    /**
     * @brief Get setting manager for the plugins.
     * @return
     */
    virtual KSettingManager * settingManager() = 0;

    virtual IModel * createModel(const KModelInfo & info) = 0;
    virtual const GroupInfoList & groups() const = 0;
    virtual QString name() const = 0;
    virtual QString author() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;
    virtual IParamsEditor * paramsEditor() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IModelFactory, "ips.rad.Plugin.IModelFactory/1.0")
QT_END_NAMESPACE

#endif // IMODELFACTORY_H
