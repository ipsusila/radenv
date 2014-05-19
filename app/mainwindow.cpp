#include <QDockWidget>
#include <QStackedWidget>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>
#include <QTextEdit>
#include <QPrinter>
#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPrintPreviewDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include "mainwindow.h"
#include "xmodelview.h"
#include "mapview.h"

#include "uioutputview.h"
#include "kstorage.h"
#include "kstoragecontent.h"
#include "kscenario.h"
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
#include "kapplication.h"
#include "kassessment.h"
#include "uiassessmentexplorer.h"
#include "dialogoption.h"
#include "dialogabout.h"

MainWindow::MainWindow(UiOutputView * vw, QWidget *parent) :
    QMainWindow(parent), outView(vw), asExplorer(0)
{
    createViews();
    createActions();
    createToolBars();
    createMenus();
    createStatusBar();
    setupActionStates();

    setWindowTitle(tr("SMEA Dose Assessments"));
    setUnifiedTitleAndToolBarOnMac(true);

    QSettings settings;
    restoreGeometry(settings.value("app/geometry").toByteArray());
    restoreState(settings.value("app/settings").toByteArray());

    //check info database
    if (KApplication::selfInstance()->hasStorage())
    {
        xTrace() << "Using database " << KApplication::selfInstance()->storage()->storageName();
    }
    else
    {
        xTrace() << "No database opened. Use New/Open Database menu to attach Database";
    }
}

MainWindow::~MainWindow()
{
    delete printer;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    QString lastStorage;
    KApplication * pm = KApplication::selfInstance();
    if (pm->hasStorage())
        lastStorage = pm->storage()->storageName();
    settings.setValue("app/storage", lastStorage);
    settings.setValue("app/geometry", this->saveGeometry());
    settings.setValue("app/state", saveState());

    event->accept();
}

/**
 * @brief MainWindow::createView
 */
