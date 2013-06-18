/*
  ZynAddSubFX - a software synthesizer

  Part.cpp - Part implementation
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

#include "Part.h"
#include "Microtonal.h"
#include "db2rapInjFunc.h"
#include "../Effects/EffectMgr.h"
#include "../Params/ADnoteParameters.h"
#include "../Params/SUBnoteParameters.h"
#include "../Params/PADnoteParameters.h"
#include "../Synth/ADnote.h"
#include "../Synth/SUBnote.h"
#include "../Synth/PADnote.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Misc/Bank.h"
#include <cmath>

using namespace std;

REALINJFUNCFUNC(panningFunc,
        pan2int, pan2real,
        x * 127.0,
        x / 127.0)

Part::Part(Node *parent,
           Microtonal *microtonal_,
           FFTwrapper *fft_,
           pthread_mutex_t *mutex_)
    :Node(parent, "Part"),
      instrument        (this, "INSTRUMENT"),
      instrumentKit     (&instrument, "INSTRUMENT_KIT"),
      tempInstrumentKitItem1 (&instrumentKit, "INSTRUMENT_KIT_ITEM"),
      instrumentControl (this),
      bankControl       (this),
      minKey            (this, "min_key", 0),
      maxKey            (this, "max_key", 127),
      keyShift          (this, "key_shift", 64),
      receiveChannel    (this, "ReceiveChannel", 0),
      panning           (this, "panning", pan2real(64), new panningFunc()),
      velSns            (this, "velocity_sensing", 64),
      velOffs           (this, "velocity_offset", 64),
      partVolume        (this, "volume", 30, new db2rapInjFunc<REALTYPE>(-40, 12.91666)),
      enabled           (this, "enabled", false)
{
    instrumentControl.addRedirection(this, new TypeFilter(Event::NewValueEvent));
    bankControl.addRedirection(this, new TypeFilter(Event::NewValueEvent));

    receiveChannel.addOption("Channel 1");
    receiveChannel.addOption("Channel 2");
    receiveChannel.addOption("Channel 3");
    receiveChannel.addOption("Channel 4");
    receiveChannel.addOption("Channel 5");
    receiveChannel.addOption("Channel 6");
    receiveChannel.addOption("Channel 7");
    receiveChannel.addOption("Channel 8");
    receiveChannel.addOption("Channel 9");
    receiveChannel.addOption("10(drums)");
    receiveChannel.addOption("Channel 11");
    receiveChannel.addOption("Channel 12");
    receiveChannel.addOption("Channel 13");
    receiveChannel.addOption("Channel 14");
    receiveChannel.addOption("Channel 15");
    receiveChannel.addOption("Channel 16");

    microtonal = microtonal_;
    fft = fft_;
    mutex      = mutex_;
    partoutl   = new REALTYPE [SOUND_BUFFER_SIZE];
    partoutr   = new REALTYPE [SOUND_BUFFER_SIZE];
    tmpoutl    = new REALTYPE [SOUND_BUFFER_SIZE];
    tmpoutr    = new REALTYPE [SOUND_BUFFER_SIZE];

    for(int n = 0; n < NUM_KIT_ITEMS; n++) {
        kit[n].Pname   = new unsigned char [PART_MAX_NAME_LEN];
        kit[n].adpars  = NULL;
        kit[n].subpars = NULL;
        kit[n].padpars = NULL;
    }

    kit[0].adpars  = new ADnoteParameters(&instrumentKit, fft);
    kit[0].subpars = new SUBnoteParameters();
    kit[0].padpars = new PADnoteParameters(fft, mutex);
//    ADPartParameters=kit[0].adpars;
//    SUBPartParameters=kit[0].subpars;

    //Part's Insertion Effects init
    for(int nefx = 0; nefx < NUM_PART_EFX; nefx++) {
        partefx[nefx]    = new EffectMgr(1, mutex);
        Pefxbypass[nefx] = false;
    }

    for(int n = 0; n < NUM_PART_EFX + 1; n++) {
        partfxinputl[n] = new REALTYPE [SOUND_BUFFER_SIZE];
        partfxinputr[n] = new REALTYPE [SOUND_BUFFER_SIZE];
    }

    killallnotes = 0;
    oldfreq      = -1.0;

    int i, j;
    for(i = 0; i < POLIPHONY; i++) {
        partnote[i].status = KEY_OFF;
        partnote[i].note   = -1;
        partnote[i].itemsplaying = 0;
        for(j = 0; j < NUM_KIT_ITEMS; j++) {
            partnote[i].kititem[j].adnote  = NULL;
            partnote[i].kititem[j].subnote = NULL;
            partnote[i].kititem[j].padnote = NULL;
        }
        partnote[i].time = 0;
    }
    cleanup();

    Pname      = new unsigned char [PART_MAX_NAME_LEN];

    oldvolumel = oldvolumer = 0.5;
    lastnote   = -1;
    lastpos    = 0; // lastpos will store previously used NoteOn(...)'s pos.
    lastlegatomodevalid = false; // To store previous legatomodevalid value.


    defaults();
}

void Part::defaults()
{
    enabled.defaults();
    minKey.defaults();
    maxKey.defaults();
    Pnoteon     = 1;
    Ppolymode   = 1;
    Plegatomode = 0;
    setPvolume(96);
    keyShift.defaults();
    receiveChannel.defaults();
    panning.defaults();
    velSns.defaults();
    velOffs.defaults();
    Pkeylimit   = 15;
    defaultsinstrument();
    ctl.defaults();
}

void Part::defaultsinstrument()
{
    ZERO(Pname, PART_MAX_NAME_LEN);

    info.Ptype = 0;
    ZERO(info.Pauthor, MAX_INFO_TEXT_SIZE + 1);
    ZERO(info.Pcomments, MAX_INFO_TEXT_SIZE + 1);

    Pkitmode  = 0;
    Pdrummode = 0;

    for(int n = 0; n < NUM_KIT_ITEMS; n++) {
        kit[n].Penabled    = 0;
        //kit[n].enabled.setValue(false);
        kit[n].Pmuted      = 0;
        kit[n].Pminkey     = 0;
        kit[n].Pmaxkey     = 127;
        kit[n].Padenabled  = 0;
        kit[n].Psubenabled = 0;
        kit[n].Ppadenabled = 0;
        ZERO(kit[n].Pname, PART_MAX_NAME_LEN);
        kit[n].Psendtoparteffect = 0;
        if(n != 0)
            setkititemstatus(n, 0);
    }
    kit[0].Penabled   = 1;
    //kit[0].enabled.setValue(true);
    kit[0].Padenabled = 1;
    kit[0].adpars->defaults();
    kit[0].subpars->defaults();
    kit[0].padpars->defaults();

    for(int nefx = 0; nefx < NUM_PART_EFX; nefx++) {
        partefx[nefx]->defaults();
        Pefxroute[nefx] = 0; //route to next effect
    }
}



/*
 * Cleanup the part
 */
