/*
  ZynAddSubFX - a software synthesizer

  ADnoteParameters.C - Parameters for ADnote (ADsynth)
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

#include "ADnoteParameters.h"
#include "EnvelopeParams.h"
#include "LFOParams.h"
#include "../Misc/db2rapInjFunc.h"

int ADnote_unison_sizes[] =
{1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 25, 30, 40, 50, 0};

class VolumeConv:public InjFunction<char, REALTYPE>
{
    public:
        inline REALTYPE operator()(const char &x) const
        {return 4.0 * pow(0.1, 3.0 * (1.0 - x / 96.0)); }

        inline char operator()(const REALTYPE &x) const
        { return round(96 * (1 - (1 / (3 * log(0.1))) * (log(x) - log(4)))); }
};

class VoiceVolumeConv:public InjFunction<char, REALTYPE>
{
    public:
        inline REALTYPE operator()(const char &x) const
        {return pow(0.1, 3.0 * (1.0 - x / 127.0)); }

        inline char operator()(const REALTYPE &x) const
        { return round(127 * (1 - (1 / (3 * log(0.1))) * (log(x)))); }
};

ADnoteVoiceParam::ADnoteVoiceParam(Node *parent, std::string id, ADnoteParameters *par)
    :Node(parent, id),
      Enabled              (this, "enabled", false),
      type                 (this, "type", 0),
      delay                (this, "delay",0),
      resonance            (this, "Resonance",1),
      extoscil             (this, "ext_oscil",-1),
      extFMoscil           (this, "ext_fm_oscil",-1),
      oscilphase           (this, "oscil_phase",64),
      FMoscilphase         (this, "FMOscilPhase",64),
      filterBypass         (this, "filter_bypass",0),
      fixedFreq            (this, "fixed_freq",0),
      fixedFreqET          (this, "fixed_freq_et", 0),
      detuneSet            (this, "", 0, &par->globalDetuneSet),
      FMDetuneSet          (this, "FM", 0, &par->globalDetuneSet),
      freqEnvelopeEnabled  (this, "freq_envelope_enabled", 0),
      freqLfoEnabled       (this, "freq_lfo_enabled", 0),
      volume               (this, "volume", 0.23, new VoiceVolumeConv)
{}

ADnoteParameters::ADnoteParameters(Node *parent, FFTwrapper *fft_)
    :PresetsArray(parent, "ADnoteParameters"),
      volume               (this, "volume", 10, new VolumeConv),
      //note: the original conversion functon is found in ADnote.cpp:337
      globalDetuneSet      (this, "", 1),
      voices               (this, "Voices")
{
    setpresettype("Padsyth");
    fft = fft_;

    voices.addType("Voice");

    FreqEnvelope = new EnvelopeParams(this, "FREQUENCY_ENVELOPE", 0, 0);
    FreqEnvelope->ASRinit(64, 50, 64, 60);
    FreqLfo      = new LFOParams(70, 0, 64, 0, 0, 0, 0, 0);

    AmpEnvelope  = new EnvelopeParams(this, "AMPLITUDE_ENVELOPE", 64, 1);
    AmpEnvelope->ADSRinit_dB(0, 40, 127, 25);
    AmpLfo = new LFOParams(80, 0, 64, 0, 0, 0, 0, 1);

    GlobalFilter   = new FilterParams(this, 2, 94, 40);
    FilterEnvelope = new EnvelopeParams(this, "FILTER_ENVELOPE", 0, 1);
    FilterEnvelope->ADSRinit_filter(64, 40, 64, 70, 60, 64);
    FilterLfo      = new LFOParams(80, 0, 64, 0, 0, 0, 0, 2);
    Reson = new Resonance();

    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
        EnableVoice(nvoice, this);

    defaults();
}

void ADnoteParameters::defaults()
{
    //Default Parameters
    /* Frequency Global Parameters */
    PStereo = 1; //stereo
    FreqEnvelope->defaults();
    FreqLfo->defaults();
    PBandwidth = 64;

    /* Amplitude Global Parameters */
    //GlobalPar.PVolume=90;
    volume.defaults();

    PPanning = 64; //center
    PAmpVelocityScaleFunction = 64;
    AmpEnvelope->defaults();
    AmpLfo->defaults();
    PPunchStrength = 0;
    PPunchTime     = 60;
    PPunchStretch  = 64;
    PPunchVelocitySensing = 72;
    Hrandgrouping  = 0;

    /* Filter Global Parameters*/
    PFilterVelocityScale = 64;
    PFilterVelocityScaleFunction = 64;
    GlobalFilter->defaults();
    FilterEnvelope->defaults();
    FilterLfo->defaults();
    Reson->defaults();


    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
        defaults(nvoice);
    ;
    VoicePar[0]->Enabled = 1;
}

