#ifndef KSCENARIO_H
#define KSCENARIO_H

#include <QGraphicsScene>
#include "imodelfactory.h"
#include "kcalculationinfo.h"
#include "iserializable.h"

class KScenarioPrivate;
class RADENV_API KScenario : public QGraphicsScene, public ISerializable
{
public:
    enum EditMode {
        None,
        Connect,
        RemoveConnection,
        RemoveModel
    };

    KScenario(int x, int y, int w, int h, QObject * parent);
    virtual ~KScenario();
    IModel * createModel(IModelFactory * factory, const KModelInfo& info);
    int copyModelsTo(KScenario * mscene, bool all) const;
    void clearModels();
    void refresh();
    void verify();
    void evaluate();
    void pause();
    void resume();
    void stop();
    ReportList reports() const;
    void setReport(KReport * rep);

    void setAssessment(KAssessment * aP);
    KAssessment * assessment() const;

    const CalculationList & calculationResults() const;
    QString name() const;
    void setName(const QString& name);
    QString description() const;
    void setDescription(const QString& desc);

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
    KScenarioPrivate * dptr;
};

#endif // KSCENARIO_H
