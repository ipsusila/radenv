#include "radglobal.h"
#include "kpluginmanager.h"
#include "imodelfactory.h"

/** empty variable */
namespace Rad {
    const char LatinEndLine = '\n';
    const Quantity EmptyQuantity = {0, Rad::Real, 0, 0, 0, 0, "","","","","",""};

    void serialize(QDataStream & stream, const Quantity * qty)
    {
        if (qty == &Rad::EmptyQuantity) {
            stream << INV_QTY_ID;
        }
        else {
            IModelFactory * fact = KPluginManager::instance()->factory(qty);
            if (fact != 0)
                stream << qty->id << fact->name();
            else
                stream << INV_QTY_ID;
        }
    }

    const Quantity * deserialize(QDataStream & stream)
    {
        unsigned int id;
        stream >> id;
        if (id == INV_QTY_ID)
            return &Rad::EmptyQuantity;

        QString factName;
        stream >> factName;

        return KPluginManager::instance()->findQuantity(factName, id);
    }
}
