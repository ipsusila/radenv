#include <QSharedData>
#include <QVector>
#include "distributioncoefficient.h"
#include "kstorage.h"
#include "kstoragecontent.h"
#include "kradionuclide.h"

static const KdValue __emptyKd = {"", -1, -1};
static const char * __ContentName = "Distribution Coefficients";
static const char * __ContentDescription = "Screening values for Kd (L/kg) for elements in natural fresh/salt water with emphasis on oxidizing conditions.";
static const KdValue __defaultKd [] =
{
    {"Ac",-1,2E+6},
    {"Ag",-1,1E+3},
    {"Am",5E+3,2E+6},
    {"As",-1,-1},
    {"At",-1,-1},
    {"Au",-1,-1},
    {"Ba",-1,5E+3},
    {"Bi",-1,-1},
    {"Br",-1,-1},
    {"C",5,2E+3},
    {"Cd",-1,2E+3},
    {"Ce",1E+4,2E+6},
    {"Cm",5E+3,2E+6},
    {"Co",5E+3,2E+5},
    {"Cr",1E+4,5E+4},
    {"Cs",1E+3,3E+3},
    {"Cu",-1,-1},
    {"Eu",5E+2,5E+5},
    {"Fe",5E+3,5E+4},
    {"Ga",-1,-1},
    {"H",0,1},
    {"Hg",-1,1E+4},
    {"I",10,2E+1},
    {"In",-1,1E+5},
    {"Mn",1E+3,2E+5},
    {"Mo",-1,-1},
    {"Na",-1,1},
    {"Nb",-1,5E+5},
    {"Ni",-1,1E+5},
    {"Np",10,5E+3},
    {"P",50,1E+2},
    {"Pa",-1,1E+6},
    {"Pb",-1,2E+5},
    {"Pd",-1,5E+4},
    {"Pm",5E+3,2E+6},
    {"Po",-1,2E+7},
    {"Pu",1E+5,1E+5},
    {"Ra",500,5E+3},
    {"Rb",-1,-1},
    {"Rh",-1,-1},
    {"Ru",500,3E+2},
    {"S",-1,5E-1},
    {"Sb",50,1E+3},
    {"Se",-1,1E+3},
    {"Sr",1E+3,1E+3},
    {"Tc",5,1E+2},
    {"Te",-1,1E+3},
    {"Th",1E+4,2E+6},
    {"Tl",-1,2E+4},
    {"U",50,1E+3},
    {"Y",-1,1E+7},
    {"Zn",500,2E+4},
    {"Zr",1E+3,1E+6}
};

class DistributionCoefficientPrivate : public QSharedData
{
public:
    IModelFactory * factory;
    KStorage * storage;
    QVector<KdValue> kdTable;
    QString name;
    QString description;
    QDateTime created;

    DistributionCoefficientPrivate(IModelFactory *f, KStorage *s)
        : factory(f), storage(s)
    {
        if (storage) {
            loadFrom(storage);
        }
        else {
            createDefault();
        }
    }
    void createDefault()
    {
        int sz = sizeof(__defaultKd) / sizeof(KdValue);
        kdTable.reserve(sz);
        for (int k = 0; k < sz; k++) {
            kdTable.append(__defaultKd[k]);
        }

        name = __ContentName;
        description = __ContentDescription;
        created = QDateTime::currentDateTime();
    }

    bool loadFrom(KStorage * storage) {
        //save pointer to storage
        this->storage = storage;

        //load from storage
        kdTable.clear();
        KStorageContent  content = storage->load(__ContentName, factory);
        if (!content.isEmpty()) {
            name = content.name();
            description = content.description();
            created = content.created();

            QDataStream stream(content);
            stream >> kdTable;
        }
        return !kdTable.isEmpty();
    }
    bool saveTo(KStorage * storage) {
        KStorageContent content(created);
        content.setFactory(factory);
        content.setName(name);
        content.setDescription(description);

        //append content
        QDataStream stream(&content, QIODevice::WriteOnly);
        stream << kdTable;
        return storage->save(content);
    }
    qreal coeff(const QString& nuclide, bool saltWater) const
    {
        QString e = KRadionuclide::elementName(nuclide);
        foreach(KdValue item, kdTable) {
            if (e == item.element) {
                if (saltWater)
                    return item.saltWater;
                return item.freshWater;
            }
        }
        return 0;
    }
    KdValue coeff(const QString& nuclide) const
    {
        QString e = KRadionuclide::elementName(nuclide);
        foreach(KdValue item, kdTable) {
            if (e == item.element) {
                return item;
            }
        }
        return __emptyKd;
    }
};

DistributionCoefficient::DistributionCoefficient(IModelFactory * factory, KStorage * storage)
    : data(new DistributionCoefficientPrivate(factory, storage))
{

}

DistributionCoefficient::DistributionCoefficient(const DistributionCoefficient & o)
    : data(o.data)
{
}
DistributionCoefficient::~DistributionCoefficient()
{

}

DistributionCoefficient & DistributionCoefficient::operator=(const DistributionCoefficient & o)
{
    if (this != &o) {
        data.operator=(o.data);
    }
    return *this;
}

bool DistributionCoefficient::save()
{
    return data->saveTo(KStorage::storage());
}
bool DistributionCoefficient::saveDefault()
{
    data->createDefault();
    return data->saveTo(KStorage::storage());
}

bool DistributionCoefficient::load(bool createOnEmpty)
{
    bool res = data->loadFrom(KStorage::storage());
    if (!res && createOnEmpty) {
        data->createDefault();
        res = data->saveTo(KStorage::storage());
    }

    return res;
}

qreal DistributionCoefficient::value(const QString& nuclide, bool saltWater) const
{
    return data->coeff(nuclide, saltWater);
}
KdValue DistributionCoefficient::value(const QString &nuclide) const
{
    return data->coeff(nuclide);
}

