#ifndef BANKCONTROL_H
#define BANKCONTROL_H

#include "Selector.h"

class BankControl : public Selector
{
    public:
        BankControl(Node *parent);
        class Bank *bank;
};

#endif // BANKCONTROL_H
