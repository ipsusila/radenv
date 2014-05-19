#include <QtDebug>
#include <QPluginLoader>
#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>
#include <QThread>
#include <QSet>
#include <QPainter>
#include "kscenario.h"
#include "imodel.h"
#include "kmodelinfo.h"
#include "koutput.h"
#include "kport.h"
#include "kconnector.h"
#include "dialogcalculation.h"
#include "kscenarioevaluator.h"
#include "kapplication.h"
#include "kassessment.h"

//Stream token
static const QString __scenarioToken = "@Scenario-v.1.0";

class KScenarioPrivate
{
    bool _snapToGrid;
    int _gridPixel;
    bool _displayGrid;
    KScenario::EditMode _editMode;
    KConnector * _tempConnector;
    DialogCalculation  * _calcDialog;
    KScenarioEvaluator * _scenarioEvaluator;
    QString _scenarioName;
    QString _scenarioDesc;

    QStringList _modelNames;
    ModelList _verifiedNodes;     ///< List of verified nodes
    CalculationList _calcInfos;

public:
    KScenarioPrivate() : _snapToGrid(true), _gridPixel(20),
        _displayGrid(true), _editMode(KScenario::None),
        _tempConnector(0), _calcDialog(0), _scenarioEvaluator(0)
    {
    }
    ~KScenarioPrivate() {
        stopCalculation();
        delete _scenarioEvaluator;
        delete _calcDialog;
    }

    inline bool snapToGrid() const { return _snapToGrid; }
    inline void setSnapToGrid(bool v) { _snapToGrid = v;}
    inline int gridPixel() const { return _gridPixel; }
    inline void setGridPixel(int px) { _gridPixel = px; }
    inline bool displayGrid() const { return _displayGrid; }
    inline void setDisplayGrid(bool v) { _displayGrid = v; }
    inline ModelList * verifiedNodes() { return &_verifiedNodes; }
    inline KScenario::EditMode editMode() const { return _editMode; }
    inline KConnector * tempConnector() const { return _tempConnector; }

    inline void setEditMode(KScenario::EditMode mode) {
        _editMode = mode;
    }
    inline void setTempConnector(KConnector * con) {
        _tempConnector = con;
    }
    inline void clearTempConnector() {
        _tempConnector = 0;
    }
    inline QString scenarioName() const {
        return _scenarioName;
    }
    inline void setScenarioName(const QString& name) {
        _scenarioName = name;
    }
    inline QString scenarioDescription() const {
        return _scenarioDesc;
    }
    inline void setScenarioDescription(const QString& desc) {
        _scenarioDesc = desc;
    }

    inline void serialize(QDataStream & s) const
    {
        s << __scenarioToken << _scenarioName << _scenarioDesc << _snapToGrid
          << _gridPixel << _displayGrid << (qint32)_editMode << _modelNames;
    }
    inline bool deserialize(QDataStream & s)
    {
        QString token;
        s >> token;
        if(token != __scenarioToken)
            return false;

        qint32 mode;
        s >> _scenarioName >> _scenarioDesc >> _snapToGrid
          >> _gridPixel >> _displayGrid >> mode >> _modelNames;
        _editMode = (KScenario::EditMode)mode;

        return true;
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
            _scenarioEvaluator->stop();
            _scenarioEvaluator->wait(wms);
        }

    }
    inline bool isRunning() const
    {
        return (_scenarioEvaluator != 0 && _scenarioEvaluator->isRunning());
    }

    void runCalculation(const KCalculationInfo & ci)
    {
        //clear calculation info
        this->clearCalculationInfo();

        stopCalculation();
        if (_scenarioEvaluator)
            delete _scenarioEvaluator;
        _scenarioEvaluator = new KScenarioEvaluator(&_verifiedNodes, ci);
        _scenarioEvaluator->start();
    }

    void pause() {
        if (isRunning())
            _scenarioEvaluator->pause();
    }

    void resume() {
        if (_scenarioEvaluator != 0)
            _scenarioEvaluator->resume();
    }
};

KScenario::KScenario(int x, int y, int w, int h, QObject *parent)
    : QGraphicsScene(x, y, w, h, parent), dptr(new KScenarioPrivate())
{
    this->setFont(QFont("monospace", 10));
}

KScenario::~KScenario()
{
    delete dptr;
    qDebug() << "Scene destroyed";
}

