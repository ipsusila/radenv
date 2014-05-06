#ifndef KMODELSCENE_H
#define KMODELSCENE_H

#include <QGraphicsScene>
#include "imodelfactory.h"
#include "kcalculationinfo.h"
#include "iserializable.h"

class KModelScenePrivate;
class RADENV_API KModelScene : public QGraphicsScene, public ISerializable
{
public:
    enum EditMode {
        None,
        Connect,
        RemoveConnection,
        RemoveModel
    };

    KModelScene(int x, int y, int w, int h);
    virtual ~KModelScene();
    IModel * createModel(IModelFactory * factory, const KModelInfo& info);
    int copyTo(KModelScene * mscene, bool all) const;
    void clearModels();
    void refresh();
    void verify();
    void calculate();
    void pause();
    void resume();
    void stop();

    const CalculationList & calculationResults() const;
    QString sceneName() const;
    void setSceneName(const QString& name);
    QString sceneDescription() const;
    void setSceneDescription(const QString& desc);

    void setDisplayGrid(bool v);
    void setSnapToGrid(bool v);
    bool displayGrid() const;
    bool snapToGrid() const;
    int grid() const;
    void setGrid(int px);

    void generateReport();
    EditMode editMode() const;
    void setEditMode(EditMode mode);
    void removeModel(IModel * model, bool redraw);
    void removeConnector(KConnector * con);
    void reannotateModels();
    ModelList models() const;
    IModel * findModel(const QString& tagName) const;
    ConnectorList connectors() const;

    void connectItem(QGraphicsItem * selItem, const QPointF& scPos);
    void cancelConnection();
    void tryDrawConnector(const QPointF& scPos);

    virtual QDataStream & serialize(QDataStream & stream) const;
    virtual QDataStream & deserialize(QDataStream & stream);
private:
    KModelScenePrivate * data;
};

#endif // KMODELSCENE_H