/*
 * Defaults a voice
 */
void ADnoteParameters::defaults(int n)
{
    int nvoice = n;
    VoicePar[nvoice]->Enabled     = 0;

    VoicePar[nvoice]->Unison_size = 1;
    VoicePar[nvoice]->Unison_frequency_spread = 60;
    VoicePar[nvoice]->Unison_stereo_spread    = 64;
    VoicePar[nvoice]->Unison_vibratto = 64;
    VoicePar[nvoice]->Unison_vibratto_speed   = 64;
    VoicePar[nvoice]->Unison_invert_phase     = 0;

    VoicePar[nvoice]->type.defaults();
    VoicePar[nvoice]->fixedFreq.defaults();
    VoicePar[nvoice]->fixedFreqET.defaults();
    VoicePar[nvoice]->resonance.defaults();
    VoicePar[nvoice]->filterBypass.defaults();
    VoicePar[nvoice]->extoscil.defaults();
    VoicePar[nvoice]->extFMoscil.defaults();
    VoicePar[nvoice]->oscilphase.defaults();
    VoicePar[nvoice]->FMoscilphase.defaults();
    VoicePar[nvoice]->delay.defaults();
    //VoicePar[nvoice]->PVolume=100;
    VoicePar[nvoice]->PVolumeminus    = 0;
    VoicePar[nvoice]->PPanning        = 64; //center
    VoicePar[nvoice]->detuneSet.defaults();
    VoicePar[nvoice]->freqLfoEnabled.defaults();
    VoicePar[nvoice]->freqEnvelopeEnabled.defaults();
    VoicePar[nvoice]->PAmpEnvelopeEnabled       = 0;
    VoicePar[nvoice]->PAmpLfoEnabled            = 0;
    VoicePar[nvoice]->PAmpVelocityScaleFunction = 127;
    VoicePar[nvoice]->PFilterEnabled            = 0;
    VoicePar[nvoice]->PFilterEnvelopeEnabled    = 0;
    VoicePar[nvoice]->PFilterLfoEnabled         = 0;
    VoicePar[nvoice]->PFMEnabled                = 0;

    //I use the internal oscillator (-1)
    VoicePar[nvoice]->PFMVoice  = -1;

    VoicePar[nvoice]->PFMVolume = 90;
    VoicePar[nvoice]->PFMVolumeDamp   = 64;

    VoicePar[nvoice]->PFMFreqEnvelopeEnabled   = 0;
    VoicePar[nvoice]->PFMAmpEnvelopeEnabled    = 0;
    VoicePar[nvoice]->PFMVelocityScaleFunction = 64;

    VoicePar[nvoice]->OscilSmp->defaults();
    VoicePar[nvoice]->FMSmp->defaults();

    VoicePar[nvoice]->AmpEnvelope->defaults();
    VoicePar[nvoice]->AmpLfo->defaults();

    VoicePar[nvoice]->FreqEnvelope->defaults();
    VoicePar[nvoice]->FreqLfo->defaults();

    VoicePar[nvoice]->VoiceFilter->defaults();
    VoicePar[nvoice]->FilterEnvelope->defaults();
    VoicePar[nvoice]->FilterLfo->defaults();

    VoicePar[nvoice]->FMFreqEnvelope->defaults();
    VoicePar[nvoice]->FMAmpEnvelope->defaults();
}


/*
 * Init the voice parameters
 */