int KScenario::copyModelsTo(KScenario * mscene, bool all) const
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
            IModel * dest = copiedModels[con->inputPort()->model()];
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

ModelList KScenario::models() const
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
ConnectorList KScenario::connectors() const
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
IModel * KScenario::findModel(const QString& tagName) const
{
    for(int k = 0; k < items().size(); k++) {
        QGraphicsItem * item = items().at(k);
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0 && md->tagName() == tagName)
            return md;
    }
    return 0;
}

IModel * KScenario::createModel(IModelFactory * factory, const KModelInfo& info)
{
    IModel * model = KApplication::selfInstance()->createModel(factory, info);
    if (model != 0) {
        dptr->annotateModel(model);
        addItem(model);
    }
    return model;
}
void KScenario::clearModels()
{
    ModelList modelList = models();
    for(int k = 0; k < modelList.size(); k++)
        removeModel(modelList.at(k), false);

    dptr->clearModelData();
    this->clear();
}
void KScenario::refresh()
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
QList<ModelLocation> KScenario::extractLocations() const
{
    QList<ModelLocation> list;
    for(int k = 0; k < items().size(); k++) {
        QGraphicsItem * item = items().at(k);
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0) {
            KLocation loc = md->location();
            if (loc.isValid())
            {
                list.append(ModelLocation(md, loc));
            }
        }
    }
    return list;
}
ReportList KScenario::reports() const
{
    ReportList list;
    ModelList modelList = models();
    for(int k = 0; k < modelList.size(); k++){
        KReport * rep = modelList.at(k)->report();
        if (rep && !list.contains(rep))
            list.append(rep);
    }
    return list;
}
void KScenario::setReport(KReport * rep)
{
    ModelList modelList = models();
    for(int k = 0; k < modelList.size(); k++){
        modelList.at(k)->setReport(rep);
    }
}