void Part::cleanup(bool final)
{
    for(int k = 0; k < POLIPHONY; k++)
        KillNotePos(k);
    for(int i = 0; i < SOUND_BUFFER_SIZE; i++) {
        partoutl[i] = final ? 0.0 : denormalkillbuf[i];
        partoutr[i] = final ? 0.0 : denormalkillbuf[i];
        tmpoutl[i]  = 0.0;
        tmpoutr[i]  = 0.0;
    }
    ctl.resetall();
    for(int nefx = 0; nefx < NUM_PART_EFX; nefx++)
        partefx[nefx]->cleanup();
    for(int n = 0; n < NUM_PART_EFX + 1; n++) {
        for(int i = 0; i < SOUND_BUFFER_SIZE; i++) {
            partfxinputl[n][i] = final ? 0.0 : denormalkillbuf[i];
            partfxinputr[n][i] = final ? 0.0 : denormalkillbuf[i];
        }
    }
}

Part::~Part()
{
    cleanup(true);
    for(int n = 0; n < NUM_KIT_ITEMS; n++) {
        if(kit[n].adpars != NULL)
            delete (kit[n].adpars);
        if(kit[n].subpars != NULL)
            delete (kit[n].subpars);
        if(kit[n].padpars != NULL)
            delete (kit[n].padpars);
        kit[n].adpars  = NULL;
        kit[n].subpars = NULL;
        kit[n].padpars = NULL;
        delete [] kit[n].Pname;
    }

    delete [] Pname;
    delete [] partoutl;
    delete [] partoutr;
    delete [] tmpoutl;
    delete [] tmpoutr;
    for(int nefx = 0; nefx < NUM_PART_EFX; nefx++)
        delete (partefx[nefx]);
    for(int n = 0; n < NUM_PART_EFX + 1; n++) {
        delete [] partfxinputl[n];
        delete [] partfxinputr[n];
    }
}

/*
 * Note On Messages
 */
