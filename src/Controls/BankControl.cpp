#include "BankControl.h"
#include "../Misc/Bank.h"

Bank *BankControl::bank = NULL;

BankControl::BankControl(Node *parent)
    :Selector(parent, "Bank", 1)
{
    if(!bank) {
        bank = new Bank();
        bank->rescanforbanks();
    }

    for(int i = 0; i < bank->banks.size(); ++i) {
        std::string name = bank->banks[i].name;
        if(name.empty())
            addOption(std::string("Empty bank"));
        else
            addOption(std::string(name));
    }
}

// vim: sw=4 sts=4 et tw=100

