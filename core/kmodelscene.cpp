#include <QtDebug>
#include <QPluginLoader>
#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>
#include <QThread>
#include <QSet>
#include "kmodelscene.h"
#include "imodel.h"
#include "kmodelinfo.h"
#include "koutput.h"
#include "kport.h"
#include "kconnector.h"
#include "dialogcalculation.h"

class KModelScenePrivate
{
    bool _snapToGrid;
    int _gridPixel;
    bool _displayGrid;
    KModelScene::EditMode _editMode;
    KConnector * _tempConnector;
    DialogCalculation  * _calcDialog;
    FactoryList _factoryList;
    QStringList _modelNames;
    ModelList _verifiedNodes;     ///< List of verified nodes
    CalculationList _calcInfos;

public:
    KModelScenePrivate() : _snapToGrid(true), _gridPixel(20),
        _displayGrid(true), _editMode(KModelScene::None),
        _tempConnector(0), _calcDialog(0)
    {
    }
    ~KModelScenePrivate() {
        delete _calcDialog;
    }

    inline bool snapToGrid() const { return _snapToGrid; }
    inline void setSnapToGrid(bool v) { _snapToGrid = v;}
    inline int gridPixel() const { return _gridPixel; }
    inline void setGridPixel(int px) { _gridPixel = px; }
    inline bool displayGrid() const { return _displayGrid; }
    inline void setDisplayGrid(bool v) { _displayGrid = v; }
    inline QList<IModel *> * verifiedNodes() { return &_verifiedNodes; }
    inline KModelScene::EditMode editMode() const { return _editMode; }
    inline KConnector * tempConnector() const { return _tempConnector; }

    inline void setEditMode(KModelScene::EditMode mode) {
        _editMode = mode;
    }
    inline void setTempConnector(KConnector * con) {
        _tempConnector = con;
    }
    inline void clearTempConnector() {
        _tempConnector = 0;
    }

    inline void addFactory(IModelFactory * factory) {
        _factoryList.append(factory);
    }
    inline int factoryCount() const {
        return _factoryList.size();
    }
    inline IModelFactory * factory(int idx) const
    {
        return _factoryList.at(idx);
    }
    inline const FactoryList & factories() const
    {
        return _factoryList;
    }
    inline void addCalculationInfo(const KCalculationInfo& info)
    {
        this->_calcInfos.append(info);
    }
    inline void clearCalculationInfo()
    {
        _calcInfos.clear();
    }
    inline const CalculationList & calculationInfo() const
    {
        return _calcInfos;
    }
    inline DialogCalculation * calculationDialog()
    {
        if (_calcDialog == 0)
            _calcDialog = new DialogCalculation();
        return _calcDialog;
    }

    inline IModel * createModel(IModelFactory * factory, const KModelInfo& info)
    {
        IModel * model = factory->createModel(info);
        if (model) {
            if (!model->initialize()) {
                delete model;
                return 0;
            }

            //add annotation
            annotateModel(model);
            return model;
        }
        return 0;
    }
    inline void annotateModel(IModel * model) {
        QString name = model->info().name();
        _modelNames.append(name);
        model->setTagId(_modelNames.count(name));
    }

    inline void clearModels()
    {
        _modelNames.clear();
        _verifiedNodes.clear();
        _calcInfos.clear();
    }
};

KModelScene::KModelScene(int x, int y, int w, int h)
    : QGraphicsScene(x, y, w, h), data(new KModelScenePrivate)
{
    this->setFont(QFont("monospace", 10));
}

KModelScene::~KModelScene()
{
    delete data;
    qDebug() << "Scene destroyed";
}

void KModelScene::copyTo(KModelScene * mscene) const
{
    Q_ASSERT(mscene);

    //copy list
    QList<QPair<IModel *, IModel *> > copyList;

    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * src = qgraphicsitem_cast<IModel *>(item);
        if (src && src->isSelected()) {
            IModel * dest = src->copyTo(mscene);
            copyList << qMakePair(src, dest);
        }
    }

    //TODO
    //copy connection
    for(int k = 0; k < copyList.size(); k++) {
        IModel * src = copyList[k].first;
        IModel * dest = copyList[k].second;

        //check connections
    }


    //verify (visit)

}