void ADnoteParameters::EnableVoice(int nvoice, ADnoteParameters *par)
{
    ADnoteVoiceParam *param = new ADnoteVoiceParam(NULL, "VoiceParam", par);
    VoicePar.push_back(param);

    voices << VoicePar[nvoice];
    voices.createChild("Voice");

    VoicePar[nvoice]->OscilSmp = new OscilGen(fft, Reson,
                                              param, "OSCIL");
    VoicePar[nvoice]->FMSmp    = new OscilGen(fft, NULL,
                                              param, "FMSmp");

    VoicePar[nvoice]->AmpEnvelope    = new EnvelopeParams(param, "AMPLITUDE_ENVELOPE", 64, 1);
    VoicePar[nvoice]->AmpEnvelope->ADSRinit_dB(0, 100, 127, 100);
    VoicePar[nvoice]->AmpLfo         = new LFOParams(90, 32, 64, 0, 0, 30, 0, 1);

    VoicePar[nvoice]->FreqEnvelope   = new EnvelopeParams(param, "FREQUENCY_ENVELOPE", 0, 0);
    VoicePar[nvoice]->FreqEnvelope->ASRinit(30, 40, 64, 60);
    VoicePar[nvoice]->FreqLfo        = new LFOParams(50, 40, 0, 0, 0, 0, 0, 0);

    VoicePar[nvoice]->VoiceFilter    = new FilterParams(param, 2, 50, 60);
    VoicePar[nvoice]->FilterEnvelope = new EnvelopeParams(param, "FILTER_ENVELOPE", 0, 0);
    VoicePar[nvoice]->FilterEnvelope->ADSRinit_filter(90, 70, 40, 70, 10, 40);
    VoicePar[nvoice]->FilterLfo      = new LFOParams(50, 20, 64, 0, 0, 0, 0, 2);

    VoicePar[nvoice]->FMFreqEnvelope = new EnvelopeParams(param, "FMFreqEnvelope", 0, 0);
    VoicePar[nvoice]->FMFreqEnvelope->ASRinit(20, 90, 40, 80);
    VoicePar[nvoice]->FMAmpEnvelope  = new EnvelopeParams(param, "FMAmpEnvelope", 64, 1);
    VoicePar[nvoice]->FMAmpEnvelope->ADSRinit(80, 90, 127, 100);
}

/*
 * Get the Multiplier of the fine detunes of the voices
 */
REALTYPE ADnoteParameters::getBandwidthDetuneMultiplier()
{
    REALTYPE bw = (PBandwidth - 64.0) / 64.0;
    bw = pow(2.0, bw * pow(fabs(bw), 0.2) * 5.0);

    return bw;
}

/*
 * Get the unison spread in cents for a voice
 */

REALTYPE ADnoteParameters::getUnisonFrequencySpreadCents(int nvoice) {
    REALTYPE unison_spread = VoicePar[nvoice]->Unison_frequency_spread / 127.0;
    unison_spread = pow(unison_spread * 2.0, 2.0) * 50.0; //cents
    return unison_spread;
}

/*
 * Kill the voice
 */
void ADnoteParameters::KillVoice(int nvoice)
{
    delete (VoicePar[nvoice]->OscilSmp);
    delete (VoicePar[nvoice]->FMSmp);

    delete (VoicePar[nvoice]->AmpEnvelope);
    delete (VoicePar[nvoice]->AmpLfo);

    delete (VoicePar[nvoice]->FreqEnvelope);
    delete (VoicePar[nvoice]->FreqLfo);

    delete (VoicePar[nvoice]->VoiceFilter);
    delete (VoicePar[nvoice]->FilterEnvelope);
    delete (VoicePar[nvoice]->FilterLfo);

    delete (VoicePar[nvoice]->FMFreqEnvelope);
    delete (VoicePar[nvoice]->FMAmpEnvelope);

    delete VoicePar[nvoice];
}

ADnoteParameters::~ADnoteParameters()
{
    delete (FreqEnvelope);
    delete (FreqLfo);
    delete (AmpEnvelope);
    delete (AmpLfo);
    delete (GlobalFilter);
    delete (FilterEnvelope);
    delete (FilterLfo);
    delete (Reson);

    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
        KillVoice(nvoice);
    ;
}

int ADnoteParameters::get_unison_size_index(int nvoice) {
    int index  = 0;
    if(nvoice >= NUM_VOICES)
        return 0;
    int unison = VoicePar[nvoice]->Unison_size;

    while(1) {
        if(ADnote_unison_sizes[index] >= unison)
            return index;
        ;
        if(ADnote_unison_sizes[index] == 0)
            return index - 1;
        ;
        index++;
    }
    return 0;
}

