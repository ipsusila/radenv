#include <QtGui>
#include "mainwindow.h"
#include "xmodelview.h"

#include "xoutputview.h"
#include "kstorage.h"
#include "kstoragecontent.h"
#include "kmodelscene.h"
#include "kdata.h"
#include "klocation.h"
#include "imodelfactory.h"
#include "imodel.h"
#include "kradionuclide.h"
#include "kgroupinfo.h"
#include "xmodelaction.h"
#include "xmodelwidget.h"

#include "dialoglocation.h"
#include "dialogradionuclide.h"
#include "kmath.h"

MainWindow::MainWindow(XOutputView * vw, QWidget *parent) :
    QMainWindow(parent), outView(vw)
{
    //create scene
    scene = new KModelScene(-800, -400, 1600, 800);

    KStorage::addStorage("/home/ips/workspace/smea.db");

    //load all plugins
    loadAllPlugins();

    createView();
    createActions();
    createToolBars();
    createMenus();
    createStatusBar();

    setWindowTitle(tr("SMEA Dose Assessments"));
    setUnifiedTitleAndToolBarOnMac(true);

    /*
    for (int k = 1; k < 20; k++)
        qDebug() << "Pr(" << k << ")=" << KMath::pr(k);

    for(int k = 0; k < 5; k++)
        qDebug() << "M-N(" << (-0.5 + k*0.5) << ")=" << KMath::ratioN(-0.5+k*0.5);
    qDebug() << "M-N(1.73) = " << KMath::ratioN(1.73);
    qDebug() << "M-N(100) = " << KMath::ratioN(100);

    KHalfLife hl(10);
    KHalfLife l2 = hl;
    qDebug() << "Half life: " << l2.displayText();
    */

    //test
    /*
    XStorageContent c;
    c.setName("Demo storage");
    c.setDescription("Test demo storage");

    XStorageContent cp = c;
    c.setDescription("Modified storage");

    QDataStream s(&c, QIODevice::Append|QIODevice::WriteOnly|QIODevice::ReadOnly);
    s << "test data write";
    XStorageContent cp2 = c;

    qDebug() << "Storage-c: " << c.name() << ",desc: " << c.description() << "size: " << c.size();
    qDebug() << "Storage-cp: " << cp.name() << ",desc: " << cp.description() << "size: " << cp.size();
    qDebug() << "Storage-cp2: " << cp2.name() << ",desc: " << cp2.description() << "size: " << cp2.size();

    s << "Add more data";
    qDebug() << "C:" << c.constData();
    qDebug() << "C2:" << cp2.constData();
    qDebug() << "Storage-c: " << c.name() << ",desc: " << c.description() << "size: " << c.size();

    const char * p = c.constData();
    qDebug() << "P:" << p;

    qDebug() << "save storage: " << storage->save(c);

    XStorageContent cr = storage->load(c.name(), 0);
    qDebug() << "Retrieve: " << cr.name() << ",Desc: " << cr.description() << ",size:" << cr.size();
    */

    /*
    XData p(2);
    p.setSymbol("Qi");
    p.setText("Discharge Rate");
    p.setDescription("Aquatic discharge rate");
    p.setUnit("Bq/m3");
    p[0] = XDataItem("Am-232", 0.13241);
    p[1] = XDataItem("Co-60", 123);

    XData p2 = p * 5;
    qDebug() << p2.text() << "(" << p2.symbol() << ") in " << p2.unit();
    for(int k = 0; k < p2.count(); k++) {
        xTrace() << "P:" << p[k].name() << ":" << p[k].numericValue();
        xInfo() << "P:" << p2[k].name() << ":" << p2[k].numericValue();
    }

    p *= 10;
    qDebug() << p2.text() << "(" << p2.symbol() << ") in " << p2.unit();
    for(int k = 0; k < p2.count(); k++) {
        xError() << "P:" << p[k].name() << ":" << p[k].numericValue();
        xWarning() << "P:" << p2[k].name() << ":" << p2[k].numericValue();
    }
    */

}

MainWindow::~MainWindow()
{
    delete view;

    //delete storage;
    KStorage::removeStorages();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for (int k = 0; k < factories.size(); k++) {
        factories[k]->onFinalized();
    }
    event->accept();
}

/**
 * @brief MainWindow::createView
 */
