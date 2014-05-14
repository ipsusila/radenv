#ifndef DIALOGASSESSMENTBROWSER_H
#define DIALOGASSESSMENTBROWSER_H

#include <QDialog>
#include "radglobal.h"

namespace Ui {
class DialogAssessmentBrowser;
}

class QListWidgetItem;
class DialogAssessmentBrowser : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAssessmentBrowser(const QStringList & excludes, QWidget *parent = 0);
    ~DialogAssessmentBrowser();
    AssessmentList assessments(QObject * parent) const;

protected:
    void displayAssessment(KAssessment * aP);
    void loadAllAssessments(const QStringList & excludes);
    
private slots:
    void on_lbAssessment_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onItemClicked();

private:
    Ui::DialogAssessmentBrowser *ui;
};

#endif // DIALOGASSESSMENTBROWSER_H
