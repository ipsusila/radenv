#ifndef IPARAMSEDITOR_H
#define IPARAMSEDITOR_H

#include "radglobal.h"

/**
 * @brief Interface to parameter editor.
 */
class IParamsEditor
{
public:
    virtual ~IParamsEditor() {}

    virtual bool initialize() = 0;
    virtual int paramsCount() const = 0;
    virtual QString paramsLabel(int idx) const = 0;
    virtual QString paramsDescription(int idx) const = 0;
    virtual QIcon paramsIcon(int idx) const = 0;
    virtual void showEditor(int idx, QWidget * parent = 0) = 0;
};

#endif // IPARAMSEDITOR_H
