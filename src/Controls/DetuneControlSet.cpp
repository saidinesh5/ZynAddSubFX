#include "DetuneControlSet.h"
#include "../Misc/Util.h"
#include "../Misc/XMLwrapper.h"

DetuneControlSet::DetuneControlSet(Node *parent, std::string prefix, int defaultType, DetuneControlSet *defaultSet)
:detune              (parent, prefix + "Detune", 8192, 0, 16383),
    octaveDetune         (parent, prefix + "OctaveDetune", 0, -8, 7),
    coarseDetune         (parent, prefix + "CoarseDetune", 0, -512, 511),
    detuneType           (parent, prefix + "DetuneType", defaultType),
    m_defaultSet(defaultSet)
{

}

void DetuneControlSet::defaults()
{
    detune.defaults();
    octaveDetune.defaults();
    coarseDetune.defaults();
    detuneType.defaults();
}

REALTYPE DetuneControlSet::getFine()
{
    //if a parent detunecontrolset has been set, and detunetype is set to default, use the settings
    //from this parent detunecontrolset
    if (m_defaultSet && !isEnabled()) {
        return newgetdetune(
                m_defaultSet->detuneType(),
                0,
                0,
                detune());
    }

    return newgetdetune(
            detuneType(),
            0,
            0,
            detune());
}

REALTYPE DetuneControlSet::getCoarse()
{
    if (m_defaultSet && !isEnabled()) {
        return newgetdetune(
                m_defaultSet->detuneType(),
                octaveDetune(),
                coarseDetune(),
                8192);
    }

    return newgetdetune(
            detuneType(),
            octaveDetune(),
            coarseDetune(),
            8192);
}

REALTYPE DetuneControlSet::get()
{
    if (m_defaultSet && !isEnabled()) {
        return newgetdetune(
                m_defaultSet->detuneType(),
                octaveDetune(),
                coarseDetune(),
                detune());
    }

    return newgetdetune(
            detuneType(),
            octaveDetune(),
            coarseDetune(),
            detune());
}

void DetuneControlSet::getFromXMLsection(XMLwrapper *xml)
{
    if (xml->versionAtLeast(3,0,0)) {
        detune.setValue         (xml->getpar(
                    "detune", detune(), 0, 16383));

        octaveDetune.setValue   (xml->getpar(
                    "octave_detune", octaveDetune(), 0, 16383));

        coarseDetune.setValue   (xml->getpar(
                    "coarse_detune", coarseDetune(), 0, 16383));

        detuneType.setValue     (xml->getpar127(
                    "detune_type", detuneType()));

    }
    else { //backwards compatible loading needed
        printf("backwards\n");
        detune.setValue         (xml->getpar(
                    "detune", detune(), 0, 16383));

        int coarse = xml->getpar(
                    "coarse_detune", coarseDetune(), 0, 16383);

        //Get Octave
        int octave   = coarse / 1024;
        if(octave >= 8)
            octave -= 16;

        int cdetune = coarse % 1024;
        if(cdetune > 512)
            cdetune -= 1024;


        //TODO: sjekk argumenter
        octaveDetune.setValue(octave);

        coarseDetune.setValue(cdetune);

        detuneType.setValue     (xml->getpar127(
                    "detune_type", detuneType()));

    }

}

void DetuneControlSet::add2XMLsection(XMLwrapper *xml)
{
    xml->addpar("detune", detune());
    xml->addpar("octave_detune", octaveDetune());
    xml->addpar("coarse_detune", coarseDetune());
    xml->addpar("detune_type", detuneType());
}


// vim: sw=4 sts=4 et tw=100
