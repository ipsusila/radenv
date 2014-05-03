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
#include "kpluginmanager.h"
#include "kcase.h"

//test
#include "testclass.h"

MainWindow::MainWindow(XOutputView * vw, QWidget *parent) :
    QMainWindow(parent), outView(vw)
{
    //create scene
    scene = new KModelScene(-800, -400, 1600, 800);

    //todo (load from database)
    KStorage::addStorage("/home/ips/workspace/smea.db");

    //load all plugins
    //loadAllPlugins();
    KPluginManager::instance()->loadPlugins();

    createView();
    createActions();
    createToolBars();
    createMenus();
    createStatusBar();

    setWindowTitle(tr("SMEA Dose Assessments"));
    setUnifiedTitleAndToolBarOnMac(true);

    qDebug() << "Number: " << QString::number(1.0, 'f', 0) << ";" << QString::number(1.2, 'f', 2);
    KPluginManager pm1;
    qDebug() << "Factory count: " << pm1.factoryCount();

}

MainWindow::~MainWindow()
{
    delete view;

    //delete storage;
    KStorage::removeStorages();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    KPluginManager::instance()->unloadPlugins();
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

    fSaveAsAct = new QAction(tr("Save Assessment &As..."), this);
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
    vDisplayGridAct->setChecked(view->isDisplayGrid());
    vDisplayGridAct->setStatusTip(tr("Show/hide grid line"));
    vDisplayGridAct->setIcon(QIcon(":/std/grid-show.png"));
    connect(vDisplayGridAct, SIGNAL(triggered(bool)), view, SLOT(displayGrid(bool)));

    vSnapGridAct = new QAction(tr("&Snap to Grid"), this);
    vSnapGridAct->setCheckable(true);
    vSnapGridAct->setChecked(view->isSnapToGrid());
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
    FactoryList factories = KPluginManager::instance()->factories();
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

void MainWindow::modelTriggeredAction(IModelFactory* f, const KModelInfo & info)
{
    Q_ASSERT(f != 0);
    //Ask scene to create model
    view->createModel(f, info);
    xTrace() << "Create model: " << info.name();
}

void MainWindow::newAssessment()
{
    quintptr addr = (quintptr)TestClass::instance();
    QString sAddr = QString::number(addr, 16);
    xTrace() << "@Address : " << sAddr;
    xTrace() << "@@Current value: " << TestClass::instance()->currentValue();
    TestClass::instance()->increment(20);
    xTrace() << "#@Next value: " << TestClass::instance()->currentValue();
}

void MainWindow::openAssessment()
{
    //open file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Data file (*.dat)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QDataStream stream(&file);
        scene->clearModels();
        scene->deserialize(stream);
        file.close();
    }
}

void MainWindow::saveAssessment()
{
    //save as current name
}

void MainWindow::saveAssessmentAs()
{
    //save as file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                                    tr("Data file (*.dat)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QDataStream stream(&file);
        scene->serialize(stream);
        file.close();
    }

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
    view->refreshModels();

}
void MainWindow::modelRadionuclides()
{
    DialogRadionuclide dlg;
    dlg.exec();
    view->refreshModels();
}
void MainWindow::modelVerify()
{
    outView->clearContents();
    view->verifyModels();
}

void MainWindow::modelCalculate()
{
    outView->clearContents();
    view->calculate();
}


void MainWindow::helpContent()
{
    //TEST
    AssessmentList list;
    list.append(KCase());
    list.append(KCase());

    AssessmentList list2 = list;
    list = AssessmentList();
}
void MainWindow::helpAbout()
{

}
