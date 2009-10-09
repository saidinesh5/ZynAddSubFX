#ifndef _SOURCEDWIDGET_H_
#define _SOURCEDWIDGET_H_

#include "../globals.h"

class SourcedWidget
{
    public:
        void setSource(unsigned char *source);
        void setSource(REALTYPE *source);

    protected:
        virtual void slotUpdateSource() = 0;

    private:
        unsigned char *m_source;
        REALTYPE      *m_realtypeSource;
};

#endif /* ifndef _SOURCEDWIDGET_H_ */

