/*
  ZynAddSubFX - a software synthesizer

  VoiceList.cpp - Voice list dialog

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

#include "VoiceList.h"
#include "CollapsableFrame.h"
#include "Oscil.h"
#include <QtDebug>
#include "ControlHelper.h"

VoiceList::VoiceList(QString absoluteId, QWidget *parent)
    :QDialog(parent)
{
    setProperty("absoluteControlId", absoluteId);

    setupUi(this);

    Node *node   = Node::get(absoluteId.toStdString());

    partBar->setControlsWidget(voiceWidget);
    partBar->setNode(node);
}

void VoiceList::on_buttonOscillator_clicked()
{
    Oscil *oscil = new Oscil(NULL);
    oscil->setProperty("absoluteControlId",
            partBar->getCurrentChild() + ".OSCIL");
    oscil->show();
}

#include "VoiceList.moc"
// vim: sw=4 sts=4 et tw=100

