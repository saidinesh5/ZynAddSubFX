#ifndef _CHARCONTROL_H_
#define _CHARCONTROL_H_

#include <string>
#include "Control.h"

class CharControl : public Control
{
public:
    CharControl(Node *parent, std::string id, std::string description, char min, char max, char defaultValue);

    virtual std::string getString()const;

    /**Set the Control to the given value
     * @param nval A number 0-127*/
    virtual void setValue(char nval);

    /**Return the midi value (aka the char)
     * @return the current value*/
    virtual char getValue()const;

private:
    char m_max, m_min, m_default, m_value;
    std::string m_desc;

};

#endif /* ifndef _CHARCONTROL_H_ */
