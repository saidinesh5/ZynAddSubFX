/*
  ZynAddSubFX - a software synthesizer

  LFOParams.h - Parameters for LFO
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

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

#ifndef LFO_PARAMS_H
#define LFO_PARAMS_H

#include "../Misc/XMLwrapper.h"
#include "Presets.h"
#include "../Controls/Toggle.h"
#include "../Controls/Ranger.h"
#include "../Controls/DescRanger.h"
#include "../Controls/Selector.h"

class LFOParams:public Presets
{
    public:
        LFOParams(Node *parent,
                  std::string id,
                  char Pfreq_,
                  char Pintensity_,
                  char Pstartphase_,
                  char PLFOtype_,
                  char Prandomness_,
                  char Pdelay_,
                  char Pcontinous,
                  char fel_);
        virtual ~LFOParams();

        void add2XML(XMLwrapper *xml);
        void defaults();
        /**Loads the LFO from the xml*/
        void getfromXML(XMLwrapper *xml);

        /*  MIDI Parameters*/
        Ranger     freq; /**<frequency*/
        DescRanger intensity; /**<intensity*/
        DescRanger startphase; /**<start phase (0=random)*/
        Selector LFOtype; /**<LFO type (sin,triangle,square,ramp,...)*/
        DescRanger randomness; /**<randomness (0=off)*/
        DescRanger freqrand; /**<frequency randomness (0=off)*/
        DescRanger delay; /**<delay (0=off)*/
        Toggle continous; /**<1 if LFO is continous*/
        DescRanger stretch; /**<how the LFO is "stretched" according the note frequency (64=no stretch)*/

        int fel; //what kind is the LFO (0 - frequency, 1 - amplitude, 2 - filter)
        static int time; //is used by Pcontinous parameter
};

#endif

