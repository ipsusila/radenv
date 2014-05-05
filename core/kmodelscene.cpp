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
#include "kmodelrunner.h"
#include "kpluginmanager.h"

//Stream token
static const QString __sceneToken = "@Scene-v.1.0";

class KModelScenePrivate
{
    bool _snapToGrid;
    int _gridPixel;
    bool _displayGrid;
    KModelScene::EditMode _editMode;
    KConnector * _tempConnector;
    DialogCalculation  * _calcDialog;
    KModelRunner * _modelRunner;

    QStringList _modelNames;
    ModelList _verifiedNodes;     ///< List of verified nodes
    CalculationList _calcInfos;

public:
    KModelScenePrivate() : _snapToGrid(true), _gridPixel(20),
        _displayGrid(true), _editMode(KModelScene::None),
        _tempConnector(0), _calcDialog(0), _modelRunner(0)
    {
    }
    ~KModelScenePrivate() {
        stopCalculation();
        delete _modelRunner;
        delete _calcDialog;
    }

    inline bool snapToGrid() const { return _snapToGrid; }
    inline void setSnapToGrid(bool v) { _snapToGrid = v;}
    inline int gridPixel() const { return _gridPixel; }
    inline void setGridPixel(int px) { _gridPixel = px; }
    inline bool displayGrid() const { return _displayGrid; }
    inline void setDisplayGrid(bool v) { _displayGrid = v; }
    inline ModelList * verifiedNodes() { return &_verifiedNodes; }
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

    inline void serialize(QDataStream & s) const
    {
        s << __sceneToken << _snapToGrid << _gridPixel
          << _displayGrid << (qint32)_editMode << _modelNames;
    }
    inline void deserialize(QDataStream & s)
    {
        QString token;
        s >> token;
        Q_ASSERT(token == __sceneToken);

        qint32 mode;
        s >> _snapToGrid >> _gridPixel
          >> _displayGrid >> mode >> _modelNames;
        _editMode = (KModelScene::EditMode)mode;
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

    inline void annotateModel(IModel * model) {
        QString name = model->info().name();
        _modelNames.append(name);
        model->setTagId(_modelNames.count(name));
    }

    inline void clearModelData()
    {
        _modelNames.clear();
        _verifiedNodes.clear();
        _calcInfos.clear();
    }

    inline void stopCalculation(int wms = 2000)
    {
        if (isRunning()) {
            _modelRunner->stop();
            _modelRunner->wait(wms);
        }

    }
    inline bool isRunning() const
    {
        return (_modelRunner != 0 && _modelRunner->isRunning());
    }

    void runCalculation(const KCalculationInfo & ci)
    {
        //clear calculation info
        this->clearCalculationInfo();

        stopCalculation();
        if (_modelRunner)
            delete _modelRunner;
        _modelRunner = new KModelRunner(&_verifiedNodes, ci);
        _modelRunner->start();
    }

    void pause() {
        if (isRunning())
            _modelRunner->pause();
    }

    void resume() {
        if (_modelRunner != 0)
            _modelRunner->resume();
    }
};

KModelScene::KModelScene(int x, int y, int w, int h)
    : QGraphicsScene(x, y, w, h), data(new KModelScenePrivate())
{
    this->setFont(QFont("monospace", 10));
}

KModelScene::~KModelScene()
{
    delete data;
    qDebug() << "Scene destroyed";
}

int KModelScene::copyTo(KModelScene * mscene, bool all) const
{
    Q_ASSERT(mscene);

    //copy selected/all models
    QMap<IModel *, IModel *> copiedModels;
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * src = qgraphicsitem_cast<IModel *>(item);
        if (src && (all || src->isSelected())) {
            IModel * dest = src->copyTo(mscene);
            copiedModels[src] = dest;
        }
    }

    //copy connection
    QMap<IModel *, IModel *>::iterator it = copiedModels.begin();
    QMap<IModel *, IModel *>::iterator end = copiedModels.end();
    while (it != end) {
        IModel * src = it.key();        //model which was copied
        IModel * cpy = it.value();      //copy model
        it++;

        //check connections to output of the model
        //dealing with output ports is enough
        ConnectorList connectors = src->outputConnectors();
        foreach(KConnector * con, connectors) {
            //if connected model is not being copied, ignore connection
            IModel * dest = copiedModels[con->outputPort()->model()];
            if (dest == 0)
                continue;

            //get the port index
            int idxOut = con->outputPort()->index();
            int idxInp = con->inputPort()->index();

            //create connection between cpy and dest
            KConnector * copyCon = new KConnector();
            con->copyTo(copyCon, cpy->outputs().at(idxOut), dest->inputs().at(idxInp));
            mscene->addItem(copyCon);
        }
    }

