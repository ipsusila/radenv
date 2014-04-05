#include <QSharedData>
#include "bioaccumulationfactor.h"
#include "kstorage.h"
#include "kelement.h"
#include "kradionuclide.h"

static const BpValue __emptyBp = {"", -1, -1, -1};
static const char * __ContentName = "Bioaccumulation Factors";
static const char * __ContentDescription = "Equilibrium ratio of the concentration of a radionuclide "
        "in aquatic food p to its dissolved concentration in water";
static const BpValue __defaultBp [] =
{
    {"Ac",15,50,1000},
    {"Ag",5,500,10000},
    {"Am",30,50,20000},
    {"As",500,1000,2000},
    {"At",15,10,50},
    {"Au",35,100,1000},
    {"Ba",4,10,1},
    {"Bi",10,20,1000},
    {"Br",400,3,10},
    {"C",-1,-1,-1},
    {"Cd",200,1000,20000},
    {"Ce",30,50,5000},
    {"Cm",30,50,30000},
    {"Co",300,1000,5000},
    {"Cr",200,200,800},
    {"Cs",2000,100,30},
    {"Cu",200,700,2000},
    {"Eu",50,300,7000},
    {"Fe",200,3000,30000},
    {"Ga",400,700,700},
    {"H",-1,-1,-1},
    {"Hg",1000,20000,20000},
    {"I",40,10,10},
    {"In",10000,1000,10000},
    {"Mn",400,400,5000},
    {"Mo",10,10,100},
    {"Na",20,0.1,0.3},
    {"Nb",300,30,1000},
    {"Ni",100,1000,2000},
    {"Np",30,10,400},
    {"P",50000,30000,20000},
    {"Pa",10,50,500},
    {"Pb",300,200,1000},
    {"Pd",10,300,300},
    {"Pm",30,500,5000},
    {"Po",50,2000,50000},
    {"Pu",30,40,3000},
    {"Ra",50,500,1000},
    {"Rb",2000,100,20},
    {"Rh",10,100,1000},
    {"Ru",10,2,2000},
    {"S",800,2,4},
    {"Sb",100,400,400},
    {"Se",200,6000,6000},
    {"Sr",15,2,2},
    {"Tc",20,30,1000},
    {"Te",400,1000,1000},
    {"Th",100,600,1000},
    {"Tl",1000,5000,5000},
    {"U",10,1,30},
    {"Y",30,20,1000},
    {"Zn",1000,1000,50000},
    {"Zr",300,20,5000}
};

class BioAccumulationFactorPrivate : public QSharedData {
public:
    IModelFactory * factory;
    KStorage * storage;
    QVector<BpValue> bpTable;
    QString name;
    QString description;
    QDateTime created;

    BioAccumulationFactorPrivate(IModelFactory *f, KStorage *s)
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
        int sz = sizeof(__defaultBp) / sizeof(BpValue);
        bpTable.reserve(sz);
        for (int k = 0; k < sz; k++) {
            bpTable.append(__defaultBp[k]);
        }

        name = __ContentName;
        description = __ContentDescription;
        created = QDateTime::currentDateTime();
    }

    bool loadFrom(KStorage * storage) {
        //save pointer to storage
        this->storage = storage;

        //load from storage
        bpTable.clear();
        KStorageContent  content = storage->load(__ContentName, factory);
        if (!content.isEmpty()) {
            name = content.name();
            description = content.description();
            created = content.created();

            QDataStream stream(content);
            stream >> bpTable;
        }
        return !bpTable.isEmpty();
    }
    bool saveTo(KStorage * storage) {
        KStorageContent content(created);
        content.setFactory(factory);
        content.setName(name);
        content.setDescription(description);

        //append content
        QDataStream stream(&content, QIODevice::WriteOnly);
        stream << bpTable;
        return storage->save(content);
    }
    qreal coeff(const QString& nuclide, BioAccumulationFactor::ValueType type) const
    {
        QString e = KRadionuclide::elementName(nuclide);
        foreach(BpValue item, bpTable) {
            if (e == item.element) {
                switch(type) {
                case BioAccumulationFactor::FreshWaterFish:
                    return item.freshwaterFish;
                case BioAccumulationFactor::MarineFish:
                    return item.marineFish;
                case BioAccumulationFactor::MarineShellfish:
                    return item.marineShellfish;
                default:
                    return -1;
                }
            }
        }
        return 0;
    }
    BpValue coeff(const QString& nuclide) const
    {
        QString e = KRadionuclide::elementName(nuclide);
        foreach(BpValue item, bpTable) {
            if (e == item.element) {
                return item;
            }
        }
        return __emptyBp;
    }
};

BioAccumulationFactor::BioAccumulationFactor(IModelFactory * factory, KStorage * storage)
    : data(new BioAccumulationFactorPrivate(factory, storage))
{
}

BioAccumulationFactor::BioAccumulationFactor(const BioAccumulationFactor &rhs) : data(rhs.data)
{
}

BioAccumulationFactor &BioAccumulationFactor::operator=(const BioAccumulationFactor &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

BioAccumulationFactor::~BioAccumulationFactor()
{
}

bool BioAccumulationFactor::save()
{
    return data->saveTo(KStorage::storage());
}
bool BioAccumulationFactor::saveDefault()
{
    data->createDefault();
    return data->saveTo(KStorage::storage());
}

bool BioAccumulationFactor::load(bool createOnEmpty)
{
    bool res = data->loadFrom(KStorage::storage());
    if (!res && createOnEmpty) {
        data->createDefault();
        res = data->saveTo(KStorage::storage());
    }

    return res;
}

qreal BioAccumulationFactor::value(const QString& nuclide, BioAccumulationFactor::ValueType type) const
{
    return data->coeff(nuclide, type);
}
BpValue BioAccumulationFactor::value(const QString &nuclide) const
{
    return data->coeff(nuclide);
}

