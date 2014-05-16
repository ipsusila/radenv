#include "koptions.h"
#include <QSharedData>
#include <QSize>
#include <QRect>

class KOptionsPrivate : public QSharedData {
private:
    QSize _defSceneSize;
    int _maxInpWidth;
    int _minInpWidth;
    int _numPrecision;
public:
    KOptionsPrivate() : _defSceneSize(1600, 800),
        _maxInpWidth(200), _minInpWidth(50), _numPrecision(6)
    {

    }

    inline QRect scenarioSceneRect() const
    {
        return QRect(- _defSceneSize.width()/2, - _defSceneSize.height()/2,
                     _defSceneSize.width(), _defSceneSize.height());
    }
    inline QSize scenarioSceneSize() const
    {
        return _defSceneSize;
    }
    inline void setScenarioSceneSize(const QSize & size)
    {
        _defSceneSize = size;
    }

    inline void setScenarioSceneSize(int w, int h)
    {
        _defSceneSize = QSize(w, h);
    }

    inline int maxInputWidth() const
    {
        return _maxInpWidth;
    }
    inline void setMaxInputWidth(int w)
    {
        _maxInpWidth = w;
    }

    inline int minInputWidth() const
    {
        return _minInpWidth;
    }
    inline void setMinInputWidth(int w)
    {
        _minInpWidth = w;
    }

    inline int numberPrecision() const
    {
        return _numPrecision;
    }
    inline void setNumberPrecision(int digit)
    {
        _numPrecision = digit;
    }
};

KOptions::KOptions() : dptr(new KOptionsPrivate)
{
}

KOptions::KOptions(const KOptions &rhs) : dptr(rhs.dptr)
{
}

KOptions &KOptions::operator=(const KOptions &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KOptions::~KOptions()
{
}

QRect KOptions::scenarioSceneRect() const
{
    return dptr->scenarioSceneRect();
}
QSize KOptions::scenarioSceneSize() const
{
    return dptr->scenarioSceneSize();
}
void KOptions::setScenarioSceneSize(const QSize & size)
{
    dptr->setScenarioSceneSize(size);
}
void KOptions::setScenarioSceneSize(int w, int h)
{
    dptr->setScenarioSceneSize(w, h);
}

int KOptions::maxInputWidth() const
{
    return dptr->maxInputWidth();
}
void KOptions::setMaxInputWidth(int w)
{
    dptr->setMaxInputWidth(w);
}

int KOptions::minInputWidth() const
{
    return dptr->minInputWidth();
}
void KOptions::setMinInputWidth(int w)
{
    dptr->setMinInputWidth(w);
}

int KOptions::numberPrecision() const
{
    return dptr->numberPrecision();
}
void KOptions::setNumberPrecision(int digit)
{
    dptr->setNumberPrecision(digit);
}
