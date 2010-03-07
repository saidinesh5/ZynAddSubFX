/*
  ZynAddSubFX - a software synthesizer

  LFOParams.C - Parameters for LFO
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

#include <math.h>
#include <stdio.h>
#include "../globals.h"
#include "LFOParams.h"

int LFOParams::time;

REALINJFUNCFUNC(LFOFreqInj,
               lfofreq2char, lfofreq2real,
               x * 127.0, x / 127.0);

LFOParams::LFOParams(Node *parent,
                     std::string id,
                     char Pfreq_,
                     char Pintensity_,
                     char Pstartphase_,
                     char PLFOtype_,
                     char Prandomness_,
                     char Pdelay_,
                     char Pcontinous_,
                     char fel_):
    Presets    (parent, id),
    freq       (this, "freq", lfofreq2real(Pfreq_), new LFOFreqInj),
    intensity  (this, "intensity", Pintensity_),
    startphase (this, "start_phase", Pstartphase_),
    LFOtype    (this, "lfo_type", PLFOtype_),
    randomness (this, "randomness_amplitude", Prandomness_),
    freqrand   (this, "randomness_frequency", 0),
    delay      (this, "delay", Pdelay_),
    continous  (this, "continous", Pcontinous_),
    stretch    (this, "stretch", 64)
{

    LFOtype.addOption("Sine"); //0
    LFOtype.addOption("Triangle"); //1
    LFOtype.addOption("Square"); //2
    LFOtype.addOption("Ramp up"); //3
    LFOtype.addOption("Ramp down"); //4
    LFOtype.addOption("Exp down 1"); //5
    LFOtype.addOption("Exp down 2"); //6

    switch(fel_) {
    case 0:
        setpresettype("Plfofrequency");
        break;
    case 1:
        setpresettype("Plfoamplitude");
        break;
    case 2:
        setpresettype("Plfofilter");
        break;
    }
    fel  = fel_;
    time = 0;

    defaults();
}

LFOParams::~LFOParams()
{}

void LFOParams::defaults()
{
    freq.defaults();
    intensity.defaults();
    startphase.defaults();
    LFOtype.defaults();
    randomness.defaults();
    delay.defaults();
    continous.defaults();
    freqrand.defaults();
    stretch.defaults();
}


void LFOParams::add2XML(XMLwrapper *xml)
{
    xml->addparreal("freq", freq());
    //see TODO in getFromXML below
    //ADDPAR(freq       , "freq");

    ADDPAR(intensity  , "intensity");
    ADDPAR(startphase , "start_phase");
    ADDPAR(LFOtype    , "lfo_type");
    ADDPAR(randomness , "randomness_amplitude");
    ADDPAR(freqrand   , "randomness_amplitude");
    ADDPAR(delay      , "delay");
    ADDPAR(continous  , "continous");
    ADDPAR(stretch    , "stretch");
}

void LFOParams::getfromXML(XMLwrapper *xml)
{
    freq.setValue(xml->getparreal("freq", freq(), 0.0, 1.0));
    //TODO: let the freq have an option that makes it write itself as a real and not a char, and
    //then uses the following form
    //GETPAR(freq       , "freq");

    GETPAR(intensity  , "intensity");
    GETPAR(startphase , "start_phase");
    GETPAR(LFOtype    , "lfo_type");
    GETPAR(randomness , "randomness_amplitude");
    GETPAR(freqrand   , "randomness_amplitude");
    GETPAR(delay      , "delay");
    GETPAR(continous  , "continous");
    GETPAR(stretch    , "stretch");
}

