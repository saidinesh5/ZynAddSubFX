#include "DelayCtl.h"
#include <sstream>

DelayCtl::DelayCtl()
    :Control(NULL, 64),value(64/127.0*1.5){} /**\todo finishme*/

std::string DelayCtl::getString() const
{
    std::ostringstream buf;
    buf<<value;
    return (buf.str() + " Seconds");
}

void DelayCtl::setValue(char nval)
{
    /**\todo add locking code*/
    //value=1+(int)(nval/127.0*SAMPLE_RATE*1.5);//0 .. 1.5 sec
    value=(nval/127.0*1.5);//0 .. 1.5 sec
}

char DelayCtl::getValue() const
{
    return((char)(value/1.5*127.0));
}

float DelayCtl::getiVal() const
{
    return(value);
}
