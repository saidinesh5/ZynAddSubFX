/*
    MusicIO.h

    Copyright 2009, Alan Calvert
    Copyright 2009, James Morris

    This file is part of yoshimi, which is free software: you can
    redistribute it and/or modify it under the terms of the GNU General
    Public License as published by the Free Software Foundation, either
    version 3 of the License, or (at your option) any later version.

    yoshimi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with yoshimi.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUDIO_OUT_H
#define AUDIO_OUT_H

#include "../Misc/Stereo.h"
#include "../Samples/Sample.h"
#include <queue>
#include <pthread.h>
#include "OutMgr.h"
#include "../Misc/Atomic.h"

class AudioOut
{
    public:
        AudioOut(OutMgr *out);
        virtual ~AudioOut() {};

        /**Start the Driver*/
        virtual bool Start()=0;
        /**Stop the Driver*/
        virtual void Stop()=0;

        /**Give the Driver Samples to process*/
        virtual void out(const Stereo<Sample> smps);
        /**Determines if new operator should/can be used*/
        virtual bool isEnabled() const {return enabled();};

        /**Report the state of the engine
         * @return 0 for stoped, 1 for running*/
        virtual int state() const {return enabled();};

    protected:
        /**Get the next sample for output.*/
        virtual const Stereo<Sample> getNext();

        std::queue<Stereo<Sample> >  outBuf;
        const Sample *  curSmp;
        Stereo<Sample> current;
        pthread_mutex_t outBuf_mutex;
        pthread_cond_t  outBuf_cv;

        OutMgr *manager;
        //thread resources
        Atomic<bool> threadStop;
        pthread_t pThread;
        Atomic<bool> enabled;
};

#endif
