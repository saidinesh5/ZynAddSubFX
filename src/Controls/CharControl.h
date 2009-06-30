#ifndef _CHARCONTROL_H_
#define _CHARCONTROL_H_

#include <string>
#include "Control.h"

class RealFunctor
{
public:
    virtual inline float operator()(unsigned char x)=0;
    virtual inline unsigned char operator()(float x)=0;
}

class LinFunctor: public RealFunctor
{
public:
    LinFunctor(float min,float max):b(min),m((max-min)/127)
    virtual float operator()(unsigned char x){m*x+b}
    virtual unsigned char operator()(float x){(char)((x-b)/m)}
private:
    float m,b;
}

class CharValueFunctor
{
public:
    virtual void valueSet(char value) {}
};

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

    void setFunctor(CharValueFunctor functor);

private:
    CharValueFunctor functor;
    char m_max, m_min, m_default, m_value;
    std::string m_desc;

};

#endif /* ifndef _CHARCONTROL_H_ */