void KScenario::generateReport()
{
    ModelList * vNodes = dptr->verifiedNodes();
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
        rep->add(*(this->assessment()));
        rep->add(*this);
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

void KScenario::verify()
{
    ModelList * vNodes = dptr->verifiedNodes();
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
void KScenario::pause()
{
    dptr->pause();
}

void KScenario::stop()
{
    dptr->stopCalculation();
}

void KScenario::resume()
{
    dptr->resume();
}

void KScenario::evaluate()
{
    if (this->items().isEmpty())
        return;

    //do verification before calculate
    //if error, return
    ModelList * vNodes = dptr->verifiedNodes();
    if (vNodes->isEmpty()) {
        verify();
        if (vNodes->isEmpty()) {
            xError() << QObject::tr("Calculation can not be performed due to some error(s) in the model network.");
            return;
        }
    }

    //show run dialog
    //and exec running thread
    DialogCalculation * dlg = dptr->calculationDialog();
    if (dlg->exec() == QDialog::Accepted)
        dptr->runCalculation(dlg->calculationInfo());
}

void KScenario::setAssessment(KAssessment * aP)
{
    setParent(aP);
}

KAssessment * KScenario::assessment() const
{
    return qobject_cast<KAssessment *>(parent());
}

const CalculationList & KScenario::calculationResults() const
{
    return dptr->calculationInfo();
}
QString KScenario::name() const
{
    return dptr->scenarioName();
}
void KScenario::setName(const QString& name)
{
    dptr->setScenarioName(name);
}

QString KScenario::description() const
{
    return dptr->scenarioDescription();
}
void KScenario::setDescription(const QString& desc)
{
    dptr->setScenarioDescription(desc);
}

bool KScenario::snapToGrid() const
{
    return dptr->snapToGrid();
}
void KScenario::setSnapToGrid(bool v)
{
    dptr->setSnapToGrid(v);
}

int KScenario::grid() const
{
    return dptr->gridPixel();
}
void KScenario::setGrid(int px)
{
    dptr->setGridPixel(px);
}
bool KScenario::displayGrid() const
{
    return dptr->displayGrid();
}
void KScenario::setDisplayGrid(bool v)
{
    dptr->setDisplayGrid(v);
}

KScenario::EditMode KScenario::editMode() const
{
    return dptr->editMode();
}
void KScenario::setEditMode(KScenario::EditMode mode)
{
    EditMode oldMode = editMode();
    if (oldMode == mode)
        return;

    //now set edit mode
    dptr->setEditMode(mode);
    KConnector * con = dptr->tempConnector();
    if (oldMode == Connect && con != 0) {
        dptr->clearTempConnector();
        removeItem(con);
        delete con;
        update();
    }
}
void KScenario::removeModel(IModel *model, bool redraw)
{
    Q_ASSERT(model);

    //remove connections
    model->removeConnections();

    //remove model
    dptr->verifiedNodes()->removeOne(model);
    this->removeItem(model);
    delete model;
    if (redraw)
        this->update();
}

void KScenario::removeConnector(KConnector *con)
{
    Q_ASSERT(con);

    //this should not happen
    //to make sure everything is OK
    if (con == dptr->tempConnector())
        dptr->clearTempConnector();

    con->disconnect();
    this->removeItem(con);
    delete con;
    this->update();
}

void KScenario::connectItem(QGraphicsItem * selItem, const QPointF& scPos)
{
    if (dptr->editMode() == Connect) {
        KPort * port = qgraphicsitem_cast<KPort*>(selItem);
        //xTrace() << "Make connection from/to port: " << port->label() << " at " << scPos;

        KConnector * con = dptr->tempConnector();
        if (con == 0) {
            //only handle if port is not null
            if (port == 0)
                return;

            con = new KConnector();
            dptr->setTempConnector(con);
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
                    dptr->clearTempConnector();
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
void KScenario::cancelConnection()
{
    KConnector * con = dptr->tempConnector();
    if (con != 0) {
        dptr->clearTempConnector();
        removeItem(con);
        delete con;
    }
}

void KScenario::tryDrawConnector(const QPointF& scPos)
{
    if (dptr->editMode() == Connect) {
        KConnector * con = dptr->tempConnector();
        if (con != 0) {
            con->setTemporaryPoint(scPos);
        }
    }
}
void KScenario::reannotateModels()
{
    //clear annotation list and verified list
    dptr->clearModelData();

    //reannotate all models
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem * item, list) {
        IModel * md = qgraphicsitem_cast<IModel *>(item);
        if (md != 0)
            dptr->annotateModel(md);
    }
    this->update();
}

QDataStream & KScenario::serialize(QDataStream & stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    ModelList lModels = models();
    ConnectorList lCons = connectors();

    //serialize internal data
    dptr->serialize(stream);
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
QDataStream & KScenario::deserialize(QDataStream & stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    QRectF rect;
    if (!dptr->deserialize(stream)) {
        xTrace() << "Aborted: Can not load scene from stream due to incompatible format";
        return stream;
    }
    stream >> rect;
    this->setSceneRect(rect);

    int nz;
    stream >> nz;
    for (int k = 0; k < nz; k++) {
        IModel * md = KApplication::selfInstance()->createModel(stream);
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
void KScenario::drawBackground(QPainter *painter, const QRectF &vrect)
{
    painter->setClipRect(vrect);
    QRectF rect = sceneRect();
    painter->fillRect(rect, this->backgroundBrush());
    if (displayGrid()) {
        int grd = grid();
        //major and minor grd pen
        QPen minPen(QColor(230,230,230), 1, Qt::DotLine);
        QPen majPen(QColor(230,230,230), 1);
        //QPen majPen(QColor(Qt::black), 1);

        //starting position, normalized to grd size
        int x = grd * ((int)rect.x() / grd);
        int y = grd * ((int)rect.y() / grd);
        int r = grd * ((int)rect.right() / grd);
        int b = grd * ((int)rect.bottom() / grd);

        //draw vertical line
        int dg = grd * 5;
        painter->setPen(minPen);
        while (x < r) {
            if (x % dg == 0) {
                painter->setPen(majPen);
                painter->drawLine(x, rect.y(), x, rect.bottom());
                painter->setPen(minPen);
            }
            else {
                painter->drawLine(x, rect.y(), x, rect.bottom());
            }

            //next grd line
            x += grd;
        }

        //draw horizontal line
        painter->setPen(minPen);
        while (y < b) {
            if (y % dg == 0) {
                painter->setPen(majPen);
                painter->drawLine(rect.x(), y, rect.right(), y);
                painter->setPen(minPen);
            }
            else {
                painter->drawLine(rect.x(), y, rect.right(), y);
            }

            //next grd line
            y += grd;
        }

        //draw
    }

    //draw the frame
    painter->setPen(QPen(Qt::darkGray, 1));
    painter->drawRect(rect);

    //display name
    QString txt = name() + " (" + assessment()->name() + ")";
    painter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, txt);
}

