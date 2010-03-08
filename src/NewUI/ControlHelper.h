/*
  ZynAddSubFX - a software synthesizer

  ControlHelper.h - The glue between the ZynAddSubFX backend and GUI

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

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
 * In the UI files, widgets may set custom properties "controlId" and "absoluteControlId". These
 * specify what control they should be connected to in an hierarchical fashion. There are basically
 * two ways to use this:
 * (1) a widget has a absoluteControlId set. It will simply be connected to this control
 * (2) a widget has controlId set, and a parent (or a parent's parent etc) has absoluteControlId
 *     set. the widget will be connected to the first absoluteControlId + a concatenation of the
 *     controlId's of the parent widgets. Example: a window has absoluteControlId = "Master.Synth",
 *     a panel in this window has controlId = "Filter", and a widget inside has controlId = "Cutoff". 
 *     This widget will be connected to the control with the path 
 *     "Master.Synth.Filter.Cutoff"
 *
 * ControlHelper is used by simply new'ing it in the constructor of the widget that is using it. Upon
 * construction, it will search the parent for hints on what control id it should be connected to,
 * and after that generally just stay in the background until events come and go.
 */
class ControlHelper:public QObject, public NodeUser
{
    Q_OBJECT

    public:
        /**
         * @param parent The parent widget that uses this controlhelper.
         */
        ControlHelper(QObject *parent);
        virtual ~ControlHelper();

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
         * @breif Get the current value of the control
         */
        int getValue();

        /**
         * @brief The reimplemented event handler from the control tree.
         */
        void handleEvent(Event *event);

        /** 
         * @brief Will print some values useful for debugging
         */
        void debugPrint();

        virtual void connectedEvent();

        virtual void disconnectedEvent();

        virtual void newValueEvent();

    public slots:
        /**
        * @brief Change the value of the control
        */
        void setValue(int value);

        /**
        * @brief Convenience function for checkboxes. Will just
        * call setValue(int)
        */
        void setValue(bool value);

        /**
         * @brief Tell the control in the backend to start MIDI learning.
         * TODO: make it show in the gui what its doing
         */
        void MIDILearn();

        /** 
         * @brief Convenience function. Will call setValue(127). Used for trigger-controls/buttons
         */
        void trigger();

        /**
         * @brief Find the absolute control id for this widget, if there is one
         *
         * @param widget the widget to start recursion from
         *
         * @return the absolute control id of the widget, or an empty string if there was no id
         * found
         */
        static QString findComponentPath(QObject *object);

        /** 
         * @brief Revert the control to its defaults
         */
        void defaults();

        /**
         * @brief Recurse up the parent chain and set control according to qt properties. These
         * properties are controlId and absoluteControlId, used throughout the ui files.
         * */
        void updateControlId();

    signals:
        /**
         * @brief This signal is emitted from the controlhelper when the value of the controls value
         * has changed. should be connected to a slot that changes the value in the gui.
         */
        void valueChanged(int value);

        /**
         * @brief For controls that have a set of string describing the values (options), this
         * signal will be emitted with a list of them when the ControlHelper.has connected to a new
         * control
         */
        void optionsChanged(QStringList options);

        /** 
         * @brief The ControlHelper.has been connected to a node in the tree.
         * 
         * @param control The control the helper has been connected to
         */
        void connected(GenControl *control);

        /** 
         * @brief The ControlHelper.has been disconnected from the node in the tree it was
         * previously connected to
         */
        void disconnected();

    protected:
        GenControl *m_control;

    private:
        void emitOptions();

        void disconnect();

};

#endif /* ifndef _CONTROLHELPER_H_ */

