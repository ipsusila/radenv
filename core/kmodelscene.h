#ifndef KMODELSCENE_H
#define KMODELSCENE_H

#include <QGraphicsScene>
#include "imodelfactory.h"
#include "kcalculationinfo.h"

class KModelScenePrivate;
class K_CORE_EXPORT KModelScene : public QGraphicsScene
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

    void connectItem(QGraphicsItem * selItem, const QPointF& scPos);
    void cancelConnection();
    void tryDrawConnector(const QPointF& scPos);
private:
    KModelScenePrivate * data;
};

#endif // KMODELSCENE_H
