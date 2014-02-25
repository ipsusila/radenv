#include <QtDebug>
#include "xactionbutton.h"

XActionButton::XActionButton(QWidget *parent) :
    QPushButton(parent)
{
    actionOwner = NULL;
//    setMinimumSize(16,16);
//    setMaximumSize(32,32);

    //set stylesheet
    //test
    /*
    setStyleSheet("background-color: gray;"
                  "border-style: outset;"
                  "border-width: 2px;"
                  "border-radius: 1px;"
                  "border-color: beige;"
                  "font: bold 10px;"
                  "padding: 4px;");
    */
}

void XActionButton::setAction(QAction *action)
{

    // if I've got already an action associated to the button
    // remove all connections

    if( actionOwner != NULL && actionOwner != action ){
        disconnect( actionOwner,
                    SIGNAL(changed()),
                    this,
                    SLOT(updateButtonStatusFromAction()) );

        disconnect( this,
                    SIGNAL(clicked()),
                    actionOwner,
                    SLOT(trigger()) );
    }
    // store the action
    actionOwner = action;

    // configure the button
    updateButtonStatusFromAction();

    // connect the action and the button
    // so that when the action is changed the
    // button is changed too!
    connect( action,
             SIGNAL(changed()),
             this,
             SLOT(updateButtonStatusFromAction()));

    // connect the button to the slot that forwards the
    // signal to the action
    connect( this,
             SIGNAL(clicked()),
             actionOwner,
             SLOT(trigger()) );
}

void XActionButton::updateButtonStatusFromAction()
{
    if (actionOwner->icon().isNull())
        setText(actionOwner->text());
    else
        setIcon(actionOwner->icon());
    setStatusTip(actionOwner->statusTip());
    setToolTip(actionOwner->toolTip());
    setEnabled(actionOwner->isEnabled());
}
