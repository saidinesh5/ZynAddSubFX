#include "InstrumentControl.h"
#include "../Misc/Bank.h"

InstrumentControl::InstrumentControl(Node *parent)
    : Selector(parent, "BankInstrument", 0)
{
    bank = new Bank();
    loadBank(1);
}

void InstrumentControl::loadBank(int bankid)
{
    if (bankid > MAX_NUM_BANKS) return;
    clearOptions();

    if (bank->banks[bankid].name!=NULL)  {
        const char* dir = bank->banks[bankid].dir;
        int ret = bank->loadbank(dir);
    } else {
        std::cerr << "Failed to load bank...\n";
        return;
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