    return copiedModels.size();
}

ModelList KModelScene::models() const
{
    ModelList list;
    for(int k = 0; k < items().size(); k++) {
        QGraphicsItem * item = items().at(k);
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md)
            list.append(md);
    }
    return list;
}
ConnectorList KModelScene::connectors() const
{
    ConnectorList list;
    for(int k = 0; k < items().size(); k++) {
        QGraphicsItem * item = items().at(k);
        KConnector * con = qgraphicsitem_cast<KConnector *>(item);
        if (con)
            list.append(con);
    }
    return list;
}
IModel * KModelScene::findModel(const QString& tagName) const
{
    for(int k = 0; k < items().size(); k++) {
        QGraphicsItem * item = items().at(k);
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0 && md->tagName() == tagName)
            return md;
    }
    return 0;
}

IModel * KModelScene::createModel(IModelFactory * factory, const KModelInfo& info)
{
    IModel * model = KPluginManager::instance()->createModel(factory, info);
    if (model != 0) {
        data->annotateModel(model);
        addItem(model);
    }
    return model;
}
void KModelScene::clearModels()
{
    ModelList modelList = models();
    for(int k = 0; k < modelList.size(); k++)
        removeModel(modelList.at(k), false);

    data->clearModelData();
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
    //fresh update
    this->update();
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
        xTrace() << *model << " : Report generated";
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
    data->pause();
}

void KModelScene::stop()
{
    data->stopCalculation();
}

void KModelScene::resume()
{
    data->resume();
}

void KModelScene::calculate()
{
    if (this->items().isEmpty())
        return;

    //do verification before calculate
    //if error, return
    ModelList * vNodes = data->verifiedNodes();
    if (vNodes->isEmpty()) {
        verify();
        if (vNodes->isEmpty()) {
            xError() << QObject::tr("Calculation can not be performed due to some error(s) in the model network.");
            return;
        }
    }

    //show run dialog
    //and exec running thread
    DialogCalculation * dlg = data->calculationDialog();
    if (dlg->exec() == QDialog::Accepted)
        data->runCalculation(dlg->calculationInfo());
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
void KModelScene::removeModel(IModel *model, bool redraw)
{
    Q_ASSERT(model);

    //remove connections
    model->removeConnections();

    //remove model
    data->verifiedNodes()->removeOne(model);
    this->removeItem(model);
    delete model;
    if (redraw)
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

            xTrace() << "Connect port :" << port->quantity()->symbol;
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
                xTrace() << "Close connection to port " << port->quantity()->symbol;
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
    data->clearModelData();

    //reannotate all models
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0)
            data->annotateModel(md);
    }
    this->update();
}

QDataStream & KModelScene::serialize(QDataStream & stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    ModelList lModels = models();
    ConnectorList lCons = connectors();

    //serialize internal data
    data->serialize(stream);
    //save scene rectangle
    stream << sceneRect();

    int nz = lModels.size();
    stream << nz;
    for (int k = 0; k < nz; k++)
        lModels.at(k)->serialize(stream);

    nz = lCons.size();
    stream << nz;
    for(int k = 0; k < nz; k++)
        lCons.at(k)->serialize(stream);

    return stream;
}
QDataStream & KModelScene::deserialize(QDataStream & stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    QRectF rect;
    data->deserialize(stream);
    stream >> rect;
    this->setSceneRect(rect);

    int nz;
    stream >> nz;
    for (int k = 0; k < nz; k++) {
        IModel * md = KPluginManager::instance()->createModel(stream);
        if (md != 0) {
            addItem(md);
            md->deserialize(stream);
        }
    }

    stream >> nz;
    for (int k = 0; k < nz; k++) {
        KConnector * con = new KConnector();
        addItem(con);
        con->deserialize(stream);
        if (!con->isConnected()) {
            removeItem(con);
            delete con;
        }
    }

    return stream;
}