void Part::NoteOn(unsigned char note,
                  unsigned char velocity,
                  int masterkeyshift)
{
    int i, pos;

    // Legato and MonoMem used vars:
    int  posb = POLIPHONY - 1; // Just a dummy initial value.
    bool legatomodevalid = false; //true when legato mode is determined applicable.
    bool doinglegato     = false; // true when we determined we do a legato note.
    bool ismonofirstnote = false; /*(In Mono/Legato) true when we determined
                  no other notes are held down or sustained.*/
    int lastnotecopy     = lastnote; //Useful after lastnote has been changed.

    if(Pnoteon == 0)
        return;
    if((note < minKey()) || (note > maxKey()))
        return;

    // MonoMem stuff:
    if(Ppolymode == 0) { // If Poly is off
        monomemnotes.push_back(note); // Add note to the list.
        monomem[note].velocity  = velocity; // Store this note's velocity.
        monomem[note].mkeyshift = masterkeyshift; /* Store masterkeyshift too,
                         I'm not sure why though... */
        if((partnote[lastpos].status != KEY_PLAYING)
           && (partnote[lastpos].status != KEY_RELASED_AND_SUSTAINED))
            ismonofirstnote = true; // No other keys are held or sustained.
    }
    else
    // Poly mode is On so just make sure the list is empty.
    if(not monomemnotes.empty())
        monomemnotes.clear();

    lastnote = note;

    pos      = -1;
    for(i = 0; i < POLIPHONY; i++) {
        if(partnote[i].status == KEY_OFF) {
            pos = i;
            break;
        }
    }

    if((Plegatomode != 0) && (Pdrummode == 0)) {
        if(Ppolymode != 0) {
            fprintf(
                stderr,
                "ZynAddSubFX WARNING: Poly and Legato modes are both On, that should not happen ! ... Disabling Legato mode ! - (Part.cpp::NoteOn(..))\n");
            Plegatomode = 0;
        }
        else {
            // Legato mode is on and applicable.
            legatomodevalid = true;
            if((not ismonofirstnote) && (lastlegatomodevalid)) {
                // At least one other key is held or sustained, and the
                // previous note was played while in valid legato mode.
                doinglegato = true; // So we'll do a legato note.
                pos  = lastpos; // A legato note uses same pos as previous..
                posb = lastposb; // .. same goes for posb.
            }
            else {
                // Legato mode is valid, but this is only a first note.
                for(i = 0; i < POLIPHONY; i++)
                    if((partnote[i].status == KEY_PLAYING)
                       || (partnote[i].status == KEY_RELASED_AND_SUSTAINED))
                        RelaseNotePos(i);

                // Set posb
                posb = (pos + 1) % POLIPHONY; //We really want it (if the following fails)
                for(i = 0; i < POLIPHONY; i++)
                    if((partnote[i].status == KEY_OFF) && (pos != i)) {
                        posb = i;
                        break;
                    }
            }
            lastposb = posb; // Keep a trace of used posb
        }
    }
    else     // Legato mode is either off or non-applicable.
    if(Ppolymode == 0) {   //if the mode is 'mono' turn off all other notes
        for(i = 0; i < POLIPHONY; i++)
            if(partnote[i].status == KEY_PLAYING)
                RelaseNotePos(i);
        RelaseSustainedKeys();
    }
    lastlegatomodevalid = legatomodevalid;

    if(pos == -1)
        //test
        fprintf(stderr,
                "%s",
                "NOTES TOO MANY (> POLIPHONY) - (Part.cpp::NoteOn(..))\n");
    else {
        //start the note
        partnote[pos].status = KEY_PLAYING;
        partnote[pos].note   = note;
        if(legatomodevalid) {
            partnote[posb].status = KEY_PLAYING;
            partnote[posb].note   = note;
        }

        //this computes the velocity sensing of the part
        REALTYPE vel = VelF(velocity / 127.0, velSns());

        //compute the velocity offset
        vel += (velOffs() - 64.0) / 64.0;
        if(vel < 0.0)
            vel = 0.0;
        else
        if(vel > 1.0)
            vel = 1.0;

        //compute the keyshift
        int partkeyshift = keyShift() - 64;
        int keyshift     = masterkeyshift + partkeyshift;

        //initialise note frequency
        REALTYPE notebasefreq;
        if(Pdrummode == 0) {
            notebasefreq = microtonal->getnotefreq(note, keyshift);
            if(notebasefreq < 0.0)
                return;                  //the key is no mapped
        }
        else
            notebasefreq = 440.0 * pow(2.0, (note - 69.0) / 12.0);
        ;

        //Portamento
        if(oldfreq < 1.0)
            oldfreq = notebasefreq;           //this is only the first note is played

        // For Mono/Legato: Force Portamento Off on first
        // notes. That means it is required that the previous note is
        // still held down or sustained for the Portamento to activate
        // (that's like Legato).
        int portamento = 0;
        if((Ppolymode != 0) || (not ismonofirstnote))
            // I added a third argument to the
            // ctl.initportamento(...) function to be able
            // to tell it if we're doing a legato note.
            portamento = ctl.initportamento(oldfreq, notebasefreq, doinglegato);

        if(portamento != 0)
            ctl.portamento.noteusing = pos;
        oldfreq = notebasefreq;

        lastpos = pos; // Keep a trace of used pos.

        if(doinglegato) {
            // Do Legato note
            if(Pkitmode == 0) { // "normal mode" legato note
                if((kit[0].Padenabled != 0)
                   && (partnote[pos].kititem[0].adnote != NULL)
                   && (partnote[posb].kititem[0].adnote != NULL)) {
                    partnote[pos].kititem[0].adnote->ADlegatonote(notebasefreq,
                                                                  vel,
                                                                  portamento,
                                                                  note,
                                                                  true);                                     //'true' is to tell it it's being called from here.
                    partnote[posb].kititem[0].adnote->ADlegatonote(notebasefreq,
                                                                   vel,
                                                                   portamento,
                                                                   note,
                                                                   true);
                }

                if((kit[0].Psubenabled != 0)
                   && (partnote[pos].kititem[0].subnote != NULL)
                   && (partnote[posb].kititem[0].subnote != NULL)) {
                    partnote[pos].kititem[0].subnote->SUBlegatonote(
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        true);
                    partnote[posb].kititem[0].subnote->SUBlegatonote(
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        true);
                }

                if((kit[0].Ppadenabled != 0)
                   && (partnote[pos].kititem[0].padnote != NULL)
                   && (partnote[posb].kititem[0].padnote != NULL)) {
                    partnote[pos].kititem[0].padnote->PADlegatonote(
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        true);
                    partnote[posb].kititem[0].padnote->PADlegatonote(
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        true);
                }
            }
            else {   // "kit mode" legato note
                int ci = 0;
                for(int item = 0; item < NUM_KIT_ITEMS; item++) {
                    if(kit[item].Pmuted != 0)
                        continue;
                    if((note < kit[item].Pminkey) || (note > kit[item].Pmaxkey))
                        continue;

                    if((lastnotecopy < kit[item].Pminkey)
                       || (lastnotecopy > kit[item].Pmaxkey))
                        continue; // We will not perform legato across 2 key regions.

                    partnote[pos].kititem[ci].sendtoparteffect  =
                        (kit[item].Psendtoparteffect <
                         NUM_PART_EFX ? kit[item].Psendtoparteffect :
                         NUM_PART_EFX);                                                                                                                //if this parameter is 127 for "unprocessed"
                    partnote[posb].kititem[ci].sendtoparteffect =
                        (kit[item].Psendtoparteffect <
                         NUM_PART_EFX ? kit[item].Psendtoparteffect :
                         NUM_PART_EFX);

                    if((kit[item].Padenabled != 0) && (kit[item].adpars != NULL)
                       && (partnote[pos].kititem[ci].adnote != NULL)
                       && (partnote[posb].kititem[ci].adnote != NULL)) {
                        partnote[pos].kititem[ci].adnote->ADlegatonote(
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);
                        partnote[posb].kititem[ci].adnote->ADlegatonote(
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);
                    }
                    if((kit[item].Psubenabled != 0)
                       && (kit[item].subpars != NULL)
                       && (partnote[pos].kititem[ci].subnote != NULL)
                       && (partnote[posb].kititem[ci].subnote != NULL)) {
                        partnote[pos].kititem[ci].subnote->SUBlegatonote(
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);
                        partnote[posb].kititem[ci].subnote->SUBlegatonote(
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);
                    }
                    if((kit[item].Ppadenabled != 0)
                       && (kit[item].padpars != NULL)
                       && (partnote[pos].kititem[ci].padnote != NULL)
                       && (partnote[posb].kititem[ci].padnote != NULL)) {
                        partnote[pos].kititem[ci].padnote->PADlegatonote(
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);
                        partnote[posb].kititem[ci].padnote->PADlegatonote(
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);
                    }

                    if((kit[item].adpars != NULL)
                       || (kit[item].subpars != NULL)
                       || (kit[item].padpars != NULL)) {
                        ci++;
                        if(((kit[item].Padenabled != 0)
                            || (kit[item].Psubenabled != 0)
                            || (kit[item].Ppadenabled != 0)) && (Pkitmode == 2))
                            break;
                    }
                }
                if(ci == 0) {
                    // No legato were performed at all, so pretend nothing happened:
                    monomemnotes.pop_back(); // Remove last note from the list.
                    lastnote = lastnotecopy; // Set lastnote back to previous value.
                }
            }
            return; // Ok, Legato note done, return.
        }

        partnote[pos].itemsplaying = 0;
        if(legatomodevalid)
            partnote[posb].itemsplaying = 0;

        if(Pkitmode == 0) { //init the notes for the "normal mode"
            partnote[pos].kititem[0].sendtoparteffect = 0;
            if(kit[0].Padenabled != 0)
                partnote[pos].kititem[0].adnote = new ADnote(kit[0].adpars,
                                                             &ctl,
                                                             notebasefreq,
                                                             vel,
                                                             portamento,
                                                             note,
                                                             false);
            if(kit[0].Psubenabled != 0)
                partnote[pos].kititem[0].subnote = new SUBnote(kit[0].subpars,
                                                               &ctl,
                                                               notebasefreq,
                                                               vel,
                                                               portamento,
                                                               note,
                                                               false);
            if(kit[0].Ppadenabled != 0)
                partnote[pos].kititem[0].padnote = new PADnote(kit[0].padpars,
                                                               &ctl,
                                                               notebasefreq,
                                                               vel,
                                                               portamento,
                                                               note,
                                                               false);
            if((kit[0].Padenabled != 0) || (kit[0].Psubenabled != 0)
               || (kit[0].Ppadenabled != 0))
                partnote[pos].itemsplaying++;

            // Spawn another note (but silent) if legatomodevalid==true
            if(legatomodevalid) {
                partnote[posb].kititem[0].sendtoparteffect = 0;
                if(kit[0].Padenabled != 0)
                    partnote[posb].kititem[0].adnote = new ADnote(kit[0].adpars,
                                                                  &ctl,
                                                                  notebasefreq,
                                                                  vel,
                                                                  portamento,
                                                                  note,
                                                                  true);                                                                        //true for silent.
                if(kit[0].Psubenabled != 0)
                    partnote[posb].kititem[0].subnote = new SUBnote(
                        kit[0].subpars,
                        &ctl,
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        true);
                if(kit[0].Ppadenabled != 0)
                    partnote[posb].kititem[0].padnote = new PADnote(
                        kit[0].padpars,
                        &ctl,
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        true);
                if((kit[0].Padenabled != 0) || (kit[0].Psubenabled != 0)
                   || (kit[0].Ppadenabled != 0))
                    partnote[posb].itemsplaying++;
            }
        }
        else {  //init the notes for the "kit mode"
            for(int item = 0; item < NUM_KIT_ITEMS; item++) {
                if(kit[item].Pmuted != 0)
                    continue;
                if((note < kit[item].Pminkey) || (note > kit[item].Pmaxkey))
                    continue;

                int ci = partnote[pos].itemsplaying; //ci=current item

                partnote[pos].kititem[ci].sendtoparteffect =
                    (kit[item].Psendtoparteffect < NUM_PART_EFX ?
                     kit[item].
                     Psendtoparteffect : NUM_PART_EFX);            //if this parameter is 127 for "unprocessed"

                if((kit[item].adpars != NULL) && ((kit[item].Padenabled) != 0))
                    partnote[pos].kititem[ci].adnote = new ADnote(
                        kit[item].adpars,
                        &ctl,
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        false);

                if((kit[item].subpars != NULL) && ((kit[item].Psubenabled) != 0))
                    partnote[pos].kititem[ci].subnote = new SUBnote(
                        kit[item].subpars,
                        &ctl,
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        false);

                if((kit[item].padpars != NULL) && ((kit[item].Ppadenabled) != 0))
                    partnote[pos].kititem[ci].padnote = new PADnote(
                        kit[item].padpars,
                        &ctl,
                        notebasefreq,
                        vel,
                        portamento,
                        note,
                        false);

                // Spawn another note (but silent) if legatomodevalid==true
                if(legatomodevalid) {
                    partnote[posb].kititem[ci].sendtoparteffect =
                        (kit[item].Psendtoparteffect <
                         NUM_PART_EFX ? kit[item].Psendtoparteffect :
                         NUM_PART_EFX);                                                                                                                 //if this parameter is 127 for "unprocessed"

                    if((kit[item].adpars != NULL)
                       && ((kit[item].Padenabled) != 0))
                        partnote[posb].kititem[ci].adnote = new ADnote(
                            kit[item].adpars,
                            &ctl,
                            notebasefreq,
                            vel,
                            portamento,
                            note,
                            true);                                                                                                //true for silent.
                    if((kit[item].subpars != NULL)
                       && ((kit[item].Psubenabled) != 0))
                        partnote[posb].kititem[ci].subnote =
                            new SUBnote(kit[item].subpars,
                                        &ctl,
                                        notebasefreq,
                                        vel,
                                        portamento,
                                        note,
                                        true);
                    if((kit[item].padpars != NULL)
                       && ((kit[item].Ppadenabled) != 0))
                        partnote[posb].kititem[ci].padnote =
                            new PADnote(kit[item].padpars,
                                        &ctl,
                                        notebasefreq,
                                        vel,
                                        portamento,
                                        note,
                                        true);

                    if((kit[item].adpars != NULL) || (kit[item].subpars != NULL))
                        partnote[posb].itemsplaying++;
                }

                if((kit[item].adpars != NULL) || (kit[item].subpars != NULL)) {
                    partnote[pos].itemsplaying++;
                    if(((kit[item].Padenabled != 0)
                        || (kit[item].Psubenabled != 0)
                        || (kit[item].Ppadenabled != 0))
                       && (Pkitmode == 2))
                        break;
                }
            }
        }
    }

    //this only relase the keys if there is maximum number of keys allowed
    setkeylimit(Pkeylimit);
}

