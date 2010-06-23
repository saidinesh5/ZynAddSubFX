/*
  ZynAddSubFX - a software synthesizer

  ADnoteParameters.h - Parameters for ADnote (ADsynth)
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

#ifndef AD_NOTE_PARAMETERS_H
#define AD_NOTE_PARAMETERS_H


#include "../globals.h"
#include "FilterParams.h"
#include "../Synth/OscilGen.h"
#include "../Synth/Resonance.h"
#include "../Misc/Util.h"
#include "../Misc/XMLwrapper.h"
#include "../DSP/FFTwrapper.h"
#include "Presets.h"
#include "../Controls/Ranger.h"
#include "../Controls/Toggle.h"
#include "../Controls/FakeChildFactory.h"
#include "../Controls/DetuneControlSet.h"
#include <vector>
#include "PresetsArray.h"

class EnvelopeParams;
class LFOParams;

enum FMTYPE {
    NONE, MORPH, RING_MOD, PHASE_MOD, FREQ_MOD, PITCH_MOD
};
extern int ADnote_unison_sizes[];

/*****************************************************************/
/*                    GLOBAL PARAMETERS                          */
/*****************************************************************/


/***********************************************************/
/*                    VOICE PARAMETERS                     */
/***********************************************************/
class ADnoteVoiceParam:public Node
{
    public:

        ADnoteVoiceParam(Node *parent, std::string id, class ADnoteParameters *par);

        /** If the voice is enabled */
        Toggle Enabled;

        /** How many subvoices are used in this voice */
        unsigned char Unison_size;

        /** How subvoices are spread */
        unsigned char Unison_frequency_spread;

        /** Stereo spread of the subvoices*/
        unsigned char Unison_stereo_spread;

        /** Vibratto of the subvoices (which makes the unison more "natural")*/
        unsigned char Unison_vibratto;

        /** Medium speed of the vibratto of the subvoices*/
        unsigned char Unison_vibratto_speed;

        /** Unison invert phase */
        unsigned char Unison_invert_phase; //0=none,1=random,2=50%,3=33%,4=25%

        /** Type of the voice (0=Sound,1=Noise)*/
        DescRanger type;

        /** Voice Delay */
        DescRanger delay;

        /** If the resonance is enabled for this voice */
        DescRanger resonance;

        // What external oscil should I use, -1 for internal OscilSmp&FMSmp
        DescRanger extoscil, extFMoscil;
        // it is not allowed that the externoscil,externFMoscil => current voice

        // oscillator phases
        DescRanger oscilphase, FMoscilphase;

        // filter bypass
        DescRanger filterBypass;

        /** Voice oscillator */
        OscilGen *OscilSmp;

        /**********************************
        *     FREQUENCY PARAMETERS        *
        **********************************/

        /** If the base frequency is fixed to 440 Hz*/
        DescRanger fixedFreq;

        /* Equal temperate (this is used only if the Pfixedfreq is enabled)
           If this parameter is 0, the frequency is fixed (to 440 Hz);
           if this parameter is 64, 1 MIDI halftone -> 1 frequency halftone */
        DescRanger fixedFreqET;

        /* Detune controls */
        DetuneControlSet detuneSet;

        /* Frequency Envelope */
        Toggle   freqEnvelopeEnabled;
        EnvelopeParams *FreqEnvelope;

        /* Frequency LFO */
        Toggle freqLfoEnabled;
        LFOParams    *FreqLfo;


        /***************************
        *   AMPLITUDE PARAMETERS   *
        ***************************/

        /* Panning       0 - random
                 1 - left
                    64 - center
                   127 - right
           The Panning is ignored if the instrument is mono */
        DescRanger panning;

        /* Voice Volume */
        //unsigned char PVolume;
        Ranger volume;

        /* If the Volume negative */
        Toggle volumeMinus;

        /* Velocity sensing */
        unsigned char PAmpVelocityScaleFunction;

        /* Amplitude Envelope */
        unsigned char   PAmpEnvelopeEnabled;
        EnvelopeParams *AmpEnvelope;

        /* Amplitude LFO */
        unsigned char PAmpLfoEnabled;
        LFOParams    *AmpLfo;



        /*************************
        *   FILTER PARAMETERS    *
        *************************/

