#include <QDebug>
#include <QStringBuilder>
#include <QDir>
#include <marble/MarbleModel.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataStyle.h>
#include <marble/AbstractFloatItem.h>
#include "mapview.h"
#include "koutput.h"
#include "kscenario.h"
#include "imodel.h"
#include "kmodelinfo.h"
#include "kdata.h"
#include "kapplication.h"
#include "koptions.h"
#include "dialogpicklocation.h"
#include "kstorage.h"

class MyPlaceMark : public GeoDataPlacemark
{
public:
    MyPlaceMark(const ModelLocation & modLoc) : GeoDataPlacemark()
    {
        setData(modLoc);
    }

    void setData(const ModelLocation & modLoc)
    {
        KLocation loc = modLoc.location;
        IModel * model = modLoc.model;
        QString desc = loc.description() + "<br />";

        //assign coordinate
        setCoordinate(loc.longitude(), loc.latitude(), 0, GeoDataCoordinates::Degree);

        //set name
        if (model->isSource())
        {
            setName(loc.name() + " (Source) ");
        }
        else
        {
            setName(loc.name());
        }

        //add description
        if (model != 0)
        {
            desc += model->info().description() + "<br />";

            //user inputs
            const KDataGroupArray & dga = model->constUserInputs();
            if (!dga.isEmpty())
            {
                desc += dga.toHtml() + "<br />";
            }

            if (model->isSource())
            {
                KDataArray da = model->result();
                if (!da.isEmpty())
                {
                    desc += da.toHtml() + "<br />";
                }
            }
            else
            {
                //crate tables
                KDataTable table;
                KDataArray da;
                model->result().separateTo(&da, &table);
                table.transpose();

                if(!da.isEmpty())
                {
                    desc += da.toHtml();
                    desc += "<br />";
                }
                if (!table.isEmpty())
                {
                    desc += table.toHtml();
                }
            }
        }
        setDescription(desc);
    }

    ~MyPlaceMark()
    {
        qDebug() << "#Removing place mark " << name();
    }
};

MapView::MapView(QWidget *parent) :
    MarbleWidget(parent), mCurrentScenario(0),
    mLastDocument(0), mInteractionMode(Default)
{
    setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    setProjection(Mercator);
    setShowBackground(true);
    setShowBorders(true);
    setShowLakes(true);
    setShowTerrain(true);
    setShowPlaces(true);
    setShowRivers(true);
    setShowCrosshairs(false);

    //get home map
    //TODO : move to options
    QDir dir(KApplication::selfInstance()->applicationDirPath());
    dir.cd("map");
    if (dir.exists())
    {
        const char * homeMap = "home.osm";
        QString homeMapFile = dir.absoluteFilePath(homeMap);
        if (QFile::exists(homeMapFile))
        {
            model()->addGeoDataFile(homeMapFile);
        }
    }

    GeoDataCoordinates home(106.674, -6.33792, 0.0, GeoDataCoordinates::Degree);
    centerOn(home);
    setZoom(2588);

    //PRSG
    //-6.352116, 106.663012
    GeoDataDocument * defDoc = new GeoDataDocument();
    GeoDataPlacemark * prsg = new GeoDataPlacemark("BATAN");
    prsg->setCoordinate(106.663012, -6.352116, 0, GeoDataCoordinates::Degree);
    prsg->setDescription("BATAN Serpong, Tangerang Selatan, Banten");
    defDoc->append(prsg);
    model()->treeModel()->addDocument(defDoc);

    connect(this, SIGNAL(mouseMoveGeoPosition(QString)), this, SLOT(handleMouseGeoposition(QString)));
    connect(this, SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),
            this, SLOT(handleMouseClickPosition(qreal,qreal,GeoDataCoordinates::Unit)));

}
MapView::InteractionMode MapView::interactionMode() const
{
    return this->mInteractionMode;
}
void MapView::pickLocationClicked(bool clicked)
{
    if (clicked)
        mInteractionMode = PickLocation;
    else
        mInteractionMode = Default;
}
void MapView::trackLocationClicked(bool clicked)
{
    if (clicked)
        mInteractionMode = Track;
    else
        mInteractionMode = Default;
}

void MapView::handleMouseGeoposition(const QString &strInfo)
{
    if (mInteractionMode == MapView::Track) {
        bool success;
        GeoDataCoordinates coordinate = GeoDataCoordinates::fromString(strInfo, success);
        if (success)
        {
            emit geopositionChanged(coordinate.toString(GeoDataCoordinates::Decimal));
        }
        else
        {
            emit geopositionChanged(QString::null);
        }
    }
}
void MapView::handleMouseClickPosition(qreal lon, qreal lat, GeoDataCoordinates::Unit unit)
{
    GeoDataCoordinates coord(lon, lat, unit);
    xTrace() << "Clicked position, longitude " << coord.longitude(GeoDataCoordinates::Degree)
             << " and latitude is " << coord.latitude(GeoDataCoordinates::Degree);

    if (mInteractionMode == PickLocation)
    {
        //pick additional info
        DialogPickLocation dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            KLocation loc = dlg.location();
            KLocation srcLoc = dlg.sourceLocation();
            loc.setLongitude(coord.longitude(GeoDataCoordinates::Degree));
            loc.setLatitude(coord.latitude(GeoDataCoordinates::Degree));

            if (srcLoc.isValid())
            {
                //calculate angle, distance
                GeoDataCoordinates srcCoord(srcLoc.longitude(), srcLoc.latitude(), 0, GeoDataCoordinates::Degree);
                GeoDataLineString line;
                line.append(srcCoord);
                line.append(coord);
                qreal distance = line.length(model()->planetRadius());
                xTrace() << "Distance between " << srcLoc.name() << " and " << loc.name() << " is " << distance;
                loc.addDistance(distance);
            }
            else
            {
                //
            }
            //save location
            KApplication::selfInstance()->storage()->save(loc);
        }
    }
}
void MapView::displayScenario(KScenario *scenario)
{
     //remove document
    if (mLastDocument == 0)
    {
        mLastDocument = new GeoDataDocument();
    }
    else
    {
        model()->treeModel()->removeDocument(mLastDocument);
        mLastDocument->clear();
        qDebug() << "#Latest GeoDocument removed....";
    }

    mCurrentScenario = scenario;
    if (scenario == 0)
        return;

    //extract all locations
    QList<ModelLocation> modelLocList = scenario->extractLocations();
    //display locations and results
    GeoDataPlacemark * lastMark = 0;
    for(int i = modelLocList.count() - 1; i >= 0; i--)
    {
        MyPlaceMark * mark = new MyPlaceMark(modelLocList.at(i));
        mLastDocument->append(mark);
        lastMark = mark;
    }
    model()->treeModel()->addDocument(mLastDocument);
    if (lastMark != 0)
        centerOn(*lastMark, true);
}
