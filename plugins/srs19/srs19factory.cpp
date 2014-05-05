#include <QtDebug>
#include "srs19factory.h"
#include "quantity.h"
#include "discharge.h"
#include "sewagedischarge.h"
#include "iparamseditor.h"
#include "kgroupinfo.h"
#include "kstorage.h"
#include "ksettingmanager.h"
#include "atmospherictransport.h"
#include "rivertransport.h"
#include "estuarinetransport.h"
#include "coastaltransport.h"
#include "smalllaketransport.h"
#include "largelaketransport.h"
#include "sedimenteffect.h"
#include "constantvalue.h"
#include "irrigation.h"
#include "vegetation.h"
#include "storedforage.h"
#include "animalfeed.h"
#include "animaltransport.h"
#include "sludgetransport.h"
#include "grounddeposition.h"
#include "aquaticfoodtransport.h"
#include "defaultvalues.h"
#include "doseestimation.h"

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(srs19, Srs19Factory)
#endif

Srs19Factory::Srs19Factory() : _paramsEditor(0), _settingManager(0),
    _manager(0), _storage(0)
{
    qDebug() << "#Creating factory instance: " << name();
}

Srs19Factory::~Srs19Factory()
{
    //nothing todo
}

void Srs19Factory::setManager(KPluginManager * mngr)
{
    _manager = mngr;
}

KPluginManager * Srs19Factory::manager() const
{
    return _manager;
}
KStorage * Srs19Factory::storage() const
{
    return _storage;
}

QString Srs19Factory::name() const
{
    return "SRS-19";
}
QString Srs19Factory::author() const
{
    return tr("I Putu Susila");
}
QString Srs19Factory::version() const
{
    return "1.0.0";
}
QString Srs19Factory::description() const
{
    return tr("Generic models for use in assessing the impact of discharges "
              "of radioactive substances to the environment. IAEA, 2001.");
}

bool Srs19Factory::isTopLevel() const
{
    return true;
}

const GroupInfoList & Srs19Factory::groups() const
{
    return _groupInfos;
}

ConstQuantityList Srs19Factory::availableQuantities() const
{
    return Srs19::availableQuantities();
}
QuantityList Srs19Factory::editableQuantities() const
{
    return Srs19::editableQuantities();
}

IParamsEditor * Srs19Factory::paramsEditor()
{
    if (_paramsEditor == 0) {
        //TODO create new

        if (_paramsEditor)
            _paramsEditor->initialize();
    }
    return _paramsEditor;
}

KSettingManager * Srs19Factory::settingManager()
{
    return _settingManager;
}

void Srs19Factory::finalize()
{
    qDebug() << "Finalizing SRS-19 factory";
    delete _paramsEditor;
    delete _settingManager;
}

KModelInfo Srs19Factory::modelInfo(int serId) const
{
    for(int j = _groupInfos.size() - 1; j >= 0; j--) {
        const KGroupInfo & gi = _groupInfos.at(j);
        const KModelInfo * mi = gi.info(serId);
        if (mi != 0)
            return *mi;
    }
    return KModelInfo();
}
void Srs19Factory::attachStorage(KStorage * stg)
{
    //assign storage
    //TODO
    _storage = stg;

    //load or create new default kd tables if does not exist
   DistributionCoefficient kd(this);
   kd.load(true);

   TransferFactor fv(this);
   fv.load(true);

   BioAccumulationFactor bp(this);
   bp.load(true);

   ExternalDoseCoefficient edc(this);
   edc.load(true);

   //reload quantities
   Srs19::reloadQuantities(this);

   _settingManager->load();
}

void Srs19Factory::detachStorage()
{
    Srs19::saveQuantities(this);
    _settingManager->save();

    //save other quantities
}

