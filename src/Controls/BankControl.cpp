#include "BankControl.h"
#include "../Misc/Bank.h"

Bank * BankControl::bank = NULL;

BankControl::BankControl(Node *parent)
    : Selector(parent, "Bank", 1)
{
    if (!bank) {

        bank = new Bank();
        bank->rescanforbanks();
    }

    for (int i = 0; i < MAX_NUM_BANKS; ++i) {
        const char* name = bank->banks[i].name;
        if (!name) {
            addOption(std::string("Empty bank"));
        } else {
            addOption(std::string(name));
        }
    }
}

// vim: sw=4 sts=4 et tw=100
