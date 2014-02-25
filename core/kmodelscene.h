#ifndef KMODELSCENE_H
#define KMODELSCENE_H

#include <QGraphicsScene>
#include <QExplicitlySharedDataPointer>
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

    void addFactory(IModelFactory * factory);
    int factoryCount() const;
    IModelFactory * factory(int idx) const;
    FactoryList factories() const;
    IModel * createModel(IModelFactory * factory, const KModelInfo& info);
    void copyTo(KModelScene * mscene) const;
    void clearModels();
    void refresh();
    void verify();
    void calculate();
    void pause();
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
    void removeModel(IModel * m);
    void removeConnector(KConnector * con);
    void reannotateModels();

    void connectItem(QGraphicsItem * selItem, const QPointF& scPos);
    void cancelConnection();
    void tryDrawConnector(const QPointF& scPos);
private:
    KModelScenePrivate * data;
};

#endif // KMODELSCENE_H