/*
 * Note Off Messages
 */
void Part::NoteOff(unsigned char note) //relase the key
{
    int i;

    // This note is released, so we remove it from the list.
    if(not monomemnotes.empty())
        monomemnotes.remove(note);

    for(i = POLIPHONY - 1; i >= 0; i--) //first note in, is first out if there are same note multiple times
        if((partnote[i].status == KEY_PLAYING) && (partnote[i].note == note)) {
            if(ctl.sustain.sustain == 0) { //the sustain pedal is not pushed
                if((Ppolymode == 0) && (not monomemnotes.empty()))
                    MonoMemRenote(); // To play most recent still held note.
                else
                    RelaseNotePos(i);
                    /// break;
            }
            else    //the sustain pedal is pushed
                partnote[i].status = KEY_RELASED_AND_SUSTAINED;
        }
}

/*
 * Controllers
 */
void Part::SetController(unsigned int type, int par)
{
    switch(type) {
    case C_pitchwheel:
        ctl.setpitchwheel(par);
        break;
    case C_expression:
        ctl.setexpression(par);
        setPvolume(Pvolume); //update the volume
        break;
    case C_portamento:
        ctl.setportamento(par);
        break;
    case C_panning:
        ctl.setpanning(par);
        panning.setInt(panning.getInt() + ctl.panning.pan);
        break;
    case C_filtercutoff:
        ctl.setfiltercutoff(par);
        break;
    case C_filterq:
        ctl.setfilterq(par);
        break;
    case C_bandwidth:
        ctl.setbandwidth(par);
        break;
    case C_modwheel:
        ctl.setmodwheel(par);
        break;
    case C_fmamp:
        ctl.setfmamp(par);
        break;
    case C_volume:
        ctl.setvolume(par);
        if(ctl.volume.receive != 0)
            partVolume.setInt(ctl.volume.volume);
        else
            setPvolume(Pvolume);
        break;
    case C_sustain:
        ctl.setsustain(par);
        if(ctl.sustain.sustain == 0)
            RelaseSustainedKeys();
        break;
    case C_allsoundsoff:
        AllNotesOff(); //Panic
        break;
    case C_resetallcontrollers:
        ctl.resetall();
        RelaseSustainedKeys();
        if(ctl.volume.receive != 0)
            partVolume.setInt(ctl.volume.volume);
        else
            setPvolume(Pvolume);
        setPvolume(Pvolume); //update the volume
        panning.setInt(panning.getInt() + ctl.panning.pan);

        for(int item = 0; item < NUM_KIT_ITEMS; item++) {
            if(kit[item].adpars == NULL)
                continue;
            kit[item].adpars->Reson->
            sendcontroller(C_resonance_center, 1.0);

            kit[item].adpars->Reson->
            sendcontroller(C_resonance_bandwidth, 1.0);
        }
        //more update to add here if I add controllers
        break;
    case C_allnotesoff:
        RelaseAllKeys();
        break;
    case C_resonance_center:
        ctl.setresonancecenter(par);
        for(int item = 0; item < NUM_KIT_ITEMS; item++) {
            if(kit[item].adpars == NULL)
                continue;
            kit[item].adpars->Reson->
            sendcontroller(C_resonance_center, ctl.resonancecenter.relcenter);
        }
        break;
    case C_resonance_bandwidth:
        ctl.setresonancebw(par);
        kit[0].adpars->Reson->
        sendcontroller(C_resonance_bandwidth, ctl.resonancebandwidth.relbw);
        break;
    }
}
/*
 * Relase the sustained keys
 */