bool Srs19Factory::initialize()
{
     //parameter editor
    _paramsEditor = 0;
    _manager = 0;
    _storage = 0;

    //setting manager
    _settingManager = new KSettingManager(this);

    ModelInfoList mi;
    //model information
    mi << KModelInfo(AirDischarge::SerialId, "AD", tr("Atmospheric Discharge"),
                     tr("Discharge of radionuclide to atmosfer"), QIcon(":/srs19/cloud.png"))
       << KModelInfo(WaterDischarge::SerialId, "WD", tr("Surface Water Discharge"),
                     tr("Discharge of radionuclide to water"), QIcon(":/srs19/water.png"))
       << KModelInfo(SewageDischarge::SerialId, "SD", tr("Sewage Discharge"),
                     tr("Discharge of radionuclide to sewage"), QIcon())
       << KModelInfo(ConstantValue::SerialId, "C", tr("Constant value"),
                      tr("User defined constant value(s)"), QIcon(":/srs19/transport.png"));
    //add to group info
    _groupInfos.append(KGroupInfo(QObject::tr("Source"), mi));

    mi.clear();
    mi << KModelInfo(AtmosphericTransport::SerialId, "AT", tr("Atmospheric Pathway"),
                     tr("Radionuclide transport model through atmospheric pathway"), QIcon(":/srs19/atmospheric.png"))
       << KModelInfo(RiverTransport::SerialId, "RT", tr("River Pathway"),
                     tr("Radionuclide transport model through river pathway"), QIcon(":/srs19/river.png"))
       << KModelInfo(EstuarineTransport::SerialId, "ET", tr("Estuarine Pathway"),
                     tr("Radionuclide transport model through estuarine pathway"), QIcon(":/srs19/transport.png"))
       << KModelInfo(CoastalTransport::SerialId, "CT", tr("Coastal Water Pathway"),
                     tr("Radionuclide transport model through coastal water"), QIcon(":/srs19/transport.png"))
       << KModelInfo(SmallLakeTransport::SerialId, "ST", tr("Small Lake Pathway"),
                     tr("Radionuclide transport model through small lake pathway"), QIcon(":/srs19/transport.png"))
       << KModelInfo(LargeLakeTransport::SerialId, "LT", tr("Large Lake Pathway"),
                     tr("Radionuclide transport model through large lake pathway"), QIcon(":/srs19/transport.png"))
       << KModelInfo(GenericCoastalTransport::SerialId, "GCT", tr("Generic Large Lake/Coastal Water Pathway"),
                     tr("Radionuclide transport model through large lake/coastal water"), QIcon(":/srs19/transport.png"))
       << KModelInfo(FreshwaterSediment::SerialId, "FS", tr("Freshwater sediment"),
                     tr("Sediment effect to radionuclide at freshwater"), QIcon(":/srs19/transport.png"))
       << KModelInfo(CoastalSediment::SerialId, "CS", tr("Coastal sediment"),
                     tr("Sediment effect to radionuclide at coastal"), QIcon(":/srs19/transport.png"))
       << KModelInfo(Irrigation::SerialId, "IR", tr("Irrigation"),
                     tr("Effect of radionuclide by irrigation"), QIcon(":/srs19/transport.png"))
       << KModelInfo(SludgeTransport::SerialId, "SL", tr("Surface sewage sludge"),
                     tr("Surface concentration per unit area of sewage sludge"), QIcon(":/srs19/transport.png"))
       << KModelInfo(GroundDeposition::SerialId, "GR", tr("Ground deposition"),
                     tr("Deposition of radionuclide in surface soil"), QIcon(":/srs19/transport.png"));
    _groupInfos.append(KGroupInfo(QObject::tr("Transport"), mi));

    mi.clear();
    mi << KModelInfo(Crop::SerialId, "CR", tr("Crops"),
                    tr("Accumulation of radionuclide in crops"), QIcon(":/srs19/transport.png"))
        << KModelInfo(Forage::SerialId, "FR", tr("Forage"),
                    tr("Accumulation of radionuclide in forages"), QIcon(":/srs19/transport.png"))
        << KModelInfo(StoredForage::SerialId, "SF", tr("Stored Forage"),
                    tr("Concentration of radionuclide in stored animal feeds"), QIcon())
        << KModelInfo(AnimalFeed::SerialId, "AF", tr("Animal Feed"),
                    tr("Concentration of radionuclide in animal feeds"), QIcon())
        << KModelInfo(MilkTransport::SerialId, "MT", tr("Milk"),
                    tr("Concentration of radionuclide in milk"), QIcon())
        << KModelInfo(MeatTransport::SerialId, "FT", tr("Meat"),
                    tr("Concentration of radionuclide in meat"), QIcon())
        << KModelInfo(FreshwaterFishTransport::SerialId, "FW", tr("Freshwater fish"),
                    tr("Concentration of radionuclide in freshwater fish"), QIcon())
        << KModelInfo(MarineFishTransport::SerialId, "MF", tr("Marine fish"),
                    tr("Concentration of radionuclide in marine fish"), QIcon())
        << KModelInfo(MarineShellfishTransport::SerialId, "SF", tr("Marine shellfish"),
                    tr("Concentration of radionuclide in marine shellfish"), QIcon()) ;
    _groupInfos.append(KGroupInfo(tr("Foods"), mi));

    mi.clear();
    mi << KModelInfo(InfantDoseEstimation::SerialId, "IF", tr("Infant"),
                    tr("Infant dose estimation"), QIcon(":/srs19/transport.png"))
        << KModelInfo(AdultDoseEstimation::SerialId, "AU", tr("Adult"),
                    tr("Adult dose estimation"), QIcon(":/srs19/transport.png"));
    _groupInfos.append(KGroupInfo(tr("Doses"), mi));

    return true;
}