        /* Voice Filter */
        unsigned char PFilterEnabled;
        FilterParams *VoiceFilter;

        /* Filter Envelope */
        unsigned char   PFilterEnvelopeEnabled;
        EnvelopeParams *FilterEnvelope;

        /* LFO Envelope */
        unsigned char PFilterLfoEnabled;
        LFOParams    *FilterLfo;

        /****************************
        *   MODULLATOR PARAMETERS   *
        ****************************/

        /* Modullator Parameters (0=off,1=Morph,2=RM,3=PM,4=FM.. */
        unsigned char PFMEnabled;

        /* Voice that I use as modullator instead of FMSmp.
           It is -1 if I use FMSmp(default).
           It maynot be equal or bigger than current voice */
        short int PFMVoice;

        /* Modullator oscillator */
        OscilGen *FMSmp;

        /* Modullator Volume */
        unsigned char PFMVolume;

        /* Modullator damping at higher frequencies */
        unsigned char PFMVolumeDamp;

        /* Modullator Velocity Sensing */
        unsigned char PFMVelocityScaleFunction;

        /* Detune of the Modullator*/
        DetuneControlSet FMDetuneSet;

        /* Frequency Envelope of the Modullator */
        unsigned char   PFMFreqEnvelopeEnabled;
        EnvelopeParams *FMFreqEnvelope;

        /* Frequency Envelope of the Modullator */
        unsigned char   PFMAmpEnvelopeEnabled;
        EnvelopeParams *FMAmpEnvelope;
};

class ADnoteParameters : public PresetsArray
{
    public:
        ADnoteParameters(Node *parent, FFTwrapper *fft_);
        virtual ~ADnoteParameters();

        //ADnoteGlobalParam GlobalPar;
        //START PASTE GLOBALPAR FROM STRUCT

        /* The instrument type  - MONO/STEREO
           If the mode is MONO, the panning of voices are not used
           Stereo=1, Mono=0. */

        unsigned char PStereo;
        Ranger volume;




        /******************************************
         *     FREQUENCY GLOBAL PARAMETERS        *
         ******************************************/

        DetuneControlSet globalDetuneSet;

        unsigned char      PBandwidth; //how much the relative fine detunes of the voices are changed

        EnvelopeParams    *FreqEnvelope; //Frequency Envelope

        LFOParams *FreqLfo; //Frequency LFO

        /********************************************
         *     AMPLITUDE GLOBAL PARAMETERS          *
         ********************************************/

        /* Panning -  0 - random
           1 - left
           64 - center
           127 - right */
        DescRanger panning;

        //unsigned char PVolume;

        unsigned char   PAmpVelocityScaleFunction;

        EnvelopeParams *AmpEnvelope;

        LFOParams      *AmpLfo;

        unsigned char   PPunchStrength, PPunchTime, PPunchStretch,
                        PPunchVelocitySensing;

        /******************************************
         *        FILTER GLOBAL PARAMETERS        *
         ******************************************/
        FilterParams *GlobalFilter;

        // filter velocity sensing
        unsigned char PFilterVelocityScale;

        // filter velocity sensing
        unsigned char   PFilterVelocityScaleFunction;

        EnvelopeParams *FilterEnvelope;

        LFOParams      *FilterLfo;

        // RESONANCE
        Resonance *Reson;

        //how the randomness is applied to the harmonics on more voices using the same oscillator
        unsigned char Hrandgrouping;


        //END PASTE GLOBALPAR


        FakeChildFactory voices;
        std::vector<ADnoteVoiceParam *> VoicePar;

        void defaults();
        void add2XML(XMLwrapper *xml);
        void getfromXML(XMLwrapper *xml);

        REALTYPE getBandwidthDetuneMultiplier();
        REALTYPE getUnisonFrequencySpreadCents(int nvoice);
        int get_unison_size_index(int nvoice);
        void set_unison_size_index(int nvoice, int index);
    private:
        void defaults(int n); //n is the nvoice

        void EnableVoice(int nvoice, class ADnoteParameters *par);
        void KillVoice(int nvoice);
        FFTwrapper *fft;

        void add2XMLsection(XMLwrapper *xml, int n);
        void getfromXMLsection(XMLwrapper *xml, int n);
};

#endif

