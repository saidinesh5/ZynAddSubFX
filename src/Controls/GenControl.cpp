/*
  ZynAddSubFX - a software synthesizer

  GenControl.cpp - Control base class
  Copyright (C) 2009-2009 Mark McCurry
  Author: Mark McCurry

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
#include "GenControl.h"
#include "EventClasses.h"
#include "JobClasses.h"

GenControl::GenControl(Node *parent, std::string id)
    :Node(parent, id), midichan(0), miditype(0)
{
    pthread_mutex_init(&localMute, NULL);
}

GenControl::~GenControl() {
    pthread_mutex_destroy(&localMute);
}

class MidiLearnNode:public Node
{
    public:

        int eventcount, learnedchan, learnedtype;
        pthread_mutex_t midilearningmutex;
        pthread_cond_t  midiLearned;
        GenControl     *parent;
        bool isLearning;

        MidiLearnNode(GenControl *parentControl)
            :Node(NULL, "Midilearn"),
              parent(parentControl),
              isLearning(true)
        {
            //init midi learn
            pthread_mutex_init(&midilearningmutex, NULL);
            pthread_cond_init(&midiLearned, NULL);
        }

        ~MidiLearnNode()
        {
            pthread_mutex_destroy(&midilearningmutex);
            pthread_cond_destroy(&midiLearned);
        }

        void handleEvent(Event *event)
        {
            MidiEvent *mid = static_cast<MidiEvent *>(event);
            if(!mid)
                return;

            if(isLearning) {
                pthread_mutex_lock(&midilearningmutex);

                if((learnedchan == mid->chan) && (learnedtype == mid->type))
                    eventcount++;
                else {
                    eventcount  = 0;
                    learnedchan = mid->chan;
                    learnedtype = mid->type;
                }
                if(eventcount > 5) {
                    pthread_cond_broadcast(&midiLearned);
                    isLearning = false;
                }
                pthread_mutex_unlock(&midilearningmutex);
            }
            else
            if((learnedchan == mid->chan) && (learnedtype == mid->type))
                parent->setChar(mid->par);

        }

        void waitForLearned()
        {
            pthread_mutex_lock(&midilearningmutex);
            learnedchan = 0;
            learnedtype = 0;
            eventcount  = 0;

            pthread_cond_wait(&midiLearned, &midilearningmutex);
            pthread_mutex_unlock(&midilearningmutex);
        }
};

bool GenControl::MIDILearn()
{
    MidiLearnNode *learnNode = new MidiLearnNode(this);
    getRoot()->addRedirection(learnNode, new TypeFilter(Event::MidiEvent));

    std::cout << "now learning..\n";
    learnNode->waitForLearned();
    std::cout << "Learned!! " << learnNode->learnedtype << "\n";
    //getRoot()->removeRedirections(learnNode);
    //delete learnNode;
}


void GenControl::lock() const
{
    pthread_mutex_lock(&localMute);
}

void GenControl::unlock() const
{
    pthread_mutex_unlock(&localMute);
}

