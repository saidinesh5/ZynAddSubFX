#include "InstrumentControl.h"
#include "../Misc/Bank.h"

using namespace std;

InstrumentControl::InstrumentControl(Node *parent)
    :Selector(parent, "BankInstrument", 0)
{
    bank = new Bank();
}

InstrumentControl::~InstrumentControl()
{
    delete bank;
}

void InstrumentControl::loadBank(char *dir)
{
    if(!dir) {
        cerr << dir << " is empty, returning...\n";
        return;
    }

    clearOptions();

    int ret = bank->loadbank(dir);
    if(-1 == ret) {
        cerr << "Failed to load bank " << dir << "\n";
        return;
    }

    for(int i = 0; i < 128; ++i)
        addOption(bank->getname(i));
    forward(new OptionsChangedEvent());
    cout << "Loaded " << dir << "\n";
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

