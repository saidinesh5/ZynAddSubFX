/*
  ZynAddSubFX - a software synthesizer

  Control.cpp - Control template
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

template <class T>
Control<T>::Control(Node *parent, std::string id, T defaultval, const InjFunction<char,T> *nfunc,enum controlType type)
    :GenControl(parent,id,type),value(defaultval),defaultval(defaultval),func(nfunc)
{
    for (char c = 0; c < 127; ++c) {
        T v = (*func)(c);
        char backval = char((*func)(v));
        if (backval != c) {
            std::cerr << " Warning (Control.cpp:" << __LINE__ << "): (" << int(c) << " != " << int(backval) << ") function is not reflective\n";
            break;
        }
    }
}

template <class T>
Control<T>::~Control()
{
    delete func;
}

template <class T>
void Control<T>::handleSyncEvent(Event *ev)
{
    //this is when the control value is to be changed to
    //something else
    if (ev->type() == Event::ChangeEvent) {
        char charval = static_cast<ChangeEvent*>(ev)->getVal();
        value = (*func)(charval);
        std::cout << getAbsoluteId() << "Received charval: " << int(charval) << " and new value " << value << "\n";
        forward(new NewValueEvent(charval));

        //and this is for reading the value of the control
    } else if (ev->type() == Event::RequestValueEvent) {
        forward(new NewValueEvent((*func)(value)));
    }
}

template <class T>
void Control<T>::setValue(const T &val)
{
    value = val;
    forward(new NewValueEvent((*func)(value)));
}

template <class T>
void Control<T>::setValue(char val)
{
    Job::push(new NodeJob(*this, new ChangeEvent(val)));
}

template <class T>
void Control<T>::requestValue()
{
    Job::push(new NodeJob(*this, new RequestValueEvent()));
}

template <class T>
void Control<T>::resetDefault()
{
    setValue(defaultval);
}

class MidiLearnNode : public Node
{
    public:

        int eventcount, learnedchan, learnedtype;
        pthread_mutex_t midilearningmutex;
        pthread_cond_t midiLearned;
        GenControl *parent;
        bool isLearning;

        MidiLearnNode(GenControl *parentControl)
            : Node(NULL, "Midilearn"),
            parent(parentControl),
            isLearning(true)
        {
            //init midi learn
            pthread_mutex_init(&midilearningmutex,NULL);
            pthread_cond_init(&midiLearned, NULL);

        }

        ~MidiLearnNode()
        {
            pthread_mutex_destroy(&midilearningmutex);
            pthread_cond_destroy(&midiLearned);
        }

        void handleEvent(Event *event)
        {
            MidiEvent *mid = static_cast<MidiEvent*>(event);
            if (!mid) return;

            if (isLearning) {

                pthread_mutex_lock(&midilearningmutex);

                if (learnedchan == mid->chan && learnedtype == mid->type) {
                    eventcount++;
                } else {
                    eventcount = 0;
                    learnedchan = mid->chan;
                    learnedtype = mid->type;
                }
                if (eventcount > 5) {
                    pthread_cond_broadcast(&midiLearned);
                    isLearning = false;
                }
                pthread_mutex_unlock(&midilearningmutex);
            } else {

                if (learnedchan == mid->chan && learnedtype == mid->type) {
                    parent->setValue(char(mid->par));
                }

            }

        }

        void waitForLearned()
        {
            pthread_mutex_lock(&midilearningmutex);
            learnedchan = 0;
            learnedtype = 0;
            eventcount = 0;

            pthread_cond_wait(&midiLearned, &midilearningmutex);
            pthread_mutex_unlock(&midilearningmutex);
        }

};

template <class T>
bool Control<T>::MIDILearn()
{
    MidiLearnNode *learnNode = new MidiLearnNode(this);
    getRoot()->addRedirection(learnNode, new TypeFilter(Event::MidiEvent));

    std::cout << "now learning..\n";
    learnNode->waitForLearned();
    std::cout << "Learned!! " << learnNode->learnedtype << "\n";
    //getRoot()->removeRedirections(learnNode);
    //delete learnNode;


}
