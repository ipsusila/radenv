#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kgroupinfo.h"

class QAction;
class QActionGroup;
class QMenu;
class QStackedWidget;
class QTextEdit;
class QPrinter;
class KScenario;
class XModelView;
class IModelFactory;
class XModelWidget;
class UiOutputView;
class UiAssessmentExplorer;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(UiOutputView * vw, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void modelTriggeredAction(IModelFactory* f, const KModelInfo & info);
    void switchView(int id, void * data);
    void printRequested(QPrinter * printer);
    void setupActionStates();
    void changeScenario(KScenario * scenario);

    void printPreview();
    void pageSetup();
    void printAll();
    void saveAsPdf();

    void editOptions();
    void editMode(QAction * act);

    void modelDatabase();
    void closeDatabase();
    void modelLocations();
    void modelRadionuclides();
    void modelVerify();
    void modelCalculate();

    void helpContent();
    void helpAbout();

protected:
    void closeEvent(QCloseEvent *event);

private:

    void createViews();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void clearAllViews();

    void createPluginMenus();
    void populatePluginMenus(XModelWidget * mw, QMenu * groupMenu,
        IModelFactory * factory, GroupInfoList groupList);
    void populatePluginMenus(QMenu * groupMenu, IModelFactory * factory, GroupInfoList groupList);

    UiOutputView * outView;
    UiAssessmentExplorer * asExplorer;
    XModelView * view;
    QTextEdit * reportView;
    QStackedWidget * pages;
    QPrinter * printer;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *modelMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction * fNewAct;
    QAction * fOpenAct;
    QAction * fSaveAct;
    QAction * fSaveAsAct;
    QAction * fSaveAsPdfAct;
    QAction * fPrintPrevAct;
    QAction * fPrintAct;
    QAction * fPageSetupAct;
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
    QAction * mCloseDbAct;
    QAction * mMapAct;
    QAction * mLocationAct;
    QAction * mRadionuclideAct;
    QAction * mVerifyAct;
    QAction * mEvalAct;
    QAction * mEvalStepAct;
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
