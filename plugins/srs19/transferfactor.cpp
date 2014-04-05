#include <QSharedData>
#include "transferfactor.h"
#include "kstorage.h"
#include "kelement.h"
#include "kradionuclide.h"

static const FvValue __emptyFv = {"", -1, -1, -1, -1};
static const char * __ContentName = "Transfer Factors for Terrestrial Foods";
static const char * __ContentDescription = "Concentration factor for the uptake of radionuclide from soil by vegetation, "
        "adjusted implicitly to account for adhesion of soil to the vegetation. For pasture grass the unit of mass is dry matter; "
        "for vegetables consumed by humans it is fresh weight.";
static const FvValue __defaultFv [] =
{
    {"Ac",0.1,0.001,2E-6,2E-5},
    {"Ag",0.1,0.01,1E-4,0.006},
    {"Am",0.1,0.002,2E-5,1E-4},
    {"Ar",0,0,0,0},
    {"As",0.2,0.08,1E-4,0.02},
    {"At",0.9,0.2,0.01,0.01},
    {"Au",0.4,0.1,1E-5,0.005},
    {"Ba",0.1,0.05,0.005,0.002},
    {"Bi",0.5,0.1,0.001,0.002},
    {"Br",2,0.4,0.02,0.05},
    {"C",-1,-1,-1,-1},
    {"Cd",5,0.5,0.02,0.001},
    {"Ce",0.1,0.05,3E-4,2E-4},
    {"Cm",0.1,0.001,2E-6,2E-5},
    {"Co",2,0.08,0.01,0.07},
    {"Cr",0.1,0.001,2E-4,0.09},
    {"Cs",1,0.04,0.01,0.05},
    {"Cu",2,0.5,0.002,0.01},
    {"Eu",0.1,0.002,6E-5,0.002},
    {"Fe",0.1,0.001,3E-4,0.05},
    {"Ga",0.1,0.003,1E-5,3E-4},
    {"H",-1,-1,-1,-1},
    {"Hg",3,0.3,5E-4,0.01},
    {"I",0.1,0.02,0.01,0.05},
    {"In",0.1,0.003,2E-4,0.004},
    {"Kr",0,0,0,0},
    {"Mn",10,0.3,3E-4,7E-4},
    {"Mo",1,0.2,0.005,0.01},
    {"Na",0.6,0.05,0.25,0.8},
    {"Nb",0.2,0.01,4E-6,3E-6},
    {"Ni",1,0.3,0.2,0.05},
    {"Np",0.5,0.04,5E-5,0.01},
    {"P",10,1,0.02,0.05},
    {"Pa",0.1,0.01,5E-6,5E-6},
    {"Pb",0.1,0.02,3E-4,7E-4},
    {"Pd",0.5,0.1,1E-4,2E-4},
    {"Pm",0.1,0.002,6E-5,0.002},
    {"Po",0.1,0.002,0.003,0.005},
    {"Pu",0.1,0.001,3E-6,2E-4},
    {"Ra",0.4,0.04,0.001,0.005},
    {"Rb",2,0.2,0.1,0.03},
    {"Rh",2,0.2,5E-4,0.002},
    {"Ru",0.2,0.05,3E-5,0.05},
    {"S",6,0.6,0.02,0.2},
    {"Sb",0.1,0.001,2.5E-4,0.005},
    {"Se",1,0.1,0.001,0.1},
    {"Sn",1,0.3,0.001,0.01},
    {"Sr",10,0.3,0.003,0.01},
    {"Tc",80,5,0.001,0.001},
    {"Te",10,1,0.005,0.07},
    {"Th",0.1,0.001,5E-6,1E-4},
    {"Tl",2,2,0.003,0.02},
    {"U",0.2,0.01,6E-4,0.003},
    {"Xe",0,0,0,0},
    {"Y",0.1,0.003,6E-5,0.01},
    {"Zn",2,2,0.01,0.2},
    {"Zr",0.1,0.001,6E-6,1E-5}
};

class TransferFactorPrivate : public QSharedData {
public:
    IModelFactory * factory;
    KStorage * storage;
    QVector<FvValue> fvTable;
    QString name;
    QString description;
    QDateTime created;

    TransferFactorPrivate(IModelFactory *f, KStorage *s)
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
        int sz = sizeof(__defaultFv) / sizeof(FvValue);
        fvTable.reserve(sz);
        for (int k = 0; k < sz; k++) {
            fvTable.append(__defaultFv[k]);
        }

        name = __ContentName;
        description = __ContentDescription;
        created = QDateTime::currentDateTime();
    }

    bool loadFrom(KStorage * storage) {
        //save pointer to storage
        this->storage = storage;

        //load from storage
        fvTable.clear();
        KStorageContent  content = storage->load(__ContentName, factory);
        if (!content.isEmpty()) {
            name = content.name();
            description = content.description();
            created = content.created();

            QDataStream stream(content);
            stream >> fvTable;
        }
        return !fvTable.isEmpty();
    }
    bool saveTo(KStorage * storage) {
        KStorageContent content(created);
        content.setFactory(factory);
        content.setName(name);
        content.setDescription(description);

        //append content
        QDataStream stream(&content, QIODevice::WriteOnly);
        stream << fvTable;
        return storage->save(content);
    }
    qreal coeff(const QString& nuclide, TransferFactor::ValueType type) const
    {
        QString e = KRadionuclide::elementName(nuclide);
        foreach(FvValue item, fvTable) {
            if (e == item.element) {
                switch(type) {
                case TransferFactor::Forage:
                    return item.forage;
                case TransferFactor::Crops:
                    return item.crops;
                case TransferFactor::Milk:
                    return item.milk;
                case TransferFactor::Meat:
                    return item.meat;
                default:
                    return -1;
                }
            }
        }
        return 0;
    }
    FvValue coeff(const QString& nuclide) const
    {
        QString e = KRadionuclide::elementName(nuclide);
        foreach(FvValue item, fvTable) {
            if (e == item.element) {
                return item;
            }
        }
        return __emptyFv;
    }
};

TransferFactor::TransferFactor(IModelFactory * factory, KStorage * storage)
    : data(new TransferFactorPrivate(factory, storage))
{
}

TransferFactor::TransferFactor(const TransferFactor &rhs) : data(rhs.data)
{
}

TransferFactor &TransferFactor::operator=(const TransferFactor &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

TransferFactor::~TransferFactor()
{
}

bool TransferFactor::save()
{
    return data->saveTo(KStorage::storage());
}
bool TransferFactor::saveDefault()
{
    data->createDefault();
    return data->saveTo(KStorage::storage());
}

bool TransferFactor::load(bool createOnEmpty)
{
    bool res = data->loadFrom(KStorage::storage());
    if (!res && createOnEmpty) {
        data->createDefault();
        res = data->saveTo(KStorage::storage());
    }

    return res;
}

qreal TransferFactor::value(const QString& nuclide, TransferFactor::ValueType type) const
{
    return data->coeff(nuclide, type);
}
FvValue TransferFactor::value(const QString &nuclide) const
{
    return data->coeff(nuclide);
}
