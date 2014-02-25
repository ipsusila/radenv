#ifndef XOUTPUTVIEW_H
#define XOUTPUTVIEW_H

#include <QTextEdit>
#include "koutput.h"

class XOutputView : public QTextEdit, public KOutput
{
    Q_OBJECT
public:
    explicit XOutputView(QWidget *parent = 0);
    ~XOutputView();

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

#endif // XOUTPUTVIEW_H