void Part::RelaseSustainedKeys()
{
    // Let's call MonoMemRenote() on some conditions:
    if((Ppolymode == 0) && (not monomemnotes.empty()))
        if(monomemnotes.back() != lastnote) // Sustain controller manipulation would cause repeated same note respawn without this check.
            MonoMemRenote(); // To play most recent still held note.

    for(int i = 0; i < POLIPHONY; i++)
        if(partnote[i].status == KEY_RELASED_AND_SUSTAINED)
            RelaseNotePos(i);
}

/*
 * Relase all keys
 */

void Part::RelaseAllKeys()
{
    for(int i = 0; i < POLIPHONY; i++)
        if((partnote[i].status != KEY_RELASED)
           && (partnote[i].status != KEY_OFF)) //thanks to Frank Neumann
            RelaseNotePos(i);
    ;
}

// Call NoteOn(...) with the most recent still held key as new note
// (Made for Mono/Legato).
void Part::MonoMemRenote()
{
    unsigned char mmrtempnote = monomemnotes.back(); // Last list element.
    monomemnotes.pop_back(); // We remove it, will be added again in NoteOn(...).
    if(Pnoteon == 0)
        RelaseNotePos(lastpos);
    else
        NoteOn(mmrtempnote, monomem[mmrtempnote].velocity,
               monomem[mmrtempnote].mkeyshift);
}

/*
 * Release note at position
 */
void Part::RelaseNotePos(int pos)
{
    for(int j = 0; j < NUM_KIT_ITEMS; j++) {
        if(partnote[pos].kititem[j].adnote != NULL)
            if(partnote[pos].kititem[j].adnote)
                partnote[pos].kititem[j].adnote->relasekey();

        if(partnote[pos].kititem[j].subnote != NULL)
            if(partnote[pos].kititem[j].subnote != NULL)
                partnote[pos].kititem[j].subnote->relasekey();

        if(partnote[pos].kititem[j].padnote != NULL)
            if(partnote[pos].kititem[j].padnote)
                partnote[pos].kititem[j].padnote->relasekey();
    }
    partnote[pos].status = KEY_RELASED;
}


/*
 * Kill note at position
 */
void Part::KillNotePos(int pos)
{
    partnote[pos].status = KEY_OFF;
    partnote[pos].note   = -1;
    partnote[pos].time   = 0;
    partnote[pos].itemsplaying = 0;

    for(int j = 0; j < NUM_KIT_ITEMS; j++) {
        if(partnote[pos].kititem[j].adnote != NULL) {
            delete (partnote[pos].kititem[j].adnote);
            partnote[pos].kititem[j].adnote = NULL;
        }
        if(partnote[pos].kititem[j].subnote != NULL) {
            delete (partnote[pos].kititem[j].subnote);
            partnote[pos].kititem[j].subnote = NULL;
        }
        if(partnote[pos].kititem[j].padnote != NULL) {
            delete (partnote[pos].kititem[j].padnote);
            partnote[pos].kititem[j].padnote = NULL;
        }
    }
    if(pos == ctl.portamento.noteusing) {
        ctl.portamento.noteusing = -1;
        ctl.portamento.used      = 0;
    }
}


/*
 * Set Part's key limit
 */
