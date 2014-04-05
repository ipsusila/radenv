#include <QtDebug>
#include "srs19factory.h"
#include "symbol.h"
#include "discharge.h"
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
#include "distributioncoefficient.h"
#include "transferfactor.h"
#include "sedimenteffect.h"
#include "constantvalue.h"
#include "irrigation.h"
#include "vegetation.h"
#include "bioaccumulationfactor.h"
#include "storedforage.h"
#include "animalfeed.h"
#include "animaltransport.h"

#include <stdio.h>


Q_EXPORT_PLUGIN2(srs19, Srs19Factory)

Srs19Factory::~Srs19Factory()
{
    qDebug() << "~Srs19Factory";

    //test
    FILE * fp = fopen("/tmp/srs19.txt", "w");
    if (fp) {
        fprintf(fp, "SRS-19 factory destroyed. plugin unloaded");
        fclose(fp);
    }
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

ConstSymbolList Srs19Factory::availableQuantities() const
{
    return Srs19::availableSymbols();
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

void Srs19Factory::onFinalized()
{
    qDebug() << "Finalizing SRS-19 factory";
    Srs19::saveSymbols(this);

    delete _paramsEditor;
    delete _settingManager;
}

bool Srs19Factory::initialize()
{
    //load or create new default kd tables if does not exist
    DistributionCoefficient kd(this);
    kd.load(true);

    TransferFactor fv(this);
    fv.load(true);

    BioAccumulationFactor bp(this);
    bp.load(true);

    //reload symbols
    Srs19::reloadSymbols(this);

     //parameter editor
    _paramsEditor = 0;

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
       << KModelInfo(SedimentEffect::SerialId, "SE", tr("Sediment effect"),
                     tr("Sediment effect to radionuclide"), QIcon(":/srs19/transport.png"))
       << KModelInfo(Irrigation::SerialId, "IR", tr("Irrigation"),
                     tr("Effect of radionuclide by irrigation"), QIcon(":/srs19/transport.png"));
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
                    tr("Concentration of radionuclide in meat"), QIcon());
    _groupInfos.append(KGroupInfo(tr("Foods"), mi));

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
    case SedimentEffect::SerialId:
        return new SedimentEffect(this, info);
    case Irrigation::SerialId:
        return new Irrigation(this, info);

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

    default:
        return 0;
    }
}