void ADnoteParameters::set_unison_size_index(int nvoice, int index) {
    int unison = 1;
    for(int i = 0; i <= index; i++) {
        unison = ADnote_unison_sizes[i];
        if(unison == 0) {
            unison = ADnote_unison_sizes[i - 1];
            break;
        }
    }

    VoicePar[nvoice]->Unison_size = unison;
}



void ADnoteParameters::add2XMLsection(XMLwrapper *xml, int n)
{
    int nvoice = n;
    if(nvoice >= NUM_VOICES)
        return;

    int oscilused = 0, fmoscilused = 0; //if the oscil or fmoscil are used by another voice

    for(int i = 0; i < NUM_VOICES; i++) {
        if(VoicePar[i]->extoscil() == nvoice)
            oscilused = 1;
        if(VoicePar[i]->extFMoscil() == nvoice)
            fmoscilused = 1;
    }

    xml->addparbool("enabled", VoicePar[nvoice]->Enabled());
    if(((VoicePar[nvoice]->Enabled() == 0) && (oscilused == 0)
        && (fmoscilused == 0)) && (xml->minimal))
        return;


    xml->addpar("type", VoicePar[nvoice]->type());

    xml->addpar("unison_size", VoicePar[nvoice]->Unison_size);
    xml->addpar("unison_frequency_spread",
                VoicePar[nvoice]->Unison_frequency_spread);
    xml->addpar("unison_stereo_spread", VoicePar[nvoice]->Unison_stereo_spread);
    xml->addpar("unison_vibratto", VoicePar[nvoice]->Unison_vibratto);
    xml->addpar("unison_vibratto_speed",
                VoicePar[nvoice]->Unison_vibratto_speed);
    xml->addpar("unison_invert_phase", VoicePar[nvoice]->Unison_invert_phase);

    xml->addpar("delay", VoicePar[nvoice]->delay());
    xml->addparbool("resonance", VoicePar[nvoice]->resonance());

    xml->addpar("ext_oscil", VoicePar[nvoice]->extoscil());
    xml->addpar("ext_fm_oscil", VoicePar[nvoice]->extFMoscil());

    xml->addpar("oscil_phase", VoicePar[nvoice]->oscilphase());
    xml->addpar("oscil_fm_phase", VoicePar[nvoice]->FMoscilphase());

    xml->addparbool("filter_enabled", VoicePar[nvoice]->PFilterEnabled);
    xml->addparbool("filter_bypass", VoicePar[nvoice]->filterBypass());

    xml->addpar("fm_enabled", VoicePar[nvoice]->PFMEnabled);

    xml->beginbranch("OSCIL");
    VoicePar[nvoice]->OscilSmp->add2XML(xml);
    xml->endbranch();


    xml->beginbranch("AMPLITUDE_PARAMETERS");
    xml->addpar("panning", VoicePar[nvoice]->PPanning);
    xml->addpar("volume", VoicePar[nvoice]->volume.getChar());
    xml->addparbool("volume_minus", VoicePar[nvoice]->PVolumeminus);
    xml->addpar("velocity_sensing", VoicePar[nvoice]->PAmpVelocityScaleFunction);

    xml->addparbool("amp_envelope_enabled",
                    VoicePar[nvoice]->PAmpEnvelopeEnabled);
    if((VoicePar[nvoice]->PAmpEnvelopeEnabled != 0) || (!xml->minimal)) {
        xml->beginbranch("AMPLITUDE_ENVELOPE");
        VoicePar[nvoice]->AmpEnvelope->add2XML(xml);
        xml->endbranch();
    }
    xml->addparbool("amp_lfo_enabled", VoicePar[nvoice]->PAmpLfoEnabled);
    if((VoicePar[nvoice]->PAmpLfoEnabled != 0) || (!xml->minimal)) {
        xml->beginbranch("AMPLITUDE_LFO");
        VoicePar[nvoice]->AmpLfo->add2XML(xml);
        xml->endbranch();
    }
    xml->endbranch();

    xml->beginbranch("FREQUENCY_PARAMETERS");
    xml->addparbool("fixed_freq", VoicePar[nvoice]->fixedFreq());
    xml->addpar("fixed_freq_et", VoicePar[nvoice]->fixedFreqET());

    VoicePar[nvoice]->detuneSet.add2XMLsection(xml);

    xml->addparbool("freq_envelope_enabled",
                    VoicePar[nvoice]->freqEnvelopeEnabled());
    if((VoicePar[nvoice]->freqEnvelopeEnabled() != 0) || (!xml->minimal)) {
        xml->beginbranch("FREQUENCY_ENVELOPE");
        VoicePar[nvoice]->FreqEnvelope->add2XML(xml);
        xml->endbranch();
    }
    xml->addparbool("freq_lfo_enabled", VoicePar[nvoice]->freqLfoEnabled());
    if((VoicePar[nvoice]->freqLfoEnabled() != 0) || (!xml->minimal)) {
        xml->beginbranch("FREQUENCY_LFO");
        VoicePar[nvoice]->FreqLfo->add2XML(xml);
        xml->endbranch();
    }
    xml->endbranch();


    if((VoicePar[nvoice]->PFilterEnabled != 0) || (!xml->minimal)) {
        xml->beginbranch("FILTER_PARAMETERS");
        xml->beginbranch("FILTER");
        VoicePar[nvoice]->VoiceFilter->add2XML(xml);
        xml->endbranch();

        xml->addparbool("filter_envelope_enabled",
                        VoicePar[nvoice]->PFilterEnvelopeEnabled);
        if((VoicePar[nvoice]->PFilterEnvelopeEnabled != 0)
           || (!xml->minimal)) {
            xml->beginbranch("FILTER_ENVELOPE");
            VoicePar[nvoice]->FilterEnvelope->add2XML(xml);
            xml->endbranch();
        }

        xml->addparbool("filter_lfo_enabled",
                        VoicePar[nvoice]->PFilterLfoEnabled);
        if((VoicePar[nvoice]->PFilterLfoEnabled != 0) || (!xml->minimal)) {
            xml->beginbranch("FILTER_LFO");
            VoicePar[nvoice]->FilterLfo->add2XML(xml);
            xml->endbranch();
        }
        xml->endbranch();
    }

    if((VoicePar[nvoice]->PFMEnabled != 0) || (fmoscilused != 0)
       || (!xml->minimal)) {
        xml->beginbranch("FM_PARAMETERS");
        xml->addpar("input_voice", VoicePar[nvoice]->PFMVoice);

        xml->addpar("volume", VoicePar[nvoice]->PFMVolume);
        xml->addpar("volume_damp", VoicePar[nvoice]->PFMVolumeDamp);
        xml->addpar("velocity_sensing",
                    VoicePar[nvoice]->PFMVelocityScaleFunction);

        xml->addparbool("amp_envelope_enabled",
                        VoicePar[nvoice]->PFMAmpEnvelopeEnabled);
        if((VoicePar[nvoice]->PFMAmpEnvelopeEnabled != 0) || (!xml->minimal)) {
            xml->beginbranch("AMPLITUDE_ENVELOPE");
            VoicePar[nvoice]->FMAmpEnvelope->add2XML(xml);
            xml->endbranch();
        }
        xml->beginbranch("MODULATOR");

        VoicePar[nvoice]->FMDetuneSet.add2XMLsection(xml);

        xml->addparbool("freq_envelope_enabled",
                        VoicePar[nvoice]->PFMFreqEnvelopeEnabled);
        if((VoicePar[nvoice]->PFMFreqEnvelopeEnabled != 0)
           || (!xml->minimal)) {
            xml->beginbranch("FREQUENCY_ENVELOPE");
            VoicePar[nvoice]->FMFreqEnvelope->add2XML(xml);
            xml->endbranch();
        }

        xml->beginbranch("OSCIL");
        VoicePar[nvoice]->FMSmp->add2XML(xml);
        xml->endbranch();

        xml->endbranch();
        xml->endbranch();
    }
}