void KModelScene::addFactory(IModelFactory *factory)
{
    data->addFactory(factory);
}
int KModelScene::factoryCount() const
{
    return data->factoryCount();
}
IModelFactory * KModelScene::factory(int idx) const
{
    return data->factory(idx);
}
FactoryList KModelScene::factories() const
{
    return data->factories();
}

IModel * KModelScene::createModel(IModelFactory * factory, const KModelInfo& info)
{
    IModel * model = data->createModel(factory, info);
    if (model)
        this->addItem(model);
    return model;
}
void KModelScene::clearModels()
{
    data->clearModels();
    this->clear();
}
void KModelScene::refresh()
{
    //do refresh
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md)
            md->refresh();
    }
}
void KModelScene::generateReport()
{
    ModelList * vNodes = data->verifiedNodes();
    if (vNodes->isEmpty()) {
        xWarning() << tr("No report generated. Model(s) not verified and/or calculated");
        return;
    }
    //get reports
    QSet<KReport *> reports;
    foreach(IModel * model, *vNodes) {
        KReport * rep = model->report();
        if (rep)
            reports.insert(rep);
    }

    //initialized reports
    foreach(KReport * rep, reports) {
        rep->beginReport();
    }

    //call generate report
    foreach(IModel * model, *vNodes) {
        model->generateReport();
    }

    //output reports
    foreach(KReport * rep, reports) {
        rep->endReport();
    }
}

void KModelScene::verify()
{
    ModelList * vNodes = data->verifiedNodes();
    vNodes->clear();

    if (this->items().isEmpty())
        return;

    xInfo() << tr("Model verification started...");

    //search node which is sources and visit
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0 && md->isSource()) {
            md->visit(0);
            vNodes->append(md);
        }
    }
    xTrace() << QObject::tr("Total source model count is ") << vNodes->size();

    //add node which is not sources
    foreach(QGraphicsItem * item, list) {
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0 && !md->isSource()) {
            //search appropriate position
            for (int k = 0; k < vNodes->size(); k++) {
                IModel * vmd = vNodes->at(k);
                if (vmd->sourceDistance() > md->sourceDistance()) {
                    vNodes->insert(k, md);
                    md = 0;
                    break;
                }
            }

            //appropriate position not found, append at last
            if (md)
                vNodes->append(md);
        }
    }
    xInfo() << QObject::tr("Total model count is ") << vNodes->size();

    bool result = true;
    int tErr = 0, tWarn = 0;
    int err, warn;
    foreach(IModel * md, *vNodes) {
        err = 0; warn = 0;
        result = md->verify(&err, &warn) && result;
        tErr += err;
        tWarn += warn;
        xInfo() << "---";
    }

    if (result) {
        if (tWarn > 0)
            xInfo() << QString(tr("Model verification succeeded, with %1 warning(s).")).arg(tWarn);
        else
            xInfo() << tr("Model verification succeeded. ");
    }
    else {
        //clear nodes
        vNodes->clear();
        xError() << QString(tr("Model verification failed. %1 error(s), "
                               "%2 warning(s) found")).arg(tErr).arg(tWarn);
    }
}
void KModelScene::pause()
{
    //todo
}

void KModelScene::stop()
{
    //todo
}