void MainWindow::createViews()
{
    view = new XModelView(this);
    reportView = new QTextEdit(this);
    reportView->setReadOnly(true);

    //map view
    mapView = new MapView(this);


    //create printer
    printer = new QPrinter(QPrinter::PrinterResolution);

    //allocate stack widget to display multiple views
    //available views:
    // - connection
    // - report
    // - map
    // - help / technical documents

    pages = new QStackedWidget();
    pages->addWidget(view);
    pages->addWidget(reportView);
    pages->addWidget(mapView);

    pages->setCurrentWidget(view);
    setCentralWidget(pages);

    viewMenu = new QMenu(tr("&View"), menuBar());

    //Create dock widget to display the output
    QDockWidget *dock = new QDockWidget(tr("Output window"), this);
    dock->setObjectName("OutputWindow");
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(outView);
    dock->setContentsMargins(2,2,2,2);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    QAction * action = dock->toggleViewAction();
    action->setText(tr("Show Output Window"));
    viewMenu->addAction(action);

    //Create dock widget to display the models
    dock = new QDockWidget(tr("Assessment Explorer"), this);
    dock->setObjectName("AssessmentExplorer");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    asExplorer = new UiAssessmentExplorer(dock);
    dock->setContentsMargins(2,2,2,0);
    dock->setWidget(asExplorer);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    action = dock->toggleViewAction();
    action->setText(tr("Show Assessment Explorer"));
    viewMenu->addAction(action);

    connect(asExplorer, SIGNAL(currentScenarioChanged(KScenario*)),
            view, SLOT(attachScenario(KScenario*)));
    connect(asExplorer, SIGNAL(removingScenario(KScenario*)),
            view, SLOT(detachScenario(KScenario*)));
    connect(asExplorer, SIGNAL(viewChanged(int, void*)),
            this, SLOT(switchView(int, void*)));
    connect(asExplorer, SIGNAL(currentScenarioChanged(KScenario*)),
            this, SLOT(changeScenario(KScenario*)));
    connect(asExplorer, SIGNAL(removingScenario(KScenario*)),
            this, SLOT(changeScenario(KScenario*)));

    connect(mapView, SIGNAL(geopositionChanged(QString)),
            this, SLOT(displayGeoposition(QString)));
    connect(asExplorer, SIGNAL(currentScenarioChanged(KScenario*)),
            mapView, SLOT(displayScenario(KScenario*)));
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
    connect(fNewAct, SIGNAL(triggered()), asExplorer, SLOT(addAssessment()));

    fOpenAct = new QAction(tr("&Open Assessment..."), this);
    fOpenAct->setShortcut(QKeySequence::Open);
    fOpenAct->setStatusTip(tr("Open existing assessment"));
    fOpenAct->setIcon(QIcon(":/std/open-assessment.png"));
    connect(fOpenAct, SIGNAL(triggered()), asExplorer, SLOT(openAssessment()));

    fSaveAct = new QAction(tr("&Save Assessment"), this);
    fSaveAct->setShortcut(QKeySequence::Save);
    fSaveAct->setStatusTip(tr("Save the assessment"));
    fSaveAct->setIcon(QIcon(":/std/save-assessment.png"));
    connect(fSaveAct, SIGNAL(triggered()), asExplorer, SLOT(saveAssessment()));

    fSaveAsAct = new QAction(tr("Save Assessment &As..."), this);
    fSaveAsAct->setShortcut(QKeySequence::SaveAs);
    fSaveAsAct->setStatusTip(tr("Save the assessment to given name"));
    fSaveAsAct->setIcon(QIcon(":/std/save-assessment-as.png"));
    connect(fSaveAsAct, SIGNAL(triggered()), asExplorer, SLOT(saveAssessmentAs()));

    fSaveAsPdfAct = new QAction(tr("Save As PDF..."), this);
    //fSaveAsPdfAct->setShortcut(QKeySequence::SaveAs);
    fSaveAsPdfAct->setStatusTip(tr("Save current view as PDF file"));
    fSaveAsPdfAct->setIcon(QIcon(":/std/savepdf.png"));
    connect(fSaveAsPdfAct, SIGNAL(triggered()), this, SLOT(saveAsPdf()));

    fPageSetupAct = new QAction(tr("Page Setup..."), this);
    //fPrintSetupAct->setShortcut(QKeySequence::Save);
    fPageSetupAct->setStatusTip(tr("Setup page for printer"));
    //fPrintSetupAct->setIcon(QIcon(":/std/save-assessment.png"));
    connect(fPageSetupAct, SIGNAL(triggered()), this, SLOT(pageSetup()));

    fPrintPrevAct = new QAction(tr("Print Pre&view..."), this);
    //fPrintPrevAct->setShortcut(QKeySequence::Print);
    fPrintPrevAct->setStatusTip(tr("Print preview"));
    //fPrintPrevAct->setIcon(QIcon(":/std/save-assessment.png"));
    connect(fPrintPrevAct, SIGNAL(triggered()), this, SLOT(printPreview()));

    fPrintAct = new QAction(tr("&Print..."), this);
    fPrintAct->setShortcut(QKeySequence::Print);
    fPrintAct->setStatusTip(tr("Print currently active document"));
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
    connect(eRenumberAct, SIGNAL(triggered()), asExplorer, SLOT(reannotateModel()));

    eClearAct = new QAction(tr("&Clear all..."), this);
    //eClearAct->setShortcuts(QKeySequence::Quit);
    eClearAct->setStatusTip(tr("Delete all models from scenario"));
    eClearAct->setIcon(QIcon(":/std/edit-clear.png"));
    connect(eClearAct, SIGNAL(triggered()), asExplorer, SLOT(clearModel()));

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

    mCloseDbAct = new QAction(tr("&Close Database"), this);
    //mCloseDbAct->setShortcuts(QKeySequence::HelpContents);
    mCloseDbAct->setStatusTip(tr("Close current database"));
    mCloseDbAct->setIcon(QIcon(":/std/model-closedb.png"));
    connect(mCloseDbAct, SIGNAL(triggered()), this, SLOT(closeDatabase()));

    mRadionuclideAct = new QAction(tr("Manage &Radionuclides..."), this);
    //mRadionuclideAct->setShortcuts(QKeySequence::HelpContents);
    mRadionuclideAct->setStatusTip(tr("Manage Radionuclide database"));
    mRadionuclideAct->setIcon(QIcon(":/std/model-radioactivity.png"));
    connect(mRadionuclideAct, SIGNAL(triggered()), this, SLOT(modelRadionuclides()));

    mScenarioAct = new QAction(tr("View &Scenario"), this);
    //mScenarioAct->setShortcuts(QKeySequence::HelpContents);
    mScenarioAct->setStatusTip(tr("Display scenario"));
    mScenarioAct->setIcon(QIcon(":/std/model-scenario.png"));
    connect(mScenarioAct, SIGNAL(triggered()), asExplorer, SLOT(displayScenario()));

    mMapAct = new QAction(tr("View &Map"), this);
    //mMapAct->setShortcuts(QKeySequence::HelpContents);
    mMapAct->setStatusTip(tr("Display map for current scenario"));
    mMapAct->setIcon(QIcon(":/std/model-map.png"));
    connect(mMapAct, SIGNAL(triggered()), asExplorer, SLOT(displayMap()));

    mMapPickLocationAct = new QAction(tr("&Pick Location"), this);
    //mMapPickLocationAct->setShortcuts(QKeySequence::HelpContents);
    mMapPickLocationAct->setStatusTip(tr("Pick location coordinate from map"));
    mMapPickLocationAct->setCheckable(true);
    mMapPickLocationAct->setIcon(QIcon(":/std/model-map-pick.png"));
    connect(mMapPickLocationAct, SIGNAL(toggled(bool)), mapView, SLOT(pickLocationClicked(bool)));

    mMapTrackLocationAct = new QAction(tr("&Track Coordinate"), this);
    //mMapTrackLocationAct->setShortcuts(QKeySequence::HelpContents);
    mMapTrackLocationAct->setStatusTip(tr("Track coordinate during mouse move"));
    mMapTrackLocationAct->setCheckable(true);
    mMapTrackLocationAct->setIcon(QIcon(":/std/model-map-track.png"));
    connect(mMapTrackLocationAct, SIGNAL(toggled(bool)), mapView, SLOT(trackLocationClicked(bool)));

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

    mEvalAct = new QAction(tr("&Evaluate"), this);
    //mCalcAct->setShortcuts(QKeySequence::HelpContents);
    mEvalAct->setStatusTip(tr("Evaluate scenario"));
    mEvalAct->setIcon(QIcon(":/std/model-calculate.png"));
    connect(mEvalAct, SIGNAL(triggered()), this, SLOT(modelCalculate()));

    mReportAct = new QAction(tr("R&eport"), this);
    //mReportAct->setShortcuts(QKeySequence::HelpContents);
    mReportAct->setStatusTip(tr("Generate model and calculation report"));
    mReportAct->setIcon(QIcon(":/std/model-report.png"));
    connect(mReportAct, SIGNAL(triggered()), asExplorer, SLOT(generateReport()));


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
    //vDisplayGridAct->setChecked(view->isDisplayGrid());
    vDisplayGridAct->setStatusTip(tr("Show/hide grid line"));
    vDisplayGridAct->setIcon(QIcon(":/std/grid-show.png"));
    connect(vDisplayGridAct, SIGNAL(triggered(bool)), view, SLOT(displayGrid(bool)));

    vSnapGridAct = new QAction(tr("&Snap to Grid"), this);
    vSnapGridAct->setCheckable(true);
    //vSnapGridAct->setChecked(view->isSnapToGrid());
    vSnapGridAct->setStatusTip(tr("Snap object to grid"));
    vSnapGridAct->setIcon(QIcon(":/std/grid-snap.png"));
    connect(vSnapGridAct, SIGNAL(triggered(bool)), view, SLOT(snapToGrid(bool)));

    vZoomInAct = new QAction(tr("Zoom In"), this);
    vZoomInAct->setStatusTip(tr("Increase zoom factor"));
    vZoomInAct->setIcon(QIcon(":/std/zoom-in.png"));
    connect(vZoomInAct, SIGNAL(triggered()), view, SLOT(zoomIn()));

    vZoomFitAct = new QAction(tr("Zoom Fit"), this);
    vZoomFitAct->setStatusTip(tr("Fit scenario into view"));
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
    fileToolBar->setObjectName("tbFile");
    fileToolBar->addAction(fQuitAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(mDatabaseAct);
    fileToolBar->addAction(mCloseDbAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(fNewAct);
    fileToolBar->addAction(fOpenAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(fSaveAct);
    fileToolBar->addAction(fSaveAsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(fPrintAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("tbEdit");
    editToolBar->addActions(eGroupEdit->actions());
    editToolBar->addSeparator();
    editToolBar->addAction(eClearAct);
    //editToolBar->addSeparator();
    editToolBar->addAction(eRenumberAct);
    //editToolBar->addSeparator();
    //editToolBar->addAction(this->eOptionsAct);

    modelToolBar = addToolBar(tr("Model"));
    modelToolBar->setObjectName("tbModel");
    modelToolBar->addSeparator();
    modelToolBar->addAction(mScenarioAct);
    modelToolBar->addAction(mVerifyAct);
    modelToolBar->addAction(mEvalAct);
    modelToolBar->addAction(mReportAct);
    modelToolBar->addSeparator();
    modelToolBar->addAction(mMapAct);
    modelToolBar->addAction(mMapPickLocationAct);
    modelToolBar->addAction(mMapTrackLocationAct);

    viewToolBar = addToolBar(tr("View"));
    viewToolBar->setObjectName("tbView");
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
    fileMenu->addAction(mDatabaseAct);
    fileMenu->addAction(mCloseDbAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fNewAct);
    fileMenu->addAction(fOpenAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fSaveAct);
    fileMenu->addAction(fSaveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fSaveAsPdfAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fPageSetupAct);
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
    //modelMenu->addAction(mDatabaseAct);
    //modelMenu->addSeparator();
    modelMenu->addAction(mRadionuclideAct);
    modelMenu->addAction(mLocationAct);
    modelMenu->addSeparator();
    modelMenu->addAction(mScenarioAct);
    modelMenu->addAction(mVerifyAct);
    modelMenu->addAction(mEvalAct);
    modelMenu->addSeparator();
    modelMenu->addAction(mReportAct);
    modelMenu->addSeparator();
    modelMenu->addAction(mMapAct);
    modelMenu->addAction(mMapPickLocationAct);
    modelMenu->addAction(mMapTrackLocationAct);

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
    bool hasViewSeparator = false;
    FactoryList factories = KApplication::selfInstance()->factories();
    foreach(IModelFactory * factory, factories) {
        //if is top level factory, add as new menu item (placed after model)
        if (factory->isTopLevel()) {
            QMenu * group = menuBar()->addMenu(factory->name());

            //Create dock widget to display the models
            QDockWidget *dock = new QDockWidget(factory->name(), this);
            dock->setObjectName(factory->name());
            dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
            XModelWidget * mw = new XModelWidget(dock);
            dock->setWidget(mw);
            addDockWidget(Qt::RightDockWidgetArea, dock);

            //adding separator in view menu
            if (!hasViewSeparator) {
                viewMenu->addSeparator();
                hasViewSeparator = true;
            }
            QAction * action = dock->toggleViewAction();
            action->setText(tr("Show ") + factory->name() + " Models");
            viewMenu->addAction(action);

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
void MainWindow::setupActionStates()
{
    bool enabled = KApplication::selfInstance()->hasStorage();
    bool isScnView = pages->currentWidget() == view && view->scene() != 0;
    bool isRepView = pages->currentWidget() == reportView;
    bool isMapView = pages->currentWidget() == mapView;
    bool hasAssessment = asExplorer->currentAssessment() != 0;
    bool hasContent = view->scene() != 0 || !reportView->document()->isEmpty();
    bool hasScn = view->scene() != 0;

    mCloseDbAct->setEnabled(enabled);
    fNewAct->setEnabled(enabled);
    fOpenAct->setEnabled(enabled);
    fSaveAct->setEnabled(enabled && hasAssessment);
    fSaveAsAct->setEnabled(enabled && hasAssessment);
    fSaveAsPdfAct->setEnabled(enabled && hasContent);
    fPrintPrevAct->setEnabled(enabled && hasContent);
    fPrintAct->setEnabled(enabled && hasContent);
    fPageSetupAct->setEnabled(enabled);

    eClearAct->setEnabled(enabled && isScnView);
    eRemoveAct->setEnabled(enabled && isScnView);
    eRemoveConAct->setEnabled(enabled && isScnView);
    eConnectAct->setEnabled(enabled && isScnView);
    eRenumberAct->setEnabled(enabled && isScnView);

    mLocationAct->setEnabled(enabled);
    mRadionuclideAct->setEnabled(enabled);
    mScenarioAct->setEnabled(enabled && !isScnView);
    mVerifyAct->setEnabled(enabled && isScnView);
    mEvalAct->setEnabled(enabled && isScnView);
    //mCalcStepAct->setEnabled(enabled);
    mReportAct->setEnabled(enabled && hasScn && !isRepView);
    mMapAct->setEnabled(enabled && hasScn && !isMapView);
    mMapPickLocationAct->setEnabled(enabled && isMapView);
    mMapTrackLocationAct->setEnabled(isMapView);
}
void MainWindow::changeScenario(KScenario * scenario)
{
    if (scenario != 0) {
        vSnapGridAct->setChecked(scenario->snapToGrid());
        vDisplayGridAct->setChecked(scenario->displayGrid());
    }
    else {
        vSnapGridAct->setChecked(false);
        vDisplayGridAct->setChecked(false);
    }
}

void MainWindow::displayGeoposition(const QString & strInfo)
{
    QStatusBar * sb = statusBar();
    if (!strInfo.isEmpty())
    {
        sb->showMessage(tr("Position is ") + strInfo);
    }
    else
    {
        sb->showMessage(strInfo);
    }
}

void MainWindow::clearAllViews()
{
    asExplorer->closeAllAssessment();
    reportView->clear();
    pages->setCurrentWidget(view);
    setupActionStates();
}

void MainWindow::modelTriggeredAction(IModelFactory* f, const KModelInfo & info)
{
    Q_ASSERT(f != 0);
    //Ask view to create model
    //view->createModel(f, info);
    asExplorer->createModel(f, info);
    xTrace() << "Create model: " << info.name();
}
void MainWindow::printRequested(QPrinter *printer)
{
    if (pages->currentWidget() == view)
        view->printDocument(printer);
    else if (pages->currentWidget() == reportView)
        reportView->print(printer);
}

void MainWindow::printPreview()
{
#if !defined(Q_OS_SYMBIAN)
    QPrintPreviewDialog dlg(printer, this);
    connect(&dlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printRequested(QPrinter*)));
    dlg.exec();
#endif
}

void MainWindow::pageSetup()
{
#if !defined(Q_OS_SYMBIAN)
    QPageSetupDialog dlg(printer, this);
    dlg.exec();
#endif
}

void MainWindow::printAll()
{
#if !defined(Q_OS_SYMBIAN)
    if (QPrintDialog(printer).exec() == QDialog::Accepted) {
        printRequested(printer);
    }
#endif
}
void MainWindow::saveAsPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"",
                                                     tr("PDF File (*.pdf);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        QPrinter pdf;
        pdf.setOutputFileName(fileName);
        pdf.setOutputFormat(QPrinter::PdfFormat);
        pdf.setOrientation(printer->orientation());
        pdf.setPaperSize(printer->paperSize());
        pdf.setResolution(printer->resolution());
        pdf.setFullPage(printer->fullPage());
        pdf.setCopyCount(1);
        printRequested(&pdf);
    }
}

void MainWindow::editOptions()
{
    DialogOption dlg;
    if (dlg.exec() == QDialog::Accepted)
        dlg.applyOptions();
}

void MainWindow::editMode(QAction * act)
{
    if (act == eNoneAct)
        view->setEditMode(KScenario::None);
    else if (act == eConnectAct)
        view->setEditMode(KScenario::Connect);
    else if (act == eRemoveConAct)
        view->setEditMode(KScenario::RemoveConnection);
    else if (act == eRemoveAct)
        view->setEditMode(KScenario::RemoveModel);
}
void MainWindow::modelDatabase()
{
    //create dialog
    QFileDialog dlg(this, tr("New/Open Database"), "", tr("Database file (*.db);;All files (*.*)"));
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setViewMode(QFileDialog::Detail);
    dlg.setConfirmOverwrite(false);
    dlg.setDefaultSuffix("db");
    dlg.setLabelText(QFileDialog::FileName, tr("Database name"));
    dlg.setReadOnly(false);
    dlg.setOption(QFileDialog::DontUseNativeDialog, false);
    dlg.setOption(QFileDialog::DontUseSheet, true);
    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    //get selected filename
    QStringList selList = dlg.selectedFiles();
    QString fileName = selList.first();
    KApplication * pm = KApplication::selfInstance();
    if (!fileName.isEmpty()) {
        if (!pm->hasStorage() || pm->storage()->storageName() != fileName) {
            pm->setStorage(fileName);
            clearAllViews();
        }
    }
}
void MainWindow::closeDatabase()
{
    // TODO: ask user for closing databases
    if (KApplication::selfInstance()->hasStorage())
    {
        clearAllViews();
        KApplication::selfInstance()->closeStorage();
        setupActionStates();
    }
}

void MainWindow::modelLocations()
{
    if (KApplication::selfInstance()->hasStorage()) {
        DialogLocation dlg(KApplication::selfInstance()->storage());
        dlg.exec();
        asExplorer->refreshScenario();
    }

}
void MainWindow::modelRadionuclides()
{
    if (KApplication::selfInstance()->hasStorage()) {
        DialogRadionuclide dlg(KApplication::selfInstance()->storage());
        dlg.exec();
        asExplorer->refreshScenario();
    }
}
void MainWindow::modelVerify()
{
    outView->clearContents();
    asExplorer->verifyScenario();
}

void MainWindow::modelCalculate()
{
    outView->clearContents();
    asExplorer->evaluateScenario();
}


void MainWindow::helpContent()
{
    QDir dir(KApplication::selfInstance()->applicationDirPath());
    const char * helpFile = "help.pdf";
    QString helpFilePath = dir.absoluteFilePath(helpFile);
    if (QFile::exists(helpFilePath))
    {
        QDesktopServices::openUrl(QUrl("file:///" + helpFilePath, QUrl::TolerantMode));
    }
    else
    {
        QMessageBox::information(this, tr("Help file"),
                                 QString(tr("Help file '%1' does not exists."))
                                 .arg(helpFilePath));
    }
}
void MainWindow::helpAbout()
{
    DialogAbout dlg;
    dlg.exec();
}
void MainWindow::switchView(int id, void *data)
{
    if (id == UiAssessmentExplorer::Report) {
        KReport * rep = reinterpret_cast<KReport*>(data);
        if (rep != 0)
            reportView->setHtml(rep->toString());
        pages->setCurrentWidget(reportView);
    }
    else if (id == UiAssessmentExplorer::Map) {
        //TODO
        //this is work, but should be replaced with efficient way
        //i.e. call display scenario when calculation/evaluation finished.
        KScenario * scenario = reinterpret_cast<KScenario*>(data);
        mapView->displayScenario(scenario);
        pages->setCurrentWidget(mapView);
    }
    else {
        pages->setCurrentWidget(view);
    }
    setupActionStates();
}
