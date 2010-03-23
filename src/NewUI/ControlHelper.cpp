/*
  ZynAddSubFX - a software synthesizer

  ControlHelper.cpp - The glue between the ZynAddSubFX backend and GUI

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

#include "ControlHelper.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QWidget>
#include <QDynamicPropertyChangeEvent>
#include <QStack>
#include <QVariant>
#include <QtDebug>
#include "Menu.h"

ControlHelper::ControlHelper(QObject *parent)
    :QObject(parent),
      m_control(NULL)
{
    //attach a menu to the parent object if it is a QWidget
    if (QWidget* widget = qobject_cast<QWidget*>(parent)) {
        new Menu(widget, this);
    }
}

ControlHelper::~ControlHelper()
{
    if (m_control)
        m_control->removeRedirections(this);
}

void ControlHelper::handleEvent(Event *event)
{
    //will be called from the engine/backend thread, so be careful (threadwise)
    //and efficient about all processing in this function

    if(event->type() == Event::NewValueEvent) {

        //sanity check
        if (static_cast<NewValueEvent *>(event)->control != m_control) {
            qDebug() << "ControlHelper: Weird, events from a control we don't know?";
            return;
        }
        newValueEvent();
    }
    else
    if(event->type() == Event::RemovalEvent) {
        //clear the current connected control
        disconnect();
        qDebug() << "Got removalevent, clearing...";
    }
    else
    if(event->type() == Event::OptionsChangedEvent)
        emitOptions();
}

void ControlHelper::connectedEvent()
{
    if (QWidget *widget = qobject_cast<QWidget*>(parent())) {
        widget->setToolTip(QString::fromStdString(m_control->getId()));
    }

    emit connected(m_control);
}

void ControlHelper::disconnectedEvent()
{
    emit disconnected();
}

void ControlHelper::newValueEvent()
{
    emit valueChanged(getValue());
}

void ControlHelper::setControl(QString absoluteId)
{
    if (m_control) m_control->removeRedirections(this);
    disconnect();

    if(absoluteId.isEmpty())
        return;

    Node::lock();
    Node *node = Node::get(absoluteId.toStdString());
    m_control = dynamic_cast<GenControl *>(node);
    if(m_control) {
        m_control->addRedirection(this);

        emitOptions();
        qDebug() << "Assigning" << this << "to" << absoluteId;
        connectedEvent();
        newValueEvent();
    } else {
        qDebug() << "Could not find a control named " << absoluteId;
    }

    Node::unlock();
}

void ControlHelper::disconnect()
{
    if(m_control) {
        m_control = NULL;
    }
    disconnectedEvent();
}

void ControlHelper::setValue(int value)
{
    if (Control<int>* intc = dynamic_cast<Control<int>*>(m_control)) {
        intc->setValue(value);
    }
    else if (m_control) {
        m_control->setChar(value);
    }
}

void ControlHelper::setValue(bool value)
{
    setValue(int(127 * value));
}

QString ControlHelper::getControlId()
{
    if(m_control)
        return QString::fromStdString(m_control->getAbsoluteId());
    else
        return "Undefined";
}

int ControlHelper::getValue()
{
    if (Control<int>* intc = dynamic_cast<Control<int>*>(m_control)) {
        return intc->getValue();
    }
    if(m_control) {
        return m_control->getChar();
    }

    qDebug() << "Warning: value for nonconnected control requested";
    return 0;
}

void ControlHelper::MIDILearn()
{
    if(m_control) {
        if (QMessageBox::information(qobject_cast<QWidget*>(parent()), QString(),
                "When you press OK, ZynAddSubFX listen for MIDI events. Once it has determined what"
                "controller should be used, it will be connected to this control.",
                QMessageBox::Ok | QMessageBox::Cancel) ==
                QMessageBox::Ok) {
            //TODO: handle result
            m_control->MIDILearn();
        }
    }
}

void ControlHelper::trigger()
{
    setValue(127);
}

void ControlHelper::updateControlId()
{
    QString fullid = findComponentPath(parent());
    if(fullid.isEmpty())
        return;
    setControl(fullid);
}

QString ControlHelper::findComponentPath(QObject *object)
{
    if (!object) return QString();

    QString fullid = object->property("absoluteControlId").toString();

    if(!fullid.isEmpty())
        //alright, we already have what we wanted
        return fullid;

    QObject *p = object;
    fullid = p->property("controlId").toString();
    if(fullid.isEmpty())
        //the object has no controlId set, so theres no reason to recurse any further for what its
        //full path would be.
        return QString();

    while(true) {
        p = p->parent();
        if(!p)
            //we've reached the end of the hierarchy without finding any absolute id. bail out
            return QString();

        QString id    = p->property("controlId").toString();
        QString absid = p->property("absoluteControlId").toString();

        if(!id.isEmpty())
            //just append the relative id and continue recursion
            fullid.prepend(id + ".");
        else
        if(!absid.isEmpty()) {
            //this appears to be the absolute id we've been looking for.
            fullid.prepend(absid + ".");
            return fullid;
        }
    }
    return QString();
}

void ControlHelper::emitOptions()
{
    QStringList options;
    if(m_control && m_control->numOptions())
        for(int i = 0; i < m_control->numOptions(); ++i)
            options << QString::fromStdString(m_control->getOption(i));
    //qDebug() << "Emitting options " << options;
    emit optionsChanged(options);
}

void ControlHelper::defaults()
{
    if (m_control) {
        m_control->defaults();
    }
}

void ControlHelper::debugPrint()
{
    qDebug() << "parent has controlId: " << parent()->property("controlId").toString() <<
        " and absoluteControlId: " << parent()->property("absoluteControlId").toString();
    if (m_control) {
        qDebug() << "Currently connected to " << QString::fromStdString(m_control->getAbsoluteId());
        qDebug() << "Current value is " << int(m_control->getChar());
    }
    else {
        qDebug() << "Currently not connected.";
    }
}

#include "ControlHelper.moc"

