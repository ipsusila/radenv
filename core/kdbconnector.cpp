#include "kdbconnector.h"
#include <QSharedData>
#include <QStringList>

class KDbConnectorPrivate : public QSharedData {
    QMap<QString, QString> _conMaps;
public:
    KDbConnectorPrivate(const QString& database)
    {
        parse(database);
        setupDatabase();
    }
    void parse(const QString& database)
    {
        //TODO
        //more rigid parsing
        /**
         * Generic rules
         *  From http://www.connectionstrings.com/formating-rules-for-connection-strings/
            1. All blank characters, except those placed within a value or within quotation marks, are ignored
            2. Blank characters will though affect connection pooling mechanism, pooled connections must have the exact same connection string
            3. If a semicolon (;) is part of a value it must be delimited by quotation marks (")
            4. Use a single-quote (') if the value begins with a double-quote (")
            5. Conversely, use the double quote (") if the value begins with a single quote (')
            6. No escape sequences are supported
            7. The value type is not relevant
            8. Names are case iNsEnSiTiVe
            9. If a KEYWORD=VALUE pair occurs more than once in the connection string, the value associated with the last occurrence is used
            10. However, if the provider keyword occurs multiple times in the string, the first occurrence is used.
            11. If a keyword contains an equal sign (=), it must be preceded by an additional equal sign to indicate that it is part of the keyword.
            12. If a value has preceding or trailing spaces it must be enclosed in single- or double quotes, ie Keyword=" value ", else the spaces are removed.

         */
        QStringList items = database.split(';', QString::SkipEmptyParts);
        for(int k = 0; k < items.size(); k++) {
            QStringList namVal = items.at(k).split('=');
            if(namVal.size() == 1)
                _conMaps[namVal.at(0)] = QString();
            else if (namVal.size() == 2)
                _conMaps[namVal.at(0)] = namVal.at(1);
        }
    }
    void setupDatabase()
    {

    }

};

KDbConnector::KDbConnector(const QString &database)
    : dptr(new KDbConnectorPrivate(database))
{
}

KDbConnector::KDbConnector(const KDbConnector &rhs) : dptr(rhs.dptr)
{
}

KDbConnector &KDbConnector::operator=(const KDbConnector &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KDbConnector::~KDbConnector()
{
}