void KModelScene::calculate()
{
    if (this->items().isEmpty())
        return;

    ModelList * vNodes = data->verifiedNodes();
    if (vNodes->isEmpty()) {
        verify();
        if (vNodes->isEmpty()) {
            xError() << QObject::tr("Calculation can not be performed due to some error(s) in the model network.");
            return;
        }
    }

    //show run dialog
    DialogCalculation * dlg = data->calculationDialog();
    if (dlg->exec() != QDialog::Accepted)
        return;

    //clear calculation info
    data->clearCalculationInfo();

    //calculation result flags
    bool result = true;

    //begin calculation loop
    KCalculationInfo ci = dlg->calculationInfo();
    int msec = ci.intervalMilisecond();
    int runId = ci.runId();
    int runCnt = ci.runCount();
    while (runId < runCnt || ci.isContinuous()) {
        xInfo() << tr("Calculating dose...") << runId;
        foreach(IModel * md, *vNodes) {
            KCalculationInfo mci(md, ci.intervalMilisecond(), runCnt, runId, ci.continueOnError());
            result = md->calculate(mci) && result;
            if (!result && !ci.continueOnError())
                goto _end_;

            //set calculation results
            mci.setResult(md->result());

            //save calculation results
            data->addCalculationInfo(mci);
        }
        //process pending events
        QCoreApplication::processEvents();

        //TODO
        //if (nsec > 0)
        //    QThread::sleep(nsec);

        runId ++;
    }

_end_:
    xInfo() << "---";
    if (result)
        xInfo() << tr("Calculation finished.");
    else
        xError() << tr("Calculation not finished. Some error(s) found");
}

const CalculationList & KModelScene::calculationResults() const
{
    return data->calculationInfo();
}

bool KModelScene::snapToGrid() const
{
    return data->snapToGrid();
}
void KModelScene::setSnapToGrid(bool v)
{
    data->setSnapToGrid(v);
}

int KModelScene::grid() const
{
    return data->gridPixel();
}
void KModelScene::setGrid(int px)
{
    data->setGridPixel(px);
}
bool KModelScene::displayGrid() const
{
    return data->displayGrid();
}
void KModelScene::setDisplayGrid(bool v)
{
    data->setDisplayGrid(v);
}

KModelScene::EditMode KModelScene::editMode() const
{
    return data->editMode();
}
void KModelScene::setEditMode(KModelScene::EditMode mode)
{
    EditMode oldMode = editMode();
    if (oldMode == mode)
        return;

    //now set edit mode
    data->setEditMode(mode);
    KConnector * con = data->tempConnector();
    if (oldMode == Connect && con != 0) {
        data->clearTempConnector();
        removeItem(con);
        delete con;
        update();
    }
}
void KModelScene::removeModel(IModel *m)
{
    Q_ASSERT(m);

    //remove connections
    m->removeConnections();

    //remove model
    data->verifiedNodes()->removeOne(m);
    this->removeItem(m);
    delete m;
    this->update();
}

void KModelScene::removeConnector(KConnector *con)
{
    Q_ASSERT(con);

    //this should not happen
    //to make sure everything is OK
    if (con == data->tempConnector())
        data->clearTempConnector();

    con->disconnect();
    this->removeItem(con);
    delete con;
    this->update();
}

void KModelScene::connectItem(QGraphicsItem * selItem, const QPointF& scPos)
{
    if (data->editMode() == Connect) {
        KPort * port = qgraphicsitem_cast<KPort*>(selItem);
        //xTrace() << "Make connection from/to port: " << port->label() << " at " << scPos;

        KConnector * con = data->tempConnector();
        if (con == 0) {
            //only handle if port is not null
            if (port == 0)
                return;

            con = new KConnector();
            data->setTempConnector(con);
            this->addItem(con);
            con->setPort(port);

            xTrace() << "Connect port :" << port->symbol()->symbol;
        }
        else {
            //we have temporary connector
            if (port != 0) {
                //assign port
                //make connection if needed
                con->setPort(port);
                if (con->isConnected()) {
                    data->clearTempConnector();
                }
                xTrace() << "Close connection to port " << port->symbol()->symbol;
            }
            else {
                //add point
                con->addPoint(scPos);
                xTrace() << "Adding point : " << scPos.x() << "," << scPos.y();
            }
        }
    }
}
void KModelScene::cancelConnection()
{
    KConnector * con = data->tempConnector();
    if (con != 0) {
        data->clearTempConnector();
        removeItem(con);
        delete con;
    }
}

void KModelScene::tryDrawConnector(const QPointF& scPos)
{
    if (data->editMode() == Connect) {
        KConnector * con = data->tempConnector();
        if (con != 0) {
            con->setTemporaryPoint(scPos);
        }
    }
}
void KModelScene::reannotateModels()
{
    //clear annotation list and verified list
    data->clearModels();

    //reannotate all models
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0)
            data->annotateModel(md);
    }
    this->update();
}

