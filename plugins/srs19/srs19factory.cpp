#include <QtDebug>
#include "srs19factory.h"
#include "discharge.h"
#include "iparamseditor.h"
#include "kgroupinfo.h"
#include "kstorage.h"
#include "atmospherictransport.h"
#include "rivertransport.h"
#include "estuarinetransport.h"
#include "coastaltransport.h"
#include "smalllaketransport.h"
#include "largelaketransport.h"

Q_EXPORT_PLUGIN2(srs19, Srs19Factory)

Srs19Factory::~Srs19Factory()
{
    qDebug() << "~Srs19Factory";
    delete _paramsEditor;
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

IParamsEditor * Srs19Factory::paramsEditor()
{
    if (_paramsEditor == 0) {
        //TODO create new

        if (_paramsEditor)
            _paramsEditor->initialize();
    }
    return _paramsEditor;
}

bool Srs19Factory::initialize()
{
     //parameter editor
    _paramsEditor = 0;

    ModelInfoList mi;
    //model information
    mi << KModelInfo(AirDischarge::SerialId, "AD", tr("Atmospheric Discharge"),
                     tr("Discharge of radionuclide to atmosfer"), QIcon(":/srs19/cloud.png"))
       << KModelInfo(WaterDischarge::SerialId, "WD", tr("Surface Water Discharge"),
                     tr("Discharge of radionuclide to water"), QIcon(":/srs19/water.png"))
       << KModelInfo(SewageDischarge::SerialId, "SD", tr("Sewage Discharge"),
                     tr("Discharge of radionuclide to sewage"), QIcon());
    //add to group info
    _groupInfos.append(KGroupInfo(QObject::tr("Discharge"), mi));

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
       << KModelInfo(LargeLakeTransport::SerialId, "LT", tr("Large Pathway"),
                     tr("Radionuclide transport model through large lake pathway"), QIcon(":/srs19/transport.png"));

    _groupInfos.append(KGroupInfo(QObject::tr("Transport"), mi));

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

    case AtmosphericTransport::SerialId:
        return new AtmosphericTransport(this, info);
    case RiverTransport::SerialId:
        return new RiverTransport(this, info);
    case EstuarineTransport::SerialId:
        return new EstuarineTransport(this, info);
    case CoastalTransport::SerialId:
        return new CoastalTransport(this, info);
    case SmallLakeTransport::SerialId:
        return new SmallLakeTransport(this, info);
    case LargeLakeTransport::SerialId:
        return new LargeLakeTransport(this, info);
    default:
        return 0;
    }
}

