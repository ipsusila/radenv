#include "khtmlreport.h"
#include "kassessment.h"
#include "kscenario.h"
#include "imodel.h"
#include "kgroup.h"
#include "kapplication.h"
#include "koptions.h"

KHtmlReport::KHtmlReport()
{
}

KHtmlReport::~KHtmlReport()
{
}

void KHtmlReport::clear()
{
    //nothing
}

void KHtmlReport::beginReport()
{
     KReport::beginReport();

     QTextStream * stream = textStream();
     Q_ASSERT(stream);

     (*stream) << "<!DOCTYPE html><html><head><title>Assessment report</title>"
               << "<style>body {margin-left: 20px; margin-top: 20px; margin-right: 20px; margin-bottom: 20px;}\n"
               << "table{border-color:#000000;}</style></head><body>";
}
void KHtmlReport::endReport()
{
    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    (*stream) << "</body></html>";
    releaseStream();
}

void KHtmlReport::add(const KAssessment& asObj)
{
    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    (*stream) << "<h2>Assessment: " << xEscHtml(asObj.name()) << "</h2><br>"
              << "<table border='0' cellpadding='3' cellspacing='0'><tr><td>Created</td><td>:</td><td>"
              << asObj.created().toString("yyyy/MM/dd hh:mm:ss") << "</td></tr>"
              << "<tr><td>Author</td><td>:</td><td>" << xEscHtml(asObj.author()) << "</td></tr>"
              << "<tr><td>Description</td><td>:</td><td>" << xEscHtml(asObj.description()) << "</td></tr>"
              << "<tr><td>Remark</td><td>:</td><td>" << xEscHtml(asObj.remark()) << "</td></tr>"
              << "</table><br>";
}
void KHtmlReport::add(const KScenario& scenario)
{
    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    (*stream) << "<hr><h3>" << xEscHtml(scenario.name()) << "</h3>"
              << "<p>" << xEscHtml(scenario.description()) << "</p>";
}

void KHtmlReport::add(IModel * model)
{
    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    const KModelInfo & info = model->info();
    (*stream) << "<hr><h4>" << xEscHtml(model->tagName()) << ":" << xEscHtml(info.text()) << "</h4>"
              << "<p>" << xEscHtml(info.description()) << "</p>";
}
void KHtmlReport::add(const KLocation& loc)
{
    if (!loc.isValid())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);
    (*stream) << "<h4>" << xEscHtml(loc.name()) << "</h4>"
              << "<table border='0' cellpadding='3' cellspacing='0'><tr><td>Location</td><td>:</td><td>"
              << loc.distance() << " m," << QObject::tr("angle ") << loc.angle() << "</td></tr>"
              << "<tr><td>Position</td><td>:</td><td>Long: " << loc.longitude() << " ,Lat: " << loc.latitude() << "</td></tr>"
              << "<tr><td>Description</td><td>:</td><td>" << xEscHtml(loc.description()) << "</td></tr>"
              << "</table>";
}
void KHtmlReport::add(const KDataGroupArray & dga)
{
    if (dga.isEmpty())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);
    (*stream) << "<h4>Input Parameters</h4>" << dga.toHtml();
}
void KHtmlReport::add(const KDataArray & da)
{
    if (da.isEmpty())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);
    (*stream) << "<h4>Parameters</h4>" << da.toHtml();
}
void KHtmlReport::add(const KDataTable & table)
{
    if (table.isEmpty())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);
    (*stream) << "<h4>Result/Output</h4>" << table.toHtml();
}