void Part::setkeylimit(unsigned char Pkeylimit)
{
    this->Pkeylimit = Pkeylimit;
    int keylimit = Pkeylimit;
    if(keylimit == 0)
        keylimit = POLIPHONY - 5;

    //release old keys if the number of notes>keylimit
    if(Ppolymode != 0) {
        int notecount = 0;
        for(int i = 0; i < POLIPHONY; i++)
            if((partnote[i].status == KEY_PLAYING)
               || (partnote[i].status == KEY_RELASED_AND_SUSTAINED))
                notecount++;
        ;
        int oldestnotepos = -1, maxtime = 0;
        if(notecount > keylimit) { //find out the oldest note
            for(int i = 0; i < POLIPHONY; i++) {
                if(((partnote[i].status == KEY_PLAYING)
                    || (partnote[i].status == KEY_RELASED_AND_SUSTAINED))
                   && (partnote[i].time > maxtime)) {
                    maxtime = partnote[i].time;
                    oldestnotepos = i;
                }
            }
        }
        if(oldestnotepos != -1)
            RelaseNotePos(oldestnotepos);
    }
}


/*
 * Prepare all notes to be turned off
 */
void Part::AllNotesOff()
{
    killallnotes = 1;
}


/*
 * Compute Part samples and store them in the partoutl[] and partoutr[]
 */
void Part::ComputePartSmps()
{
    int i, k;
    int noteplay; //0 if there is nothing activated
    for(int nefx = 0; nefx < NUM_PART_EFX + 1; nefx++) {
        for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
            partfxinputl[nefx][i] = 0.0;
            partfxinputr[nefx][i] = 0.0;
        }
    }

    for(k = 0; k < POLIPHONY; k++) {
        if(partnote[k].status == KEY_OFF)
            continue;
        noteplay = 0;
        partnote[k].time++;
        //get the sampledata of the note and kill it if it's finished

        for(int item = 0; item < partnote[k].itemsplaying; item++) {
            int sendcurrenttofx = partnote[k].kititem[item].sendtoparteffect;

            ADnote  *adnote     = partnote[k].kititem[item].adnote;
            SUBnote *subnote    = partnote[k].kititem[item].subnote;
            PADnote *padnote    = partnote[k].kititem[item].padnote;
            //get from the ADnote
            if(adnote != NULL) {
                noteplay++;
                if(adnote->ready != 0)
                    adnote->noteout(&tmpoutl[0], &tmpoutr[0]);
                else
                    for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
                        tmpoutl[i] = 0.0;
                        tmpoutr[i] = 0.0;
                    }
                ;
                if(adnote->finished() != 0) {
                    delete (adnote);
                    partnote[k].kititem[item].adnote = NULL;
                }
                for(i = 0; i < SOUND_BUFFER_SIZE; i++) { //add the ADnote to part(mix)
                    partfxinputl[sendcurrenttofx][i] += tmpoutl[i];
                    partfxinputr[sendcurrenttofx][i] += tmpoutr[i];
                }
            }
            //get from the SUBnote
            if(subnote != NULL) {
                noteplay++;
                if(subnote->ready != 0)
                    subnote->noteout(&tmpoutl[0], &tmpoutr[0]);
                else
                    for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
                        tmpoutl[i] = 0.0;
                        tmpoutr[i] = 0.0;
                    }
                ;

                for(i = 0; i < SOUND_BUFFER_SIZE; i++) { //add the SUBnote to part(mix)
                    partfxinputl[sendcurrenttofx][i] += tmpoutl[i];
                    partfxinputr[sendcurrenttofx][i] += tmpoutr[i];
                }
                if(subnote->finished() != 0) {
                    delete (subnote);
                    partnote[k].kititem[item].subnote = NULL;
                }
            }
            //get from the PADnote
            if(padnote != NULL) {
                noteplay++;
                if(padnote->ready != 0)
                    padnote->noteout(&tmpoutl[0], &tmpoutr[0]);
                else
                    for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
                        tmpoutl[i] = 0.0;
                        tmpoutr[i] = 0.0;
                    }
                ;
                if(padnote->finished() != 0) {
                    delete (padnote);
                    partnote[k].kititem[item].padnote = NULL;
                }
                for(i = 0; i < SOUND_BUFFER_SIZE; i++) { //add the PADnote to part(mix)
                    partfxinputl[sendcurrenttofx][i] += tmpoutl[i];
                    partfxinputr[sendcurrenttofx][i] += tmpoutr[i];
                }
            }
        }
        //Kill note if there is no synth on that note
        if(noteplay == 0)
            KillNotePos(k);
    }


    //Apply part's effects and mix them
    for(int nefx = 0; nefx < NUM_PART_EFX; nefx++) {
        if(!Pefxbypass[nefx]) {
            partefx[nefx]->out(partfxinputl[nefx], partfxinputr[nefx]);
            if(Pefxroute[nefx] == 2) {
                for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
                    partfxinputl[nefx + 1][i] += partefx[nefx]->efxoutl[i];
                    partfxinputr[nefx + 1][i] += partefx[nefx]->efxoutr[i];
                }
            }
        }
        int routeto = ((Pefxroute[nefx] == 0) ? nefx + 1 : NUM_PART_EFX);
        for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
            partfxinputl[routeto][i] += partfxinputl[nefx][i];
            partfxinputr[routeto][i] += partfxinputr[nefx][i];
        }
    }
    for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
        partoutl[i] = partfxinputl[NUM_PART_EFX][i];
        partoutr[i] = partfxinputr[NUM_PART_EFX][i];
    }

    //Kill All Notes if killallnotes!=0
    if(killallnotes != 0) {
        for(i = 0; i < SOUND_BUFFER_SIZE; i++) {
            REALTYPE tmp =
                (SOUND_BUFFER_SIZE - i) / (REALTYPE) SOUND_BUFFER_SIZE;
            partoutl[i] *= tmp;
            partoutr[i] *= tmp;
            tmpoutl[i]   = 0.0;
            tmpoutr[i]   = 0.0;
        }
        for(int k = 0; k < POLIPHONY; k++)
            KillNotePos(k);
        killallnotes = 0;
        for(int nefx = 0; nefx < NUM_PART_EFX; nefx++)
            partefx[nefx]->cleanup();
        ;
    }
    ctl.updateportamento();
}