IModel * Srs19Factory::createModel(const KModelInfo & info)
{
    switch(info.serialId()) {
    case AirDischarge::SerialId:
        return new AirDischarge(this, info);
    case WaterDischarge::SerialId:
        return new WaterDischarge(this, info);
    case SewageDischarge::SerialId:
        return new SewageDischarge(this, info);
    case ConstantValue::SerialId:
        return new ConstantValue(this, info);

    case AtmosphericTransport::SerialId:
        return new AtmosphericTransport(this, info);
    case RiverTransport::SerialId:
        return new RiverTransport(this, info);
    case EstuarineTransport::SerialId:
        return new EstuarineTransport(this, info);
    case CoastalTransport::SerialId:
        return new CoastalTransport(this, info);
    case GenericCoastalTransport::SerialId:
        return new GenericCoastalTransport(this, info);
    case SmallLakeTransport::SerialId:
        return new SmallLakeTransport(this, info);
    case LargeLakeTransport::SerialId:
        return new LargeLakeTransport(this, info);
    case FreshwaterSediment::SerialId:
        return new FreshwaterSediment(this, info);
    case CoastalSediment::SerialId:
        return new CoastalSediment(this, info);
    case Irrigation::SerialId:
        return new Irrigation(this, info);
    case SludgeTransport::SerialId:
        return new SludgeTransport(this, info);
    case GroundDeposition::SerialId:
        return new GroundDeposition(this, info);

    case Crop::SerialId:
        return new Crop(this, info);
    case Forage::SerialId:
        return new Forage(this, info);
    case StoredForage::SerialId:
        return new StoredForage(this, info);
    case AnimalFeed::SerialId:
        return new AnimalFeed(this, info);
    case MilkTransport::SerialId:
        return new MilkTransport(this, info);
    case MeatTransport::SerialId:
        return new MeatTransport(this, info);
    case FreshwaterFishTransport::SerialId:
        return new FreshwaterFishTransport(this, info);
    case MarineFishTransport::SerialId:
        return new MarineFishTransport(this, info);
    case MarineShellfishTransport::SerialId:
        return new MarineShellfishTransport(this, info);

    case InfantDoseEstimation::SerialId:
        return new InfantDoseEstimation(this, info);
    case AdultDoseEstimation::SerialId:
            return new AdultDoseEstimation(this, info);

    default:
        return 0;
    }
}

