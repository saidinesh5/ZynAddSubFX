#ifndef CONTROLS_DETUNECONTROLSET_H
#define CONTROLS_DETUNECONTROLSET_H

#include "DescRanger.h"

class DetuneControlSet
{
    public:
        DetuneControlSet(Node *parent, std::string prefix,
                int defaultType, class DetuneControlSet* defaultSet = NULL);

        void defaults();
        REALTYPE getFine();
        REALTYPE getCoarse();
        REALTYPE get();

        bool isEnabled() { return (detuneType() != 0); }

        void getFromXMLsection(class XMLwrapper *xml);
        void add2XMLsection(class XMLwrapper *xml);

        /** Fine detune */
        DescRanger detune;

        /** Octave detune */
        DescRanger octaveDetune;

        /** Coarse detune NOTE: this also included octave in old zyn */
        DescRanger coarseDetune;

        /** Detune type */
        DescRanger detuneType;

    private:
        class DetuneControlSet *m_defaultSet;

};

#endif // CONTROLS_DETUNECONTROLSET_H
