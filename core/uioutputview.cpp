#include<QtDebug>
#include "uioutputview.h"

UiOutputView::UiOutputView(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);
    this->setFont(QFont("monospace", 10));

    connect(this, SIGNAL(outputClear()), this, SLOT(onOutputClear()));
    connect(this, SIGNAL(outputNewLine()), this, SLOT(onOutputNewLine()));
    connect(this, SIGNAL(outputReady(QString)), this, SLOT(onOutputReady(QString)));
}
UiOutputView::~UiOutputView()
{
    qDebug() << "~Output handler (view) destroyed";
}

void UiOutputView::onOutputReady(const QString& msg)
{
    this->writeTimestamp();
    this->insertHtml(msg);
}

void UiOutputView::onOutputNewLine()
{
    if (!this->document()->isEmpty()) {
        this->moveCursor(QTextCursor::End);
        this->insertHtml("<br>");
    }
}

void UiOutputView::onOutputClear()
{
    _timestamp = QDateTime();
    this->clear();
}

void UiOutputView::writeTimestamp()
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

bool UiOutputView::isHtml() const
{
    return true;
}

void UiOutputView::clearContents()
{
    emit outputClear();
}
void UiOutputView::newLine()
{
    emit outputNewLine();
}

void UiOutputView::traceOut(const QString& text)
{
    QString msg = QString(tr("<font color='#0000aa'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");
    emit outputReady(msg);
}
void UiOutputView::infoOut(const QString& text)
{
    QString msg = QString(tr("<font color='#000000'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");
    emit outputReady(msg);
}
void UiOutputView::warningOut(const QString& text)
{
    QString msg = QString(tr("<font color='#6d5608'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");
    emit outputReady(msg);
}
void UiOutputView::errorOut(const QString& text)
{
    QString msg = QString(tr("<font color='#bb0000'>%1 </font>"))
            .arg(text);
    msg.replace("\r\n", "<br>").replace("\n", "<br>")
            .replace("\t", "&nbsp;&nbsp;").replace("  ", "&nbsp;&nbsp;");

    emit outputReady(msg);
}
