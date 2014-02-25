#include <QtDebug>
#include "xoutputview.h"

XOutputView::XOutputView(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);

    connect(this, SIGNAL(outputClear()), this, SLOT(onOutputClear()));
    connect(this, SIGNAL(outputNewLine()), this, SLOT(onOutputNewLine()));
    connect(this, SIGNAL(outputReady(QString)), this, SLOT(onOutputReady(QString)));
}
XOutputView::~XOutputView()
{
    qDebug() << "Output handler (view) destroyed";
}

void XOutputView::onOutputReady(const QString& msg)
{
    this->writeTimestamp();
    this->insertHtml(msg);
}

void XOutputView::onOutputNewLine()
{
    if (!this->document()->isEmpty()) {
        this->moveCursor(QTextCursor::End);
        this->insertHtml("<br>");
    }
}

void XOutputView::onOutputClear()
{
    _timestamp = QDateTime();
    this->clear();
}

void XOutputView::writeTimestamp()
{
    //ensure cursor at end position
    this->moveCursor(QTextCursor::End);

    //check timestamp
    QDateTime curTime = QDateTime::currentDateTime();
    if (_timestamp.isNull() || _timestamp.addSecs(5) <= curTime) {
        if (!_timestamp.isNull())
            this->insertHtml("<br>");
        _timestamp = curTime;
        this->insertHtml(_timestamp.toString("yyyy-MM-dd hh:mm:ss") + "<br>");
    }
}

bool XOutputView::isHtml() const
{
    return true;
}

void XOutputView::clearContents()
{
    emit outputClear();
}
void XOutputView::newLine()
{
    emit outputNewLine();
}

void XOutputView::traceOut(const QString& text)
{
    QString msg = QString(tr("<font color='#0000aa'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");
    emit outputReady(msg);
}
void XOutputView::infoOut(const QString& text)
{
    QString msg = QString(tr("<font color='#000000'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");
    emit outputReady(msg);
}
void XOutputView::warningOut(const QString& text)
{
    QString msg = QString(tr("<font color='#6d5608'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");
    emit outputReady(msg);
}
void XOutputView::errorOut(const QString& text)
{
    QString msg = QString(tr("<font color='#bb0000'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");

    emit outputReady(msg);
}