void ADnoteParameters::add2XML(XMLwrapper *xml)
{
    xml->addparbool("stereo", PStereo);

    xml->beginbranch("AMPLITUDE_PARAMETERS");
    //xml->addpar("volume",PVolume);
    xml->addpar("volume", volume.getChar());
    xml->addpar("panning", PPanning);
    xml->addpar("velocity_sensing", PAmpVelocityScaleFunction);
    xml->addpar("punch_strength", PPunchStrength);
    xml->addpar("punch_time", PPunchTime);
    xml->addpar("punch_stretch", PPunchStretch);
    xml->addpar("punch_velocity_sensing", PPunchVelocitySensing);
    xml->addpar("harmonic_randomness_grouping", Hrandgrouping);

    xml->beginbranch("AMPLITUDE_ENVELOPE");
    AmpEnvelope->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("AMPLITUDE_LFO");
    AmpLfo->add2XML(xml);
    xml->endbranch();
    xml->endbranch();

    xml->beginbranch("FREQUENCY_PARAMETERS");

    globalDetuneSet.add2XMLsection(xml);

    xml->addpar("bandwidth", PBandwidth);

    xml->beginbranch("FREQUENCY_ENVELOPE");
    FreqEnvelope->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("FREQUENCY_LFO");
    FreqLfo->add2XML(xml);
    xml->endbranch();
    xml->endbranch();


    xml->beginbranch("FILTER_PARAMETERS");
    xml->addpar("velocity_sensing_amplitude", PFilterVelocityScale);
    xml->addpar("velocity_sensing", PFilterVelocityScaleFunction);

    xml->beginbranch("FILTER");
    GlobalFilter->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("FILTER_ENVELOPE");
    FilterEnvelope->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("FILTER_LFO");
    FilterLfo->add2XML(xml);
    xml->endbranch();
    xml->endbranch();

    xml->beginbranch("RESONANCE");
    Reson->add2XML(xml);
    xml->endbranch();

    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++) {
        xml->beginbranch("VOICE", nvoice);
        add2XMLsection(xml, nvoice);
        xml->endbranch();
    }
}


