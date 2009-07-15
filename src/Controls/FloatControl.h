#ifndef _FLOATCONTROL_H_
#define _FLOATCONTROL_H_

#include <string>
#include "Control.h"

class FloatControl : public Control
{
public:
    FloatControl(Node *parent, std::string id, std::string description, float min, float max, float defaultValue);

    virtual std::string getString()const;

    void handleEvent(class Event *ev){};

    /**Set the Control to the given value
     * @param nval A number 0-127*/
    virtual void setValue(char nval);

    /**Return the midi value (aka the char)
     * @return the current value*/
    virtual char getValue()const;

    float getFloat() const;

    void setDb2rapConversion(bool value);

    virtual void handleSyncEvent(Event *event);

private:
    inline float toFloat(char val) const;
    inline char toChar(float val) const;
    float m_max, m_min, m_default, m_value;
    std::string m_desc;

    bool m_db2rapConversion;

};

#endif /* ifndef _FLOATCONTROL_H_ */
