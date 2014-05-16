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
              << "<table border='0' cellpadding='5' cellspacing='0'><tr><td>Created</td><td>:</td><td>"
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
              << "<table border='0' cellpadding='5' cellspacing='0'><tr><td>Location</td><td>:</td><td>"
              << loc.distance() << " m," << QObject::tr("angle ") << loc.angle() << "</td></tr>"
              << "<tr><td>Position</td><td>:</td><td>Lat:" << loc.latitude() << ",Long:" << loc.longitude() << "</td></tr>"
              << "<tr><td>Description</td><td>:</td><td>" << xEscHtml(loc.description()) << "</td></tr>"
              << "</table>";
}
void KHtmlReport::add(const KDataGroupArray & dga)
{
    if (dga.isEmpty())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    (*stream) << "<h4>Input Parameters</h4><table border='1' cellpadding='5' cellspacing='0'>"
              << "<tr><th>Quantity</th><th>Value</th><th>Unit</th></tr>";
    for(int k = 0; k < dga.size(); k++) {
        const DataGroup & group = dga.at(k);
        if (group.isEmpty())
            continue;

        (*stream) << "<tr><td colspan='3'>" << xEscHtml(group.name) << "</td></tr>";
        for(int n = 0; n < group.count(); n++) {
            const KData & d = group.itemAt(n);
            const Quantity * qty = d.quantityPtr();
            if (qty->isComment()) {
                (*stream) << "<tr><td colspan='3'>" << xEscHtml(d.toString()) << "</td></tr>";
            }
            else if (d.contains(KData::Array)) {
                (*stream) << "<tr><td colspan='3'><em>" << xEscHtml(qty->displayText())
                          << ":" << xEscHtml(qty->description) << "</em></td></tr>";
                for(int m = 0; m < d.count(); m++) {
                    const KDataItem & di = d.at(m);
                    (*stream) << "<tr><td>" << xEscHtml(di.name()) << "</td>"
                              << "<td>" << di.toString(qty) << "</td>"
                              << "<td>" << xEscHtml(qty->unit) << "</td></tr>";
                }
            }
            else {
                (*stream) << "<tr><td>" << xEscHtml(qty->displayText()) << "</td>"
                          << "<td>" << d.toString() << "</td>"
                          << "<td>" << xEscHtml(qty->unit) << "</td></tr>";
            }
        }
    }

    (*stream) << "</table>\n";
}
void KHtmlReport::add(const KDataArray & da)
{
    if (da.isEmpty())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    (*stream) << "<h4>Parameters</h4><table border='1' cellpadding='5' cellspacing='0'>"
              << "<tr><th>Quantity</th><th>Value</th><th>Unit</th></tr>";
    for(int k = 0; k < da.size(); k++)
    {
        const KData & d = da.at(k);
        const Quantity * qty = d.quantityPtr();
        if (qty->isComment()) {
            (*stream) << "<tr><td colspan='3'>" << xEscHtml(d.toString()) << "</td></tr>";
        }
        else if (d.contains(KData::Array)) {
            (*stream) << "<tr><td colspan='3'><em>" << xEscHtml(qty->displayText())
                      << ":" << xEscHtml(qty->description) << "</em></td></tr>";
            for(int m = 0; m < d.count(); m++) {
                const KDataItem & di = d.at(m);
                (*stream) << "<tr><td>" << di.name() << "</td>"
                          << "<td>" << di.toString(qty) << "</td>"
                          << "<td>" << xEscHtml(qty->unit) << "</td></tr>";
            }
        }
        else {
            (*stream) << "<tr><td>" << xEscHtml(qty->displayText()) << "</td>"
                      << "<td>" << d.toString() << "</td>"
                      << "<td>" << xEscHtml(qty->unit) << "</td></tr>";
        }
    }
    (*stream) << "</table>\n";
}
void KHtmlReport::add(const KDataTable & table)
{
    if (table.isEmpty())
        return;

    QTextStream * stream = textStream();
    Q_ASSERT(stream);

    int nrow = table.rowCount();
    int ncol = table.columnCount();
    (*stream) << "<h4>Result/Output</h4><table border='1' cellpadding='5' cellspacing='0'>";

    //column header (first column is empty)
    (*stream) << "<tr><th>&nbsp;</th>";
    for(int c = 0; c < ncol; c++)
        (*stream) << "<th>" << xEscHtml(table.columnHeader(c)) << "</th>";
    (*stream) << "</tr>";

    //contents
    for(int r = 0; r < nrow; r++) {
        //first column
        (*stream) << "<tr><td>" << xEscHtml(table.rowHeader(r)) << "</td>";

        //next column
        for(int c = 0; c < ncol; c++) {
            QString vstr = table.value(r, c).toString();
            double num;
            bool ok;
            num = vstr.toDouble(&ok);
            if (ok)
                vstr = QString::number(num, 'g', KApplication::selfInstance()->options()->numberPrecision());
            (*stream) << "<td>" << xEscHtml(vstr) << "</td>";
        }
        (*stream) << "</tr>";
    }

    (*stream) << "</table>\n";
}