void ADnoteParameters::getfromXML(XMLwrapper *xml)
{
    PStereo = xml->getparbool("stereo", PStereo);

    if(xml->enterbranch("AMPLITUDE_PARAMETERS")) {
        //PVolume=xml->getpar127("volume",PVolume);
        volume.setChar(char(xml->getpar127("volume", volume.getChar())));
        PPanning = xml->getpar127("panning", PPanning);
        PAmpVelocityScaleFunction = xml->getpar127(
            "velocity_sensing",
            
            PAmpVelocityScaleFunction);

        PPunchStrength = xml->getpar127("punch_strength",
                                                  PPunchStrength);
        PPunchTime     = xml->getpar127("punch_time",
                                                  PPunchTime);
        PPunchStretch  = xml->getpar127("punch_stretch",
                                                  PPunchStretch);
        PPunchVelocitySensing = xml->getpar127(
            "punch_velocity_sensing",
            PPunchVelocitySensing);
        Hrandgrouping  = xml->getpar127(
            "harmonic_randomness_grouping",
            Hrandgrouping);

        if(xml->enterbranch("AMPLITUDE_ENVELOPE")) {
            AmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        if(xml->enterbranch("AMPLITUDE_LFO")) {
            AmpLfo->getfromXML(xml);
            xml->exitbranch();
        }

        xml->exitbranch();
    }

    if(xml->enterbranch("FREQUENCY_PARAMETERS")) {

        globalDetuneSet.getFromXMLsection(xml);

        PBandwidth    = xml->getpar127("bandwidth",
                                                 PBandwidth);

        xml->enterbranch("FREQUENCY_ENVELOPE");
        FreqEnvelope->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FREQUENCY_LFO");
        FreqLfo->getfromXML(xml);
        xml->exitbranch();

        xml->exitbranch();
    }


    if(xml->enterbranch("FILTER_PARAMETERS")) {
        PFilterVelocityScale = xml->getpar127(
            "velocity_sensing_amplitude",
            PFilterVelocityScale);
        PFilterVelocityScaleFunction = xml->getpar127(
            "velocity_sensing",
            PFilterVelocityScaleFunction);

        xml->enterbranch("FILTER");
        GlobalFilter->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FILTER_ENVELOPE");
        FilterEnvelope->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FILTER_LFO");
        FilterLfo->getfromXML(xml);
        xml->exitbranch();
        xml->exitbranch();
    }

    if(xml->enterbranch("RESONANCE")) {
        Reson->getfromXML(xml);
        xml->exitbranch();
    }

    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++) {
        VoicePar[nvoice]->Enabled = 0;
        if(xml->enterbranch("VOICE", nvoice) == 0)
            continue;
        getfromXMLsection(xml, nvoice);
        xml->exitbranch();
    }
}

