/*
  ZynAddSubFX - a software synthesizer

  FilterParams.h - Parameters for filter
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

#ifndef FILTER_PARAMS_H
#define FILTER_PARAMS_H

#include "../globals.h"
#include "../Misc/XMLwrapper.h"
#include "Presets.h"
#include "../Controls/Ranger.h"
#include "../Controls/DescRanger.h"
#include "../Controls/Toggle.h"
#include "../Controls/Selector.h"
#include "PresetsArray.h"

class FilterParams : public PresetsArray
{
    public:
        FilterParams(Node *parent,
                     unsigned char Ptype_,
                     unsigned char Pfreq,
                     unsigned char Pq_);
        ~FilterParams();

        void add2XML(XMLwrapper *xml);
        void add2XMLsection(XMLwrapper *xml, int n);
        void defaults();
        void getfromXML(XMLwrapper *xml);
        void getfromXMLsection(XMLwrapper *xml, int n);


        void getfromFilterParams(FilterParams *pars);

        REALTYPE getfreq();
        REALTYPE getq();
        REALTYPE getfreqtracking(REALTYPE notefreq);
        REALTYPE getgain();

        Selector category; //Filter category (Analog/Formant/StVar)
        Selector type; // Filter type  (for analog lpf,hpf,bpf..)
        Ranger frequency; // Frequency (64-central frequency)
        DescRanger q; // Q parameters (resonance or bandwidth)
        DescRanger stages; //filter stages+1
        DescRanger freqtrack; //how the filter frequency is changing according the note frequency
        DescRanger gain; //filter's output gain

        //Formant filter parameters
        Node formantFilter;
        DescRanger numformants; //how many formants are used
        DescRanger formantslowness; //how slow varies the formants
        DescRanger vowelclearness; //how vowels are kept clean (how much try to avoid "mixed" vowels)
        DescRanger octavesfreq; //the center frequency of the res. func., and the number of octaves
        Ranger centerFrequency;

        struct {
            struct {
                unsigned char freq, amp, q; //frequency,amplitude,Q
            } formants[FF_MAX_FORMANTS];
        } Pvowels[FF_MAX_VOWELS];


        DescRanger sequencesize; //how many vowels are in the sequence
        DescRanger sequencestretch; //how the sequence is stretched (how the input from filter envelopes/LFOs/etc. is "stretched")
        Toggle sequencereversed; //if the input from filter envelopes/LFOs/etc. is reversed(negated)
        struct {
            unsigned char nvowel; //the vowel from the position
        } Psequence[FF_MAX_SEQUENCE];

        REALTYPE getcenterfreq();
        REALTYPE getoctavesfreq();
        REALTYPE getfreqpos(REALTYPE freq);
        REALTYPE getfreqx(REALTYPE x);

        void formantfilterH(int nvowel, int nfreqs, REALTYPE *freqs); //used by UI

        REALTYPE getformantfreq(unsigned char freq);
        REALTYPE getformantamp(unsigned char amp);
        REALTYPE getformantq(unsigned char q);

        bool changed;

    private:
        void defaults(int n);

        //stored default parameters
        unsigned char Dtype;
        unsigned char Dq;
};

#endif

