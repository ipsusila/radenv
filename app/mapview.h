#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <marble/MarbleWidget.h>
#include <marble/GeoDataDocument.h>

using namespace Marble;

class KScenario;
class MapView : public MarbleWidget
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = 0);

    enum InteractionMode
    {
        Default,
        Track,
        PickLocation
    };

    InteractionMode interactionMode() const;
    
signals:
    void geopositionChanged(const QString & positionInfo);
public slots:
    void handleMouseGeoposition(const QString & strInfo);
    void handleMouseClickPosition(qreal lon, qreal lat, GeoDataCoordinates::Unit unit);
    void displayScenario(KScenario * scenario);
    void pickLocationClicked(bool clicked);
    void trackLocationClicked(bool clicked);

private:
    KScenario *mCurrentScenario;
    GeoDataDocument * mLastDocument;
    InteractionMode mInteractionMode;

};

#endif // MAPVIEW_H
