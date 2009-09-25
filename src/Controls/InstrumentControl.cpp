#include "InstrumentControl.h"
#include "../Misc/Bank.h"

Bank * InstrumentControl::bank = NULL;

InstrumentControl::InstrumentControl(Node *parent)
    : Selector(parent, "BankInstrument", 0)
{
    if (!bank) {
        bank = new Bank();
        bank->rescanforbanks();

        for (int i=1;i<MAX_NUM_BANKS;i++) {
            if (bank->banks[i].name!=NULL)  {
                const char* dir = bank->banks[i].dir;

                int ret = bank->loadbank(dir);

                //qDebug() << "Loading " << bank->banks[i].name << " returned " << ret;
                break;
            }
        }
    }
    for (int i = 0; i < 128; ++i) {
        addOption(bank->getname(i));
    }
}

#if 0
std::string InstrumentControl::getOption(int index) const
{
    return std::string(bank->getname(index));
}

int InstrumentControl::numOptions() const
{
    return 128;
}
#endif

// vim: sw=4 sts=4 et tw=100
