#include <QTextStream>
#include "kreport.h"
#include "koutput.h"
#include "kcase.h"
#include "imodel.h"
#include "kgroup.h"

KReport::KReport() : _name("[Default]"), _stream(0)
{
}
KReport::KReport(const KReport &rhs)
{
    Q_UNUSED(rhs);
}
KReport & KReport::operator=(const KReport &rhs)
{
    Q_UNUSED(rhs);
    return *this;
}

KReport::~KReport()
{
    delete _stream;
}

QString KReport::name() const
{
    return _name;
}
void KReport::setName(const QString& n)
{
    _name = n;
}
void KReport::beginReport()
{
     //initialize report
    _buffer.clear();
    if (_stream)
        delete _stream;
    _stream = new QTextStream(&_buffer);
    _stream->setCodec("UTF-8");
}
void KReport::endReport()
{
    delete _stream;
    _stream = 0;

    //write to output window
    QString txt = toPlainText();
    KOutput & out = KOutput::handler();
    if (out.isHtml()) {
        txt.replace('<', "&lt;")
            .replace('>', "&gt;")
            .replace(Rad::LatinEndLine, "<br>");
    }
    out.info(txt);
    out.newLine();
}

QString KReport::toPlainText()
{
    if (_buffer.isEmpty())
        return QString();
    return QString::fromUtf8(_buffer.constData());
}

/*
void KReport::printData(const KData& d)
{
    Q_ASSERT(_stream);
    if (d.quantity() == Rad::CommentQuantity)
        (*_stream) << "//" << d.value().toString() << Rad::LatinEndLine;
    else if (d.isValid())
        (*_stream) << d.displayText() << Rad::LatinEndLine;
}
*/

void KReport::addAssessment(const KCase& caseObj)
{
    Q_ASSERT(_stream);
    //display to info
    (*_stream)  << Rad::LatinEndLine << "---------------------------------------------------------------------------------" << Rad::LatinEndLine
                << QObject::tr("Created     : ") << caseObj.created().toString("yyyy/MM/dd hh:mm:ss") << Rad::LatinEndLine
                << QObject::tr("Name        : ") << caseObj.name()  << Rad::LatinEndLine
                << QObject::tr("Author      : ") << caseObj.author()  << Rad::LatinEndLine
                << QObject::tr("Description : ") << caseObj.description()  << Rad::LatinEndLine
                << QObject::tr("Remark      : ") << caseObj.remark()  << Rad::LatinEndLine
                << QObject::tr("Document    : ") << caseObj.docname() << Rad::LatinEndLine;
}
void KReport::addModel(IModel * model)
{
    Q_ASSERT(_stream);
    const KModelInfo & info = model->info();
    (*_stream)  << Rad::LatinEndLine << "=================================================================================" << Rad::LatinEndLine
                << QObject::tr("Model ") << model->tagName() << " " << info.text() << Rad::LatinEndLine
                << info.description() << Rad::LatinEndLine;
}
void KReport::addLocation(const KLocation& loc)
{
    Q_ASSERT(_stream);
    if (!loc.isValid())
        return;

    (*_stream)  << QObject::tr("Location    : ") << loc.code() << "," << loc.name() << Rad::LatinEndLine
                << QObject::tr("Description : ") << loc.description() << Rad::LatinEndLine
                << QObject::tr("Distance    : ") << loc.distance() << " m," << QObject::tr("angle ") << loc.angle() << Rad::LatinEndLine
                << QObject::tr("Latitude    : ") << loc.latitude() << Rad::LatinEndLine
                << QObject::tr("Longitude   : ") << loc.longitude() << Rad::LatinEndLine;
}
void KReport::addUserInputs(const KDataGroupArray & dga)
{
    Q_ASSERT(_stream);
    if (dga.isEmpty())
        return;

    (*_stream) << Rad::LatinEndLine << "---------------------------------------------------------------------------------" << Rad::LatinEndLine;
    for(int k = 0; k < dga.size(); k++) {
        const DataGroup & group = dga.at(k);
        if (group.isEmpty())
            continue;

        (*_stream) << QObject::tr("@Group : ") << group.name << Rad::LatinEndLine;
        for(int n = 0; n < group.count(); n++)
            (*_stream) << group.itemAt(n);
    }
}
void KReport::addResult(const KDataArray & da)
{
    Q_ASSERT(_stream);
    (*_stream) << da;
    /*
    if (da.isEmpty())
        return;

    (*_stream) << Rad::LatinEndLine << "---------------------------------------------------------------------------------" << Rad::LatinEndLine;
    for(int k = 0; k < da.size(); k++)
        printData(da.at(k));
    */
}
void KReport::addResult(const KDataTable & table)
{
    Q_ASSERT(_stream);
    (*_stream) << table;
}
