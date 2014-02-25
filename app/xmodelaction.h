#ifndef XMODELACTION_H
#define XMODELACTION_H

#include <QAction>
#include <QtDebug>
#include "kmodelinfo.h"

class IModelFactory;

/**
 * @brief Custom action for executing model information.
 * Ref: http://schneide.wordpress.com/2010/07/26/non-trivial-custom-data-in-qactions/
 */
class XModelAction : public QAction
{
    Q_OBJECT
public:
    XModelAction(IModelFactory * f, const KModelInfo& inf, QObject *parent = 0)
        : QAction(inf.text(), parent), _factory(f), _info(inf)
    {
        if (inf.icon().isNull()) {
            QIcon ico(":/std/sort.png");
            this->setIcon(ico);
        }
        else
            this->setIcon(inf.icon());
        connect(this, SIGNAL(triggered()), this, SLOT(onTriggered()));
    }

private:
    IModelFactory * _factory;
    KModelInfo _info;

signals:
    /**
     * @brief Signals that will be emitted when the action is triggered by user.
     * The signals send factory and info as the parameter.
     * @param f
     * @param info
     */
    void triggered(IModelFactory* f, const KModelInfo & info);

private slots:
    /**
     * @brief Action is called by the QAction's triggered().
     * This slots will emits XModelAction::triggered(XModelFactory*, const XModelInfo &)
     */
    void onTriggered() {
        emit triggered(_factory, _info);
    }
};

#endif // XMODELACTION_H