void MainWindow::createView()
{
    view = new XModelView(scene, this);

    //allocate stack widget to display multiple views
    //available views:
    // - connection
    // - report
    // - map
    // - help / technical documents

    pages = new QStackedWidget();
    pages->addWidget(view);
    pages->setCurrentWidget(view);

    this->setCentralWidget(pages);

    viewMenu = new QMenu(tr("&View"), menuBar());

    //Create dock widget to display the output
    QDockWidget *dock = new QDockWidget(tr("Output window"), this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(outView);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createActions()
{
    /*
    QAction * fNewAct;
    QAction * fOpenAct;
    QAction * fSaveAct;
    QAction * fSaveAsAct;
    QAction * fPrintPrevAct;
    QAction * fPrintAct;
    QAction * fPrintSetupAct;
    QAction * fQuitAct;
    */
    fNewAct = new QAction(tr("&New Assessment..."), this);
    fNewAct->setShortcut(QKeySequence::New);
    fNewAct->setStatusTip(tr("Create new assessment"));
    fNewAct->setIcon(QIcon(":/std/new-assessment.png"));
    connect(fNewAct, SIGNAL(triggered()), this, SLOT(newAssessment()));

    fOpenAct = new QAction(tr("&Open Assessment..."), this);
    fOpenAct->setShortcut(QKeySequence::Open);
    fOpenAct->setStatusTip(tr("Open existing assessment"));
    fOpenAct->setIcon(QIcon(":/std/open-assessment.png"));
    connect(fOpenAct, SIGNAL(triggered()), this, SLOT(openAssessment()));

    fSaveAct = new QAction(tr("&Save Assessment"), this);
    fSaveAct->setShortcut(QKeySequence::Save);
    fSaveAct->setStatusTip(tr("Save the assessment"));
    fSaveAct->setIcon(QIcon(":/std/save-assessment.png"));
    connect(fSaveAct, SIGNAL(triggered()), this, SLOT(saveAssessment()));

    fSaveAsAct = new QAction(tr("Save Scenario &As..."), this);
    fSaveAsAct->setShortcut(QKeySequence::SaveAs);
    fSaveAsAct->setStatusTip(tr("Save the assessment to given name"));
    fSaveAsAct->setIcon(QIcon(":/std/save-assessment-as.png"));
    connect(fSaveAsAct, SIGNAL(triggered()), this, SLOT(saveAssessmentAs()));

    fPrintSetupAct = new QAction(tr("Print Setup..."), this);
    //fPrintSetupAct->setShortcut(QKeySequence::Save);
    fPrintSetupAct->setStatusTip(tr("Setup printer"));
    //fPrintSetupAct->setIcon(QIcon(":/std/save-assessment.png"));
    connect(fPrintSetupAct, SIGNAL(triggered()), this, SLOT(printSetup()));

    fPrintPrevAct = new QAction(tr("Print Pre&view..."), this);
    //fPrintPrevAct->setShortcut(QKeySequence::Print);
    fPrintPrevAct->setStatusTip(tr("Print preview"));
    //fPrintPrevAct->setIcon(QIcon(":/std/save-assessment.png"));
    connect(fPrintPrevAct, SIGNAL(triggered()), this, SLOT(printPreview()));

    fPrintAct = new QAction(tr("&Print"), this);
    fPrintAct->setShortcut(QKeySequence::Print);
    fPrintAct->setStatusTip(tr("Print all"));
    fPrintAct->setIcon(QIcon(":/std/print.png"));
    connect(fPrintAct, SIGNAL(triggered()), this, SLOT(printAll()));

    fQuitAct = new QAction(tr("&Quit"), this);
    fQuitAct->setShortcuts(QKeySequence::Quit);
    fQuitAct->setStatusTip(tr("Quit the application"));
    fQuitAct->setIcon(QIcon(":/std/app-quit.png"));
    connect(fQuitAct, SIGNAL(triggered()), this, SLOT(close()));

    /*
    QAction * eClearAct;
    QAction * eNoneAct;
    QAction * eRemoveAct;
    QAction * eRemoveConAct;
    QAction * eConnectAct;
    QAction * eRenumberAct;
    QAction * eOptionsAct;
    */
    eGroupEdit = new QActionGroup(this);

    eNoneAct = new QAction(tr("C&ursor"), eGroupEdit);
    //eNoneAct->setShortcuts(QKeySequence::Quit);
    eNoneAct->setStatusTip(tr("Move and navigation mode."));
    eNoneAct->setIcon(QIcon(":/std/edit-cursor.png"));
    eNoneAct->setCheckable(true);
    //connect(eNoneAct, SIGNAL(triggered()), view, SLOT(connectModel()));

    eConnectAct = new QAction(tr("C&onnect"), eGroupEdit);
    //eConnectAct->setShortcuts(QKeySequence::Quit);
    eConnectAct->setStatusTip(tr("Create connection between model"));
    eConnectAct->setIcon(QIcon(":/std/edit-connect.png"));
    eConnectAct->setCheckable(true);
    //connect(eConnectAct, SIGNAL(triggered()), view, SLOT(connectModel()));

    eRemoveConAct = new QAction(tr("R&emove Connection"), eGroupEdit);
    //eRemoveConAct->setShortcuts(QKeySequence::Quit);
    eRemoveConAct->setStatusTip(tr("Remove model connection"));
    eRemoveConAct->setIcon(QIcon(":/std/edit-remove-con.png"));
    eRemoveConAct->setCheckable(true);
    //connect(eRemoveConAct, SIGNAL(triggered()), view, SLOT(removeConnection()));

    eRemoveAct = new QAction(tr("&Remove Model"), eGroupEdit);
    //eRemoveAct->setShortcuts(QKeySequence::Quit);
    eRemoveAct->setStatusTip(tr("Remove selected model"));
    eRemoveAct->setIcon(QIcon(":/std/edit-remove.png"));
    eRemoveAct->setCheckable(true);
    //connect(eRemoveAct, SIGNAL(triggered()), view, SLOT(removeModel()));

    eGroupEdit->addAction(eNoneAct);
    eGroupEdit->addAction(eConnectAct);
    eGroupEdit->addAction(eRemoveConAct);
    eGroupEdit->addAction(eRemoveAct);
    connect(eGroupEdit, SIGNAL(triggered(QAction*)), this, SLOT(editMode(QAction*)));
    eNoneAct->setChecked(true);

    eRenumberAct = new QAction(tr("Re&number tag"), this);
    //eRenumberAct->setShortcuts(QKeySequence::Quit);
    eRenumberAct->setStatusTip(tr("Renumber model's tag"));
    eRenumberAct->setIcon(QIcon(":/std/edit-renumber.png"));
    connect(eRenumberAct, SIGNAL(triggered()), view, SLOT(reannotateModel()));

    eClearAct = new QAction(tr("&Clear all..."), this);
    //eClearAct->setShortcuts(QKeySequence::Quit);
    eClearAct->setStatusTip(tr("Delete all models from scene"));
    eClearAct->setIcon(QIcon(":/std/edit-clear.png"));
    connect(eClearAct, SIGNAL(triggered()), view, SLOT(clearModel()));

    eOptionsAct = new QAction(tr("&Options..."), this);
    //eOptionsAct->setShortcuts(QKeySequence::Quit);
    eOptionsAct->setStatusTip(tr("Edit options"));
    eOptionsAct->setIcon(QIcon(":/std/edit-option.png"));
    connect(eOptionsAct, SIGNAL(triggered()), this, SLOT(editOptions()));

    /*
    QAction * mDatabaseAct;
    QAction * mMapAct;
    QAction * mLocationAct;
    QAction * mRadionuclideAct;
    QAction * mVerifyAct;
    QAction * mCalcAct;
    QAction * mCalcStepAct;
    */
    mDatabaseAct = new QAction(tr("&New/Open Database..."), this);
    //mDatabaseAct->setShortcuts(QKeySequence::HelpContents);
    mDatabaseAct->setStatusTip(tr("Create or open database"));
    mDatabaseAct->setIcon(QIcon(":/std/model-database.png"));
    connect(mDatabaseAct, SIGNAL(triggered()), this, SLOT(modelDatabase()));

    mRadionuclideAct = new QAction(tr("Manage &Radionuclides..."), this);
    //mRadionuclideAct->setShortcuts(QKeySequence::HelpContents);
    mRadionuclideAct->setStatusTip(tr("Manage Radionuclide database"));
    mRadionuclideAct->setIcon(QIcon(":/std/model-radioactivity.png"));
    connect(mRadionuclideAct, SIGNAL(triggered()), this, SLOT(modelRadionuclides()));

    mLocationAct = new QAction(tr("Manage &Locations..."), this);
    //mLocationAct->setShortcuts(QKeySequence::HelpContents);
    mLocationAct->setStatusTip(tr("Manage Location database"));
    mLocationAct->setIcon(QIcon(":/std/model-location.png"));
    connect(mLocationAct, SIGNAL(triggered()), this, SLOT(modelLocations()));

    mVerifyAct = new QAction(tr("&Verify"), this);
    //mVerifyAct->setShortcuts(QKeySequence::HelpContents);
    mVerifyAct->setStatusTip(tr("Check the validity of each model"));
    mVerifyAct->setIcon(QIcon(":/std/model-verify.png"));
    connect(mVerifyAct, SIGNAL(triggered()), this, SLOT(modelVerify()));

    mCalcAct = new QAction(tr("&Calculate"), this);
    //mCalcAct->setShortcuts(QKeySequence::HelpContents);
    mCalcAct->setStatusTip(tr("Calculate concentration and dose"));
    mCalcAct->setIcon(QIcon(":/std/model-calculate.png"));
    connect(mCalcAct, SIGNAL(triggered()), this, SLOT(modelCalculate()));

    mReportAct = new QAction(tr("R&eport"), this);
    //mReportAct->setShortcuts(QKeySequence::HelpContents);
    mReportAct->setStatusTip(tr("Generate model and calculation report"));
    mReportAct->setIcon(QIcon(":/std/model-report.png"));
    connect(mReportAct, SIGNAL(triggered()), view, SLOT(report()));


    /*
    QAction * vZoomFitAct;
    QAction * vZoomInAct;
    QAction * vZoomOutAct;
    QAction * vZoomOrigAct;
    QAction * vDisplayGridAct;
    QAction * vSnapGridAct;
    */
    vDisplayGridAct = new QAction(tr("&Display Grid"), this);
    vDisplayGridAct->setCheckable(true);
    vDisplayGridAct->setChecked(scene->displayGrid());
    vDisplayGridAct->setStatusTip(tr("Show/hide grid line"));
    vDisplayGridAct->setIcon(QIcon(":/std/grid-show.png"));
    connect(vDisplayGridAct, SIGNAL(triggered(bool)), view, SLOT(displayGrid(bool)));

    vSnapGridAct = new QAction(tr("&Snap to Grid"), this);
    vSnapGridAct->setCheckable(true);
    vSnapGridAct->setChecked(scene->snapToGrid());
    vSnapGridAct->setStatusTip(tr("Snap object to grid"));
    vSnapGridAct->setIcon(QIcon(":/std/grid-snap.png"));
    connect(vSnapGridAct, SIGNAL(triggered(bool)), view, SLOT(snapToGrid(bool)));

    vZoomInAct = new QAction(tr("Zoom In"), this);
    vZoomInAct->setStatusTip(tr("Increase zoom factor"));
    vZoomInAct->setIcon(QIcon(":/std/zoom-in.png"));
    connect(vZoomInAct, SIGNAL(triggered()), view, SLOT(zoomIn()));

    vZoomFitAct = new QAction(tr("Zoom Fit"), this);
    vZoomFitAct->setStatusTip(tr("Fit scene into view"));
    vZoomFitAct->setIcon(QIcon(":/std/zoom-fit.png"));
    connect(vZoomFitAct, SIGNAL(triggered()), view, SLOT(zoomFit()));

    vZoomOutAct = new QAction(tr("Zoom Out"), this);
    vZoomOutAct->setStatusTip(tr("Decrease zoom factor"));
    vZoomOutAct->setIcon(QIcon(":/std/zoom-out.png"));
    connect(vZoomOutAct, SIGNAL(triggered()), view, SLOT(zoomOut()));

    vZoomOrigAct = new QAction(tr("Zoom Original"), this);
    vZoomOrigAct->setStatusTip(tr("Zoom to original size"));
    vZoomOrigAct->setIcon(QIcon(":/std/zoom-actual.png"));
    connect(vZoomOrigAct, SIGNAL(triggered()), view, SLOT(zoomOriginal()));

    /*
    QAction * hHelpAct;
    QAction * hAboutAct;
    */
    hHelpAct = new QAction(tr("&Help Contents..."), this);
    hHelpAct->setShortcuts(QKeySequence::HelpContents);
    hHelpAct->setStatusTip(tr("Display help contents"));
    hHelpAct->setIcon(QIcon(":/std/help-content.png"));
    connect(hHelpAct, SIGNAL(triggered()), this, SLOT(helpContent()));

    hAboutAct = new QAction(tr("&About Application..."), this);
    //hAboutAct->setShortcuts(QKeySequence::HelpContents);
    hAboutAct->setStatusTip(tr("Display about dialog"));
    hAboutAct->setIcon(QIcon(":/std/app-icon.png"));
    connect(hAboutAct, SIGNAL(triggered()), this, SLOT(helpAbout()));
}

/**
 * @brief MainWindow::createToolBars
 */
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(fQuitAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(fNewAct);
    fileToolBar->addAction(fOpenAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(fSaveAct);
    fileToolBar->addAction(fSaveAsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(fPrintAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addActions(eGroupEdit->actions());
    editToolBar->addSeparator();
    editToolBar->addAction(eClearAct);
    //editToolBar->addSeparator();
    editToolBar->addAction(eRenumberAct);
    editToolBar->addSeparator();
    editToolBar->addAction(this->eOptionsAct);

    modelToolBar = addToolBar(tr("Model"));
    modelToolBar->addAction(mDatabaseAct);
    modelToolBar->addAction(mLocationAct);
    modelToolBar->addSeparator();
    modelToolBar->addAction(mVerifyAct);
    modelToolBar->addAction(mCalcAct);
    modelToolBar->addAction(mReportAct);

    viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(this->vDisplayGridAct);
    viewToolBar->addAction(this->vSnapGridAct);
    viewToolBar->addAction(this->vZoomOrigAct);
    viewToolBar->addAction(this->vZoomOutAct);
    viewToolBar->addAction(this->vZoomFitAct);
    viewToolBar->addAction(this->vZoomInAct);

    viewToolBar->addSeparator();
    viewToolBar->addAction(this->hHelpAct);
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fNewAct);
    fileMenu->addAction(fOpenAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fSaveAct);
    fileMenu->addAction(fSaveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fPrintSetupAct);
    fileMenu->addAction(fPrintPrevAct);
    fileMenu->addAction(fPrintAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fQuitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addActions(eGroupEdit->actions());
    editMenu->addSeparator();
    editMenu->addAction(eClearAct);
    //editMenu->addSeparator();
    editMenu->addAction(eRenumberAct);
    editMenu->addSeparator();
    editMenu->addAction(this->eOptionsAct);

    //populate plugin menus and submenus
    modelMenu = menuBar()->addMenu(tr("&Model"));
    modelMenu->addAction(mDatabaseAct);
    modelMenu->addSeparator();
    modelMenu->addAction(mRadionuclideAct);
    modelMenu->addAction(mLocationAct);
    modelMenu->addSeparator();
    modelMenu->addAction(mVerifyAct);
    modelMenu->addAction(mCalcAct);
    modelMenu->addAction(mReportAct);

    //create plugin menus
    //the item is placed after [Model]
    createPluginMenus();

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addAction(vZoomInAct);
    viewMenu->addAction(vZoomFitAct);
    viewMenu->addAction(vZoomOutAct);
    viewMenu->addAction(vZoomOrigAct);

    viewMenu->addSeparator();
    viewMenu->addAction(this->vDisplayGridAct);
    viewMenu->addAction(this->vSnapGridAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(hHelpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(hAboutAct);
}
void MainWindow::createPluginMenus()
{
    //add top menus
    bool hasSeparator = false;    
    FactoryList factories = scene->factories();
    foreach(IModelFactory * factory, factories) {
        //if is top level factory, add as new menu item (placed after model)
        if (factory->isTopLevel()) {
            QMenu * group = menuBar()->addMenu(factory->name());

            //Create dock widget to display the models
            QDockWidget *dock = new QDockWidget(factory->name() + tr(" Tools"), this);
            dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
            XModelWidget * mw = new XModelWidget(dock);
            dock->setWidget(mw);
            addDockWidget(Qt::RightDockWidgetArea, dock);
            viewMenu->addAction(dock->toggleViewAction());

            //Populate menus and its submneu
            populatePluginMenus(mw, group, factory, factory->groups());
        }
        else {
            //if separator is not yet added, add new separator
            if (!hasSeparator) {
                modelMenu->addSeparator();
                hasSeparator = true;
            }
            populatePluginMenus(modelMenu, factory, factory->groups());
        }
    }
}

/**
 * @brief MainWindow::populatePluginMenus
 * @param groupMenu
 * @param factory
 * @param groupList
 */
void MainWindow::populatePluginMenus(XModelWidget * mw, QMenu * groupMenu,
    IModelFactory * factory, GroupInfoList groupList)
{
    //navigate through group list
    while (!groupList.isEmpty()) {
        //navigate through the children group than
        //populate plugins menus
        KGroupInfo group = groupList.takeFirst();
        QMenu * gmenu = groupMenu->addMenu(group.name());
        GroupInfoList childs = group.children();
        populatePluginMenus(mw, gmenu, factory, childs);

        //create group at widgets
        //add infos
        ModelInfoList infos = group.infos();
        XModelGroup * wgroup = mw->createGroup(group.name());
        foreach(KModelInfo inf, infos){
            XModelAction * action = new XModelAction(factory, inf, this);
            connect(action, SIGNAL(triggered(IModelFactory*,KModelInfo)), this,
                    SLOT(modelTriggeredAction(IModelFactory*,KModelInfo)));
            gmenu->addAction(action);

            //add to toolbar
            mw->addModelAction(wgroup, action);
        }
    }
}

void MainWindow::populatePluginMenus(QMenu * groupMenu,
    IModelFactory * factory, GroupInfoList groupList)
{
    while (!groupList.isEmpty()) {
        KGroupInfo group = groupList.takeFirst();
        QMenu * gmenu = groupMenu->addMenu(group.name());
        GroupInfoList childs = group.children();
        populatePluginMenus(gmenu, factory, childs);

        //create group at widgets
        //add infos
        ModelInfoList infos = group.infos();
        foreach(KModelInfo inf, infos){
            XModelAction * action = new XModelAction(factory, inf, this);
            connect(action, SIGNAL(triggered(IModelFactory*,KModelInfo)), this,
                    SLOT(modelTriggeredAction(IModelFactory*,KModelInfo)));
            gmenu->addAction(action);

            //add to toolbar
            modelToolBar->addAction(action);
        }
    }
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

/**
 * @brief MainWindow::loadAllPlugins
 */
void MainWindow::loadAllPlugins()
{
    //load plugin from default
    if (!loadPlugin(scene))
        qWarning() << "Can not load any plugins from [Default] location";

    //load plugins at user directory
    //TODO
}

/**
 * @brief MainWindow::loadPlugin
 * @param scene
 * @param path
 * @return
 */
bool MainWindow::loadPlugin(KModelScene * scene, const QString& path)
{
    QDir pluginsDir;
    if (path.isEmpty()) {
        //default plugin dirs
        pluginsDir = QDir(qApp->applicationDirPath());
    #if defined(Q_OS_WIN)
        if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
            pluginsDir.cdUp();
    #elif defined(Q_OS_MAC)
        if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
    #endif
        pluginsDir.cd("plugins");
    }
    else {
        pluginsDir = QDir(path);
    }

    //load available plugins
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            IModelFactory * factory = qobject_cast<IModelFactory *>(plugin);
            if (factory) {
                factory->initialize();
                scene->addFactory(factory);

                xTrace() << "Plugin directory: " << pluginsDir.absoluteFilePath(fileName);
                xInfo() << "Name:" << factory->name() << ",Author:" << factory->author()
                         << "Version: " << factory->version();
                factories.append(factory);
            }
        }
        else {
            qWarning() << "Error: " << pluginLoader.errorString();
            pluginLoader.unload();
        }
    }

    return (scene->factoryCount() > 0);
}

void MainWindow::modelTriggeredAction(IModelFactory* f, const KModelInfo & info)
{
    Q_ASSERT(f != 0);
    //Ask scene to create model
    if (scene)
        scene->createModel(f, info);
    xTrace() << "Create model: " << info.name();
}

void MainWindow::newAssessment()
{

}

void MainWindow::openAssessment()
{

}

void MainWindow::saveAssessment()
{

}

void MainWindow::saveAssessmentAs()
{

}

void MainWindow::printPreview()
{

}

void MainWindow::printSetup()
{

}

void MainWindow::printAll()
{

}

void MainWindow::editOptions()
{

}

void MainWindow::editMode(QAction * act)
{
    if (act == eNoneAct)
        view->setEditMode(KModelScene::None);
    else if (act == eConnectAct)
        view->setEditMode(KModelScene::Connect);
    else if (act == eRemoveConAct)
        view->setEditMode(KModelScene::RemoveConnection);
    else if (act == eRemoveAct)
        view->setEditMode(KModelScene::RemoveModel);
}
void MainWindow::modelDatabase()
{

}

void MainWindow::modelLocations()
{
    DialogLocation dlg;
    dlg.exec();
    scene->refresh();

}
void MainWindow::modelRadionuclides()
{
    DialogRadionuclide dlg;
    dlg.exec();
    scene->refresh();
}
void MainWindow::modelVerify()
{
    outView->clearContents();
    scene->verify();
}

void MainWindow::modelCalculate()
{
    outView->clearContents();
    scene->calculate();
}


void MainWindow::helpContent()
{

}
void MainWindow::helpAbout()
{

}
