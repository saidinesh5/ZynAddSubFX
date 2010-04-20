/*
  ZynAddSubFX - a software synthesizer

  EnvelopeParams.h - Parameters for Envelope
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

#ifndef ENVELOPE_PARAMS_H
#define ENVELOPE_PARAMS_H

#include "../globals.h"
#include "../Misc/XMLwrapper.h"
#include "Presets.h"
#include "../Controls/Node.h"
#include "../Controls/DescRanger.h"
#include "../Controls/Toggle.h"

#define MAX_ENVELOPE_POINTS 40
#define MIN_ENVELOPE_DB -40

class EnvelopeParams:public Presets
{
    public:
        EnvelopeParams(Node *parent,
                       std::string id,
                       unsigned char Penvstretch_,
                       unsigned char Pforcedrelease_);
        virtual ~EnvelopeParams();
        void ADSRinit(char A_dt, char D_dt, char S_val, char R_dt);
        void ADSRinit_dB(char A_dt, char D_dt, char S_val, char R_dt);
        void ASRinit(char A_val, char A_dt, char R_val, char R_dt);
        void ADSRinit_filter(char A_val,
                             char A_dt,
                             char D_val,
                             char D_dt,
                             char R_dt,
                             char R_val);
        void ASRinit_bw(char A_val, char A_dt, char R_val, char R_dt);
        void converttofree();

        void add2XML(XMLwrapper *xml);
        void defaults();
        void getfromXML(XMLwrapper *xml);

        REALTYPE getdt(char i);

        /* MIDI Parameters */
        Toggle freemode; //1 daca este in modul free sau 0 daca este in mod ADSR,ASR,...
        DescRanger envpoints;
        DescRanger envsustain; //127 pentru dezactivat

        Node envdtNode;
        DescRanger* envdt[MAX_ENVELOPE_POINTS];
        Node envvalNode;
        DescRanger* envval[MAX_ENVELOPE_POINTS];
        DescRanger envstretch; //64=normal stretch (piano-like), 0=no stretch
        Toggle forcedrelease; //0 - OFF, 1 - ON
        Toggle linearenvelope; //if the amplitude envelope is linear

        DescRanger A_dt, D_dt, R_dt, A_val, D_val, S_val, R_val;

        int Envmode; // 1 for ADSR parameters (linear amplitude)
        // 2 for ADSR_dB parameters (dB amplitude)
        // 3 for ASR parameters (frequency LFO)
        // 4 for ADSR_filter parameters (filter parameters)
        // 5 for ASR_bw parameters (bandwidth parameters)

    private:
        void store2defaults();

        /* Default parameters */
        //unsigned char Denvstretch;
        //unsigned char Dforcedrelease;
        //unsigned char Dlinearenvelope;
        //unsigned char DA_dt, DD_dt, DR_dt,
                      //DA_val, DD_val, DS_val, DR_val;
};

#endif

