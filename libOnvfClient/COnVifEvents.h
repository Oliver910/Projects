#ifndef _CONVIFEVENT_H_
#define _CONVIFEVENT_H_

#include "fOnVifToolBase.h"

class COnVifEvent
    :public fOnVifToolBase
{
public:
    COnVifEvent(CUrlTool *pUrl);
    virtual ~COnVifEvent();
};
#endif
