/*
  ZynAddSubFX - a software synthesizer

  EnvelopeParams.C - Parameters for Envelope
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

#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include "EnvelopeParams.h"
#include <sstream>
#include <iomanip>

EnvelopeParams::EnvelopeParams(Node *parent,
                               std::string id,
                               unsigned char Penvstretch_,
                               unsigned char Pforcedrelease_) :
                               Presets(parent, id),
                               freemode(this, "free_mode", 1),
                               envpoints(this, "EnvPoints", 1),
                               envsustain(this, "env_sustain", 1),
                               envdtNode(this, "EnvDt"),
                               envvalNode(this, "EnvVal"),
                               envstretch(this, "env_stretch", Penvstretch_),
                               forcedrelease(this, "forced_release", Pforcedrelease_),
                               linearenvelope(this, "LinearEnvelope", 0),

                               A_dt(this, "A_dt", 10),
                               D_dt(this, "D_dt", 10),
                               R_dt(this, "R_dt", 10),
                               A_val(this, "A_val", 64),
                               D_val(this, "D_val", 64),
                               S_val(this, "Sustain", 64),
                               R_val(this, "R_val", 64)
{
    for(int i = 0; i < MAX_ENVELOPE_POINTS; i++) {
        std::stringstream s;
        s << std::setfill('0') << std::setw(2) << i;
        envdt[i]  = new DescRanger(&envdtNode, s.str(), 32);
        envval[i] = new DescRanger(&envvalNode, s.str(), 64);
    }

    envdt[0]->setValue(0); //not used
    Envmode         = 1;

    store2defaults();
}

EnvelopeParams::~EnvelopeParams()
{
    for(int i = 0; i < MAX_ENVELOPE_POINTS; i++) {
        delete envdt[i];
        delete envval[i];
    }
}

REALTYPE EnvelopeParams::getdt(char i)
{
    REALTYPE result = (pow(2.0, envdt[(int)i]->getValue() / 127.0 * 12.0) - 1.0) * 10.0; //miliseconds
    return result;
}


/*
 * ADSR/ASR... initialisations
 */
void EnvelopeParams::ADSRinit(char A_dt, char D_dt, char S_val, char R_dt)
{
    setpresettype("Penvamplitude");
    Envmode   = 1;

    this->A_dt.setValue(A_dt);
    this->D_dt.setValue(D_dt);
    this->S_val.setValue(S_val);
    this->R_dt.setValue(R_dt);

    freemode.setValue(0);
    converttofree();

    store2defaults();
}

void EnvelopeParams::ADSRinit_dB(char A_dt, char D_dt, char S_val, char R_dt)
{
    setpresettype("Penvamplitude");
    Envmode   = 2;

    this->A_dt.setValue(A_dt);
    this->D_dt.setValue(D_dt);
    this->S_val.setValue(S_val);
    this->R_dt.setValue(R_dt);

    freemode.setValue(0);
    converttofree();

    store2defaults();
}

void EnvelopeParams::ASRinit(char A_val, char A_dt, char R_val, char R_dt)
{
    setpresettype("Penvfrequency");
    Envmode   = 3;

    this->A_val.setValue(A_val);
    this->A_dt.setValue(A_dt);
    this->R_val.setValue(R_val);
    this->R_dt.setValue(R_dt);

    freemode.setValue(0);
    converttofree();

    store2defaults();
}

void EnvelopeParams::ADSRinit_filter(char A_val,
                                     char A_dt,
                                     char D_val,
                                     char D_dt,
                                     char R_dt,
                                     char R_val)
{
    setpresettype("Penvfilter");
    Envmode   = 4;

    this->A_val.setValue(A_val);
    this->A_dt.setValue(A_dt);
    this->D_val.setValue(D_val);
    this->D_dt.setValue(D_dt);
    this->R_dt.setValue(R_dt);
    this->R_val.setValue(R_val);

    freemode.setValue(0);
    converttofree();
    store2defaults();
}

void EnvelopeParams::ASRinit_bw(char A_val, char A_dt, char R_val, char R_dt)
{
    setpresettype("Penvbandwidth");
    Envmode   = 5;

    this->A_val.setValue(A_val);
    this->A_dt.setValue(A_dt);
    this->R_val.setValue(R_val);
    this->R_dt.setValue(R_dt);

    freemode.setValue(0);
    converttofree();
    store2defaults();
}

/*
 * Convert the Envelope to freemode
 */
