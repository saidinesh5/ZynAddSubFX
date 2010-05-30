/*
  ZynAddSubFX - a software synthesizer

  GenControl.h - Control base class
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
#ifndef _GEN_CONTROL_H
#define _GEN_CONTROL_H

#include <pthread.h>
#include <string>
#include "Node.h"


/**A control for a parameter within the program*/
class GenControl:public Node
{
    public:
        virtual int getInt() const            = 0;
        virtual void setInt(int val)          = 0;
        virtual std::string getString() const = 0;
        virtual void defaults()               = 0;

        bool MIDILearn();

        virtual void queueGetInt()           = 0;
        virtual void queueSetInt(int value)  = 0;

        virtual int numOptions() const {return 0;}
        virtual std::string getOption(int /*i*/) const {return "";}

        virtual void addpar(class XMLwrapper *, const std::string&) {};
        virtual void getpar(class XMLwrapper *, const std::string&) {};
    protected:
        GenControl(Node *parent, std::string id);
        virtual ~GenControl();
        mutable pthread_mutex_t localMute;

        void lock() const;
        void unlock() const;


    private:

        int midichan, miditype;

        //std::string m_description;
};

#endif

