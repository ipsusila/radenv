#ifndef UIOUTPUTVIEW_H
#define UIOUTPUTVIEW_H

#include <QTextEdit>
#include "koutput.h"

class K_CORE_EXPORT UiOutputView : public QTextEdit, public KOutput
{
    Q_OBJECT
public:
    explicit UiOutputView(QWidget *parent = 0);
    ~UiOutputView();

    virtual void clearContents();
    virtual void newLine();
    virtual bool isHtml() const;

protected:
    virtual void traceOut(const QString& text);
    virtual void infoOut(const QString& text);
    virtual void warningOut(const QString& text);
    virtual void errorOut(const QString& text);

private:
    void writeTimestamp();

    QDateTime _timestamp;

signals:
    void outputReady(const QString& msg);
    void outputNewLine();
    void outputClear();

public slots:

    void onOutputReady(const QString& msg);
    void onOutputNewLine();
    void onOutputClear();

};

#endif // UIOUTPUTVIEW_H
