#ifndef _CONTROLHELPER_H_
#define _CONTROLHELPER_H_

#include "../Controls/Control.h"
#include "../Controls/ControlUser.h"
#include <QObject>
#include <QMutex>
#include <QStringList>

/**
 * This class is the glue between the GUI and the Control tree. It will send out qt events whenever
 * the controls value is changed, and it can also change the controls value by calling setvalue.
 *
 * It is used by simply new'ing it in the constructor of the widget that is using it. Upon
 * construction, it will search the parent for hints on what control id it should be connected to,
 * and after that generally just stay in the background until events come and go.
 */
class ControlHelper : public QObject, public NodeUser
{
    Q_OBJECT

    public:
        /** 
         * @param parent The parent widget that uses this controlhelper.
         */
        ControlHelper(QObject *parent);

        /**
         * A special variant of the controlhelper that monitors all events for property changes in
         * order to let the controlId property mechanism working
         */
        ControlHelper(class QCoreApplication *app);

        /** 
         * @brief The Qt event handler, reimplementedfor watching the parent widget for property
         * changes that would change the current control etc.
         */
        bool eventFilter ( QObject * watched, QEvent * event );
        /** 
         * @return The absolute id for the control this controlhelper is assigned to
         */
        QString getControlId();

        /** 
         * @brief Set the absolute id of the control to watch, and register with it.
         * 
         * @param absoluteId The id to the control. If this string is empty, the controlhelper will
         * disconnect form the current control.
         */
        void setControl(QString absoluteId);


        /** 
         * @brief Tell the connected control to send an event with its current value. Usually this
         * should happen automatically.
         */
        //void requestValue();

        /**
         * @breif Get the current char value of the control
         */
        char getValue();

        /** 
         * @brief The reimplemented event handler from the control tree.
         */
        void handleEvent(Event *event);

    public slots:
        /** 
        * @brief Set the value of the control.
        * 
        * @param value the value to set
        */
        void setValue(char value);

        /** 
        * @brief Convenience function due to many qt controls using int in their signals. Will just
        * call setValue(char)
        */
        void setValue(int value);

        /** 
        * @brief Convenience function for checkboxes. Will just
        * call setValue(char)
        */
        void setValue(bool value);

        /** 
         * @brief Tell the control in the backend to start MIDI learning.
         * TODO: make it show in the gui what its doing
         */
        void MIDILearn();

        /** 
         * @brief Recurse up the parent chain and set control according to qt properties
         */
        void updateControlId();

        /** 
         * @brief Find the absolute control id for this widget, if there is one
         * 
         * @param widget the widget to start recursion from
         * 
         * @return the absolute control id of the widget, or an empty string if there was no id
         * found
         */
        static QString findComponentPath(QObject *object);

    signals:
        /** 
         * @brief This signal is emitted from the controlhelper when the value of the controls value
         * has changed. should be connected to a slot that changes the value in the gui.
         */
        void valueChanged(int value);

        /** 
         * @brief For controls that have a set of string describing the values (options), this
         * signal will be emitted with a list of them when the controlhelper has connected to a new
         * control
         */
        void optionsChanged(QStringList options);

    private:
        void emitOptions();

        GenControl *m_control;
        QString m_controlId;
        int expectedValueEvents;
        QMutex expectedEventMutex;
};

#endif /* ifndef _CONTROLHELPER_H_ */