void ADnoteParameters::getfromXMLsection(XMLwrapper *xml, int n)
{
    int nvoice = n;
    if(nvoice >= NUM_VOICES)
        return;

    VoicePar[nvoice]->Enabled     = xml->getparbool("enabled", 0);

    VoicePar[nvoice]->Unison_size =
        xml->getpar127("unison_size", VoicePar[nvoice]->Unison_size);
    VoicePar[nvoice]->Unison_frequency_spread = xml->getpar127(
        "unison_frequency_spread",
        VoicePar[nvoice]->Unison_frequency_spread);
    VoicePar[nvoice]->Unison_stereo_spread    = xml->getpar127(
        "unison_stereo_spread",
        VoicePar[nvoice]->Unison_stereo_spread);
    VoicePar[nvoice]->Unison_vibratto = xml->getpar127(
        "unison_vibratto",
        VoicePar[nvoice]->
        Unison_vibratto);
    VoicePar[nvoice]->Unison_vibratto_speed   = xml->getpar127(
        "unison_vibratto_speed",
        VoicePar[nvoice]->Unison_vibratto_speed);
    VoicePar[nvoice]->Unison_invert_phase     = xml->getpar127(
        "unison_invert_phase",
        VoicePar[nvoice]->Unison_invert_phase);

    VoicePar[nvoice]->type.setValue(xml->getpar127("type",
                                            VoicePar[nvoice]->type()));
    VoicePar[nvoice]->delay.setValue(xml->getpar127("delay",
                                                  VoicePar[nvoice]->delay()));
    VoicePar[nvoice]->resonance.setValue(
        xml->getparbool("resonance", VoicePar[nvoice]->resonance()));

    VoicePar[nvoice]->extoscil.setValue(xml->getpar("ext_oscil",
                                                   -1,
                                                   -1,
                                                   nvoice - 1));
    VoicePar[nvoice]->extFMoscil.setValue(xml->getpar("ext_fm_oscil",
                                                   -1,
                                                   -1,
                                                   nvoice - 1));

    VoicePar[nvoice]->oscilphase.setValue(
        xml->getpar127("oscil_phase", VoicePar[nvoice]->oscilphase()));
    VoicePar[nvoice]->FMoscilphase.setValue(xml->getpar127(
        "oscil_fm_phase", VoicePar[nvoice]->FMoscilphase()));

    VoicePar[nvoice]->PFilterEnabled = xml->getparbool(
        "filter_enabled",
        VoicePar[nvoice]->
        PFilterEnabled);
    VoicePar[nvoice]->filterBypass.setValue(xml->getparbool(
        "filter_bypass",
        VoicePar[nvoice]->filterBypass()));

    VoicePar[nvoice]->PFMEnabled     =
        xml->getpar127("fm_enabled", VoicePar[nvoice]->PFMEnabled);

    if(xml->enterbranch("OSCIL")) {
        VoicePar[nvoice]->OscilSmp->getfromXML(xml);
        xml->exitbranch();
    }


    if(xml->enterbranch("AMPLITUDE_PARAMETERS")) {
        VoicePar[nvoice]->PPanning =
            xml->getpar127("panning", VoicePar[nvoice]->PPanning);
        VoicePar[nvoice]->volume.setChar(xml->getpar127("volume",
                                                             VoicePar[nvoice]->
                                                             volume.
                                                             getChar()));
        VoicePar[nvoice]->PVolumeminus = xml->getparbool(
            "volume_minus",
            VoicePar[nvoice]->
            PVolumeminus);
        VoicePar[nvoice]->PAmpVelocityScaleFunction = xml->getpar127(
            "velocity_sensing",
            VoicePar[nvoice]->PAmpVelocityScaleFunction);

        VoicePar[nvoice]->PAmpEnvelopeEnabled = xml->getparbool(
            "amp_envelope_enabled",
            VoicePar[nvoice]->PAmpEnvelopeEnabled);
        if(xml->enterbranch("AMPLITUDE_ENVELOPE")) {
            VoicePar[nvoice]->AmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice]->PAmpLfoEnabled = xml->getparbool(
            "amp_lfo_enabled",
            VoicePar[nvoice]->
            PAmpLfoEnabled);
        if(xml->enterbranch("AMPLITUDE_LFO")) {
            VoicePar[nvoice]->AmpLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if(xml->enterbranch("FREQUENCY_PARAMETERS")) {
        VoicePar[nvoice]->fixedFreq.setValue(xml->getparbool(
            "fixed_freq",
            VoicePar[nvoice]->fixedFreq()));
        VoicePar[nvoice]->fixedFreqET.setValue(xml->getpar127(
            "fixed_freq_et",
            VoicePar[nvoice]->fixedFreqET()));


        VoicePar[nvoice]->detuneSet.getFromXMLsection(xml);


        VoicePar[nvoice]->freqEnvelopeEnabled.setValue(xml->getparbool(
            "freq_envelope_enabled",
            VoicePar[nvoice]->freqEnvelopeEnabled()));
        if(xml->enterbranch("FREQUENCY_ENVELOPE")) {
            VoicePar[nvoice]->FreqEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice]->freqLfoEnabled.setValue(xml->getparbool(
            "freq_lfo_enabled",
            VoicePar[nvoice]->freqLfoEnabled()));
        if(xml->enterbranch("FREQUENCY_LFO")) {
            VoicePar[nvoice]->FreqLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if(xml->enterbranch("FILTER_PARAMETERS")) {
        if(xml->enterbranch("FILTER")) {
            VoicePar[nvoice]->VoiceFilter->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice]->PFilterEnvelopeEnabled = xml->getparbool(
            "filter_envelope_enabled",
            VoicePar[nvoice]->PFilterEnvelopeEnabled);
        if(xml->enterbranch("FILTER_ENVELOPE")) {
            VoicePar[nvoice]->FilterEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice]->PFilterLfoEnabled = xml->getparbool(
            "filter_lfo_enabled",
            VoicePar[nvoice]->PFilterLfoEnabled);
        if(xml->enterbranch("FILTER_LFO")) {
            VoicePar[nvoice]->FilterLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if(xml->enterbranch("FM_PARAMETERS")) {
        VoicePar[nvoice]->PFMVoice =
            xml->getpar("input_voice",
                        VoicePar[nvoice]->PFMVoice,
                        -1,
                        nvoice - 1);

        VoicePar[nvoice]->PFMVolume     =
            xml->getpar127("volume", VoicePar[nvoice]->PFMVolume);
        VoicePar[nvoice]->PFMVolumeDamp = xml->getpar127(
            "volume_damp",
            VoicePar[nvoice]->
            PFMVolumeDamp);
        VoicePar[nvoice]->PFMVelocityScaleFunction = xml->getpar127(
            "velocity_sensing",
            VoicePar[nvoice]->PFMVelocityScaleFunction);

        VoicePar[nvoice]->PFMAmpEnvelopeEnabled    = xml->getparbool(
            "amp_envelope_enabled",
            VoicePar[nvoice]->PFMAmpEnvelopeEnabled);
        if(xml->enterbranch("AMPLITUDE_ENVELOPE")) {
            VoicePar[nvoice]->FMAmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        if(xml->enterbranch("MODULATOR")) {

            VoicePar[nvoice]->FMDetuneSet.getFromXMLsection(xml);

            VoicePar[nvoice]->PFMFreqEnvelopeEnabled = xml->getparbool(
                "freq_envelope_enabled",
                VoicePar[nvoice]->PFMFreqEnvelopeEnabled);
            if(xml->enterbranch("FREQUENCY_ENVELOPE")) {
                VoicePar[nvoice]->FMFreqEnvelope->getfromXML(xml);
                xml->exitbranch();
            }

            if(xml->enterbranch("OSCIL")) {
                VoicePar[nvoice]->FMSmp->getfromXML(xml);
                xml->exitbranch();
            }

            xml->exitbranch();
        }
        xml->exitbranch();
    }
}