/*
 * Parameter control
 */
void Part::setPvolume(char Pvolume_)
{
    Pvolume = Pvolume_;
    partVolume.setInt(Pvolume_);
}

/*
 * Enable or disable a kit item
 */
void Part::setkititemstatus(int kititem, int Penabled_)
{
    if((kititem == 0) && (kititem >= NUM_KIT_ITEMS))
        return;                                        //nonexistent kit item and the first kit item is always enabled
    kit[kititem].Penabled = Penabled_;

    bool resetallnotes = false;
    if(Penabled_ == 0) {
        if(kit[kititem].adpars != NULL)
            delete (kit[kititem].adpars);
        if(kit[kititem].subpars != NULL)
            delete (kit[kititem].subpars);
        if(kit[kititem].padpars != NULL) {
            delete (kit[kititem].padpars);
            resetallnotes = true;
        }
        kit[kititem].adpars   = NULL;
        kit[kititem].subpars  = NULL;
        kit[kititem].padpars  = NULL;
        kit[kititem].Pname[0] = '\0';
    }
    else {
        if(kit[kititem].adpars == NULL)
            kit[kititem].adpars = new ADnoteParameters(&instrumentKit, fft);
        if(kit[kititem].subpars == NULL)
            kit[kititem].subpars = new SUBnoteParameters();
        if(kit[kititem].padpars == NULL)
            kit[kititem].padpars = new PADnoteParameters(fft, mutex);
    }

    if(resetallnotes)
        for(int k = 0; k < POLIPHONY; k++)
            KillNotePos(k);
}



void Part::add2XMLinstrument(XMLwrapper *xml)
{
    xml->beginbranch("INFO");
    xml->addparstr("name", (char *)Pname);
    xml->addparstr("author", (char *)info.Pauthor);
    xml->addparstr("comments", (char *)info.Pcomments);
    xml->addpar("type", info.Ptype);
    xml->endbranch();


    xml->beginbranch("INSTRUMENT_KIT");
    xml->addpar("kit_mode", Pkitmode);
    xml->addparbool("drum_mode", Pdrummode);

    for(int i = 0; i < NUM_KIT_ITEMS; i++) {
        xml->beginbranch("INSTRUMENT_KIT_ITEM", i);
        xml->addparbool("enabled", kit[i].Penabled);
        if(kit[i].Penabled != 0) {
            xml->addparstr("name", (char *)kit[i].Pname);

            xml->addparbool("muted", kit[i].Pmuted);
            xml->addpar("min_key", kit[i].Pminkey);
            xml->addpar("max_key", kit[i].Pmaxkey);

            xml->addpar("send_to_instrument_effect", kit[i].Psendtoparteffect);

            xml->addparbool("add_enabled", kit[i].Padenabled);
            if((kit[i].Padenabled != 0) && (kit[i].adpars != NULL)) {
                xml->beginbranch("ADD_SYNTH_PARAMETERS");
                kit[i].adpars->add2XML(xml);
                xml->endbranch();
            }

            xml->addparbool("sub_enabled", kit[i].Psubenabled);
            if((kit[i].Psubenabled != 0) && (kit[i].subpars != NULL)) {
                xml->beginbranch("SUB_SYNTH_PARAMETERS");
                kit[i].subpars->add2XML(xml);
                xml->endbranch();
            }

            xml->addparbool("pad_enabled", kit[i].Ppadenabled);
            if((kit[i].Ppadenabled != 0) && (kit[i].padpars != NULL)) {
                xml->beginbranch("PAD_SYNTH_PARAMETERS");
                kit[i].padpars->add2XML(xml);
                xml->endbranch();
            }
        }
        xml->endbranch();
    }
    xml->endbranch();

    xml->beginbranch("INSTRUMENT_EFFECTS");
    for(int nefx = 0; nefx < NUM_PART_EFX; nefx++) {
        xml->beginbranch("INSTRUMENT_EFFECT", nefx);
        xml->beginbranch("EFFECT");
        partefx[nefx]->add2XML(xml);
        xml->endbranch();

        xml->addpar("route", Pefxroute[nefx]);
        partefx[nefx]->setdryonly(Pefxroute[nefx] == 2);
        xml->addparbool("bypass", Pefxbypass[nefx]);
        xml->endbranch();
    }
    xml->endbranch();
}


void Part::add2XML(XMLwrapper *xml)
{
    //parameters
    xml->addparbool("enabled", enabled.getInt());
    if((enabled()) && (xml->minimal))
        return;

    xml->addpar("volume", Pvolume);
    xml->addpar("panning", panning.getInt());

    xml->addpar("min_key", minKey());
    xml->addpar("max_key", maxKey());
    xml->addpar("key_shift", keyShift());
    xml->addpar("rcv_chn", receiveChannel());

    xml->addpar("velocity_sensing", velSns());
    xml->addpar("velocity_offset", velOffs());

    xml->addparbool("note_on", Pnoteon);
    xml->addparbool("poly_mode", Ppolymode);
    xml->addpar("legato_mode", Plegatomode);
    xml->addpar("key_limit", Pkeylimit);

    xml->beginbranch("INSTRUMENT");
    add2XMLinstrument(xml);
    xml->endbranch();

    xml->beginbranch("CONTROLLER");
    ctl.add2XML(xml);
    xml->endbranch();
}

int Part::saveXML(const char *filename)
{
    XMLwrapper *xml;
    xml = new XMLwrapper();

    xml->beginbranch("INSTRUMENT");
    add2XMLinstrument(xml);
    xml->endbranch();

    int result = xml->saveXMLfile(filename);
    delete (xml);
    return result;
}

int Part::loadXMLinstrument(const char *filename)
{
    XMLwrapper *xml = new XMLwrapper();
    if(xml->loadXMLfile(filename) < 0) {
        delete (xml);
        return -1;
    }

    if(xml->enterbranch("INSTRUMENT") == 0)
        return -10;
    getfromXMLinstrument(xml);
    xml->exitbranch();

    delete (xml);
    return 0;
}


void Part::applyparameters(bool lockmutex)
{
    for(int n = 0; n < NUM_KIT_ITEMS; n++)
        if((kit[n].padpars != NULL) && (kit[n].Ppadenabled != 0))
            kit[n].padpars->applyparameters(lockmutex);
}

