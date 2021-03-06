/*
  ZynAddSubFX - a software synthesizer

  LFO.cpp - LFO implementation
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LFO.h"


LFO::LFO(LFOParams *lfopars, REALTYPE basefreq)
{
    if(lfopars->stretch() == 0)
        lfopars->stretch.setValue(1);
    REALTYPE lfostretch = pow(basefreq / 440.0,
                              (lfopars->stretch() - 64.0) / 63.0);         //max 2x/octave

    REALTYPE lfofreq    =
        (pow(2, lfopars->freq() * 10.0) - 1.0) / 12.0 * lfostretch;
    incx = fabs(lfofreq) * (REALTYPE)SOUND_BUFFER_SIZE / (REALTYPE)SAMPLE_RATE;

    if(lfopars->continous() == 0) {
        if(lfopars->startphase() == 0)
            x = RND;
        else
            x = fmod((lfopars->startphase() - 64.0) / 127.0 + 1.0, 1.0);
    }
    else {
        REALTYPE tmp = fmod(lfopars->time * incx, 1.0);
        x = fmod((lfopars->startphase() - 64.0) / 127.0 + 1.0 + tmp, 1.0);
    }

    //Limit the Frequency(or else...)
    if(incx > 0.49999999)
        incx = 0.499999999;


    lfornd = lfopars->randomness() / 127.0;
    if(lfornd < 0.0)
        lfornd = 0.0;
    else
    if(lfornd > 1.0)
        lfornd = 1.0;

//    lfofreqrnd=pow(lfopars->Pfreqrand/127.0,2.0)*2.0*4.0;
    lfofreqrnd = pow(lfopars->freqrand() / 127.0, 2.0) * 4.0;

    switch(lfopars->fel) {
    case 1:
        lfointensity = lfopars->intensity() / 127.0;
        break;
    case 2:
        lfointensity = lfopars->intensity() / 127.0 * 4.0;
        break; //in octave
    default:
        lfointensity = pow(2, lfopars->intensity() / 127.0 * 11.0) - 1.0; //in centi
        x -= 0.25; //chance the starting phase
        break;
    }

    amp1     = (1 - lfornd) + lfornd * RND;
    amp2     = (1 - lfornd) + lfornd * RND;
    lfotype  = lfopars->LFOtype();
    lfodelay = lfopars->delay() / 127.0 * 4.0; //0..4 sec
    incrnd   = nextincrnd = 1.0;
    freqrndenabled = (lfopars->freqrand() != 0);
    computenextincrnd();
    computenextincrnd(); //twice because I want incrnd & nextincrnd to be random
}

LFO::~LFO()
{}

/*
 * LFO out
 */
REALTYPE LFO::lfoout()
{
    REALTYPE out;
    switch(lfotype) {
    case 1: //LFO_TRIANGLE
        if((x >= 0.0) && (x < 0.25))
            out = 4.0 * x;
        else
        if((x > 0.25) && (x < 0.75))
            out = 2 - 4 * x;
        else
            out = 4.0 * x - 4.0;
        break;
    case 2: //LFO_SQUARE
        if(x < 0.5)
            out = -1;
        else
            out = 1;
        break;
    case 3: //LFO_RAMPUP
        out = (x - 0.5) * 2.0;
        break;
    case 4: //LFO_RAMPDOWN
        out = (0.5 - x) * 2.0;
        break;
    case 5: //LFO_EXP_DOWN 1
        out = pow(0.05, x) * 2.0 - 1.0;
        break;
    case 6: //LFO_EXP_DOWN 2
        out = pow(0.001, x) * 2.0 - 1.0;
        break;
    default:
        out = cos(x * 2.0 * PI); //LFO_SINE
    }


    if((lfotype == 0) || (lfotype == 1))
        out *= lfointensity * (amp1 + x * (amp2 - amp1));
    else
        out *= lfointensity * amp2;
    if(lfodelay < 0.00001) {
        if(freqrndenabled == 0)
            x += incx;
        else {
            float tmp = (incrnd * (1.0 - x) + nextincrnd * x);
            if(tmp > 1.0)
                tmp = 1.0;
            else
            if(tmp < 0.0)
                tmp = 0.0;
            x += incx * tmp;
        }
        if(x >= 1) {
            x    = fmod(x, 1.0);
            amp1 = amp2;
            amp2 = (1 - lfornd) + lfornd * RND;

            computenextincrnd();
        }
    }
    else
        lfodelay -= (REALTYPE)SOUND_BUFFER_SIZE / (REALTYPE)SAMPLE_RATE;
    return out;
}

/*
 * LFO out (for amplitude)
 */
REALTYPE LFO::amplfoout()
{
    REALTYPE out;
    out = 1.0 - lfointensity + lfoout();
    if(out < -1.0)
        out = -1.0;
    else
    if(out > 1.0)
        out = 1.0;
    return out;
}


void LFO::computenextincrnd()
{
    if(freqrndenabled == 0)
        return;
    incrnd     = nextincrnd;
    nextincrnd = pow(0.5, lfofreqrnd) + RND * (pow(2.0, lfofreqrnd) - 1.0);
}