void EnvelopeParams::converttofree()
{
    switch(Envmode) {
    case 1:
        envpoints.setValue(4);
        envsustain.setValue(2);
        envval[0]->setValue(0);
        envdt[1]->setValue(A_dt());
        envval[1]->setValue(127);
        envdt[2]->setValue(D_dt());
        envval[2]->setValue(S_val());
        envdt[3]->setValue(R_dt());
        envval[3]->setValue(0);
        break;
    case 2:
        envpoints.setValue(4);
        envsustain.setValue(2);
        envval[0]->setValue(0);
        envdt[1]->setValue(A_dt());
        envval[1]->setValue(127);
        envdt[2]->setValue(D_dt());
        envval[2]->setValue(S_val());
        envdt[3]->setValue(R_dt());
        envval[3]->setValue(0);
        break;
    case 3:
        envpoints.setValue(3);
        envsustain.setValue(1);
        envval[0]->setValue(A_val());
        envdt[1]->setValue(A_dt());
        envval[1]->setValue(64);
        envdt[2]->setValue(R_dt());
        envval[2]->setValue(R_val());
        break;
    case 4:
        envpoints.setValue(4);
        envsustain.setValue(2);
        envval[0]->setValue(A_val());
        envdt[1]->setValue(A_dt());
        envval[1]->setValue(D_val());
        envdt[2]->setValue(D_dt());
        envval[2]->setValue(64);
        envdt[3]->setValue(R_dt());
        envval[3]->setValue(R_val());
        break;
    case 5:
        envpoints.setValue(3);
        envsustain.setValue(1);
        envval[0]->setValue(A_val());
        envdt[1]->setValue(A_dt());
        envval[1]->setValue(64);
        envdt[2]->setValue(R_dt());
        envval[2]->setValue(R_val());
        break;
    }
}




void EnvelopeParams::add2XML(XMLwrapper *xml)
{
    ADDPAR(freemode, "free_mode");
    ADDPAR(forcedrelease, "forced_release");
    ADDPAR(linearenvelope, "linear_envelope");
    ADDPAR(envpoints, "env_points");
    ADDPAR(envsustain, "env_sustain");
    ADDPAR(envstretch, "env_stretch");
    ADDPAR(A_dt, "A_dt");
    ADDPAR(D_dt, "D_dt");
    ADDPAR(R_dt, "R_dt");
    ADDPAR(A_val, "A_val");
    ADDPAR(D_val, "D_val");
    ADDPAR(S_val, "S_val");
    ADDPAR(R_val, "R_val");

    if((freemode() != 0) || (!xml->minimal)) {
        for(int i = 0; i < envpoints(); i++) {
            xml->beginbranch("POINT", i);
            if(i != 0)
                ADDPAR(*envdt[i], "dt");
            ADDPAR(*envval[i], "val");
            xml->endbranch();
        }
    }
}



void EnvelopeParams::getfromXML(XMLwrapper *xml)
{

    GETPAR(freemode, "free_mode");
    GETPAR(envpoints, "env_points");
    GETPAR(envsustain, "env_sustain");
    GETPAR(envstretch, "env_stretch");
    GETPAR(forcedrelease, "forced_release");
    GETPAR(linearenvelope, "linear_envelope");

    GETPAR(A_dt, "A_dt");
    GETPAR(D_dt, "D_dt");
    GETPAR(R_dt, "R_dt");
    GETPAR(A_val, "A_val");
    GETPAR(D_val, "D_val");
    GETPAR(S_val, "S_val");
    GETPAR(R_val, "R_val");

    for(int i = 0; i < envpoints(); i++) {
        if(xml->enterbranch("POINT", i) == 0)
            continue;
        if(i != 0)
            GETPAR(*envdt[i], "dt");
        GETPAR(*envval[i], "val");
        xml->exitbranch();
    }

    if(!freemode())
        converttofree();
}


void EnvelopeParams::defaults()
{
    envstretch.defaults();
    forcedrelease.defaults();
    linearenvelope.defaults();
    A_dt.defaults();
    D_dt.defaults();
    R_dt.defaults();
    A_val.defaults();
    D_val.defaults();
    S_val.defaults();
    R_val.defaults();

    freemode.setValue(0);
    converttofree();
}

void EnvelopeParams::store2defaults()
{
    envstretch.storeDefaults();
    forcedrelease.storeDefaults();
    linearenvelope.storeDefaults();
    A_dt.storeDefaults();
    D_dt.storeDefaults();
    R_dt.storeDefaults();
    A_val.storeDefaults();
    D_val.storeDefaults();
    S_val.storeDefaults();
    R_val.storeDefaults();
}