void Part::getfromXMLinstrument(XMLwrapper *xml)
{
    if(xml->enterbranch("INFO")) {
        xml->getparstr("name", (char *)Pname, PART_MAX_NAME_LEN);
        xml->getparstr("author", (char *)info.Pauthor, MAX_INFO_TEXT_SIZE);
        xml->getparstr("comments", (char *)info.Pcomments, MAX_INFO_TEXT_SIZE);
        info.Ptype = xml->getpar("type", info.Ptype, 0, 16);

        xml->exitbranch();
    }

    if(xml->enterbranch("INSTRUMENT_KIT")) {
        Pkitmode  = xml->getpar127("kit_mode", Pkitmode);
        Pdrummode = xml->getparbool("drum_mode", Pdrummode);

        setkititemstatus(0, 0);
        for(int i = 0; i < NUM_KIT_ITEMS; i++) {
            if(xml->enterbranch("INSTRUMENT_KIT_ITEM", i) == 0)
                continue;
            setkititemstatus(i, xml->getparbool("enabled", kit[i].Penabled));
            if(kit[i].Penabled == 0) {
                xml->exitbranch();
                continue;
            }

            xml->getparstr("name", (char *)kit[i].Pname, PART_MAX_NAME_LEN);

            kit[i].Pmuted  = xml->getparbool("muted", kit[i].Pmuted);
            kit[i].Pminkey = xml->getpar127("min_key", kit[i].Pminkey);
            kit[i].Pmaxkey = xml->getpar127("max_key", kit[i].Pmaxkey);

            kit[i].Psendtoparteffect = xml->getpar127(
                "send_to_instrument_effect",
                kit[i].Psendtoparteffect);

            kit[i].Padenabled = xml->getparbool("add_enabled",
                                                kit[i].Padenabled);
            if(xml->enterbranch("ADD_SYNTH_PARAMETERS")) {
                kit[i].adpars->getfromXML(xml);
                xml->exitbranch();
            }

            kit[i].Psubenabled = xml->getparbool("sub_enabled",
                                                 kit[i].Psubenabled);
            if(xml->enterbranch("SUB_SYNTH_PARAMETERS")) {
                kit[i].subpars->getfromXML(xml);
                xml->exitbranch();
            }

            kit[i].Ppadenabled = xml->getparbool("pad_enabled",
                                                 kit[i].Ppadenabled);
            if(xml->enterbranch("PAD_SYNTH_PARAMETERS")) {
                kit[i].padpars->getfromXML(xml);
                xml->exitbranch();
            }

            xml->exitbranch();
        }

        xml->exitbranch();
    }


    if(xml->enterbranch("INSTRUMENT_EFFECTS")) {
        for(int nefx = 0; nefx < NUM_PART_EFX; nefx++) {
            if(xml->enterbranch("INSTRUMENT_EFFECT", nefx) == 0)
                continue;
            if(xml->enterbranch("EFFECT")) {
                partefx[nefx]->getfromXML(xml);
                xml->exitbranch();
            }

            Pefxroute[nefx]  = xml->getpar("route",
                                           Pefxroute[nefx],
                                           0,
                                           NUM_PART_EFX);
            partefx[nefx]->setdryonly(Pefxroute[nefx] == 2);
            Pefxbypass[nefx] = xml->getparbool("bypass", Pefxbypass[nefx]);
            xml->exitbranch();
        }
        xml->exitbranch();
    }
}

void Part::getfromXML(XMLwrapper *xml)
{
    enabled.setInt(xml->getparbool("enabled", enabled.getInt()));

    setPvolume(xml->getpar127("volume", Pvolume));
    panning.setInt(xml->getpar127("panning", panning.getInt()));

    minKey.setValue(xml->getpar127("min_key", minKey()));
    maxKey.setValue(xml->getpar127("max_key", maxKey()));
    keyShift.setValue(xml->getpar127("key_shift", keyShift()));
    receiveChannel.setInt(xml->getpar127("rcv_chn", receiveChannel()));

    velSns.setValue(xml->getpar127("velocity_sensing", velSns()));
    velOffs.setValue(xml->getpar127("velocity_offset", velOffs()));

    Pnoteon     = xml->getparbool("note_on", Pnoteon);
    Ppolymode   = xml->getparbool("poly_mode", Ppolymode);
    Plegatomode = xml->getparbool("legato_mode", Plegatomode); //older versions
    if(!Plegatomode)
        Plegatomode = xml->getpar127("legato_mode", Plegatomode);
    Pkeylimit   = xml->getpar127("key_limit", Pkeylimit);


    if(xml->enterbranch("INSTRUMENT")) {
        getfromXMLinstrument(xml);
        xml->exitbranch();
    }

    if(xml->enterbranch("CONTROLLER")) {
        ctl.getfromXML(xml);
        xml->exitbranch();
    }
}

void Part::handleSyncEvent(Event *event)
{
    if(event->type() == Event::NewValueEvent) {
        NewValueEvent *newValue = static_cast<NewValueEvent *>(event);

        if(newValue->control == &instrumentControl) {
            instrumentControl.bank->loadfromslot(instrumentControl.getInt(), this);

            //The current nio implementation locks the master mutex before handling jobs etc, which
            //means that we have to be careful not to lock the mutex from here.
            applyparameters(false);
        }
    }
}

void Part::handleEvent(Event *event)
{
    if(event->type() == Event::NewValueEvent) {
        NewValueEvent *newValue = static_cast<NewValueEvent *>(event);

        if (newValue->wasChanged) {

            if(newValue->control == &bankControl)

                instrumentControl.loadBank(
                        bankControl.bank->banks[bankControl()].dir);

            else if (newValue->control == &instrumentControl) {

                Job::push(new NodeJob(this, new NewValueEvent(*newValue)));

            } else {

                std::cout << "Part: NewValueEvent for unknown control" << std::endl;

            }
        }
    }
}

void Part::enablePart()
{
    //if (Penabled) return;
    //Penabled = true;
}

void Part::disablePart()
{
    //if (!Penabled) return;
    //Penabled = false;
}

