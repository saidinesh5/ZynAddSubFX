#include "addnoteui.h"

AddNoteUi::AddNoteUi(QString id)
        : QDialog(NULL),
        id(id)
{
    setupUi(this);
    //amplitudeEnvelope->setSources(master->part[partIndex]->kit[kitIndex].adpars->GlobalPar.AmpEnvelope);
    //filterEnvelope->setSources(master->part[partIndex]->kit[kitIndex].adpars->GlobalPar.FilterEnvelope);
    //frequencyEnvelope->setSources(master->part[partIndex]->kit[kitIndex].adpars->GlobalPar.FreqEnvelope);

    //amplitudeLFO->setSources(master->part[partIndex]->kit[kitIndex].adpars->GlobalPar.AmpLfo);
    //filterLFO->setSources(master->part[partIndex]->kit[kitIndex].adpars->GlobalPar.FilterLfo);
    //frequencyLFO->setSources(master->part[partIndex]->kit[kitIndex].adpars->GlobalPar.FreqLfo);

}


#include "addnoteui.moc"
