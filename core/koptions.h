#ifndef KOPTIONS_H
#define KOPTIONS_H

#include "radglobal.h"

class KOptionsPrivate;

class RADENV_API KOptions
{
public:
    KOptions();
    KOptions(const KOptions &);
    KOptions &operator=(const KOptions &);
    ~KOptions();

    QRect scenarioSceneRect() const;
    QSize scenarioSceneSize() const;
    void setScenarioSceneSize(const QSize & size);
    void setScenarioSceneSize(int w, int h);

    int maxInputWidth() const;
    void setMaxInputWidth(int w);
    int minInputWidth() const;
    void setMinInputWidth(int w);
    int numberPrecision() const;
    void setNumberPrecision(int digit);

private:
    QSharedDataPointer<KOptionsPrivate> dptr;
};

#endif // KOPTIONS_H
