#ifndef INSTRUMENTCONTROL_H
#define INSTRUMENTCONTROL_H

#include "Selector.h"

class InstrumentControl:public Selector
{
    public:
        InstrumentControl(Node *parent);
        virtual ~InstrumentControl();
        //virtual std::string getOption(int index) const;
        //virtual int numOptions() const;
        //private:
        class Bank * bank;
        void loadBank(char *dir);
};

#endif // INSTRUMENTCONTROL_H

