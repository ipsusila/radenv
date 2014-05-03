#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kgroupinfo.h"

class QAction;
class QActionGroup;
class QMenu;
class QStackedWidget;
class KModelScene;
class XModelView;
class IModelFactory;
class XModelWidget;
class XOutputView;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(XOutputView * vw, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void modelTriggeredAction(IModelFactory* f, const KModelInfo & info);

    void newAssessment();
    void openAssessment();
    void saveAssessment();
    void saveAssessmentAs();
    void printPreview();
    void printSetup();
    void printAll();

    void editOptions();
    void editMode(QAction * act);

    void modelDatabase();
    void modelLocations();
    void modelRadionuclides();
    void modelVerify();
    void modelCalculate();

    void helpContent();
    void helpAbout();

protected:
    void closeEvent(QCloseEvent *event);

private:

    void createView();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    void createPluginMenus();
    void populatePluginMenus(XModelWidget * mw, QMenu * groupMenu,
        IModelFactory * factory, GroupInfoList groupList);
    void populatePluginMenus(QMenu * groupMenu, IModelFactory * factory, GroupInfoList groupList);

    XOutputView * outView;
    KModelScene * scene;
    XModelView * view;
    QStackedWidget * pages;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *modelMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction * fNewAct;
    QAction * fOpenAct;
    QAction * fSaveAct;
    QAction * fSaveAsAct;
    QAction * fPrintPrevAct;
    QAction * fPrintAct;
    QAction * fPrintSetupAct;
    QAction * fQuitAct;

    QAction * eClearAct;
    QAction * eNoneAct;
    QAction * eRemoveAct;
    QAction * eRemoveConAct;
    QAction * eConnectAct;
    QAction * eRenumberAct;
    QAction * eOptionsAct;
    QActionGroup * eGroupEdit;

    QAction * mDatabaseAct;
    QAction * mMapAct;
    QAction * mLocationAct;
    QAction * mRadionuclideAct;
    QAction * mVerifyAct;
    QAction * mCalcAct;
    QAction * mCalcStepAct;
    QAction * mReportAct;

    QAction * vZoomFitAct;
    QAction * vZoomInAct;
    QAction * vZoomOutAct;
    QAction * vZoomOrigAct;
    QAction * vDisplayGridAct;
    QAction * vSnapGridAct;

    QAction * hHelpAct;
    QAction * hAboutAct;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *modelToolBar;
    QToolBar *viewToolBar;
};

#endif // MAINWINDOW_H
