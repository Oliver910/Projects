#ifndef _CONVIFCLIENT_H_
#define _CONVIFCLIENT_H_

#include "CUrlTool.h"

extern "C" {
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/debugXML.h>
#include <libxml/xmlerror.h>
#include "libxml/encoding.h"
};
#include "COnVifDeviceMngmt.h"
#include "COnVifEvents.h"
#include "COnVifImagingConf.h"
#include "COnVifMediaConf.h"
#include "COnVifPTZ.h"

class COnVifClient
    :public COnVifDeviceMngmt
{
    std::string m_strUser;
    std::string m_strPassword;
    int  m_s32Port;
    std::string m_strHost;
    CUrlTool m_sUrl;

//	COnVifDeviceMngmt m_sDeviceMngmt;
    COnVifEvent       m_sEvents;
    COnVifImagingConf m_sImagingConf;
    COnVifMediaConf   m_sMediaConf;
    COnVifPTZ         m_sVifPTZ;

public:
    COnVifClient();
    virtual ~COnVifClient();
    void set_host(std::string strHost,int s32Port=80)
    {
        m_strHost = strHost;
        m_s32Port = s32Port;
        m_sUrl.set_host(m_strHost,m_s32Port);
    }
    void set_author(std::string strUser,std::string strPassword)
    {
        m_strUser     = strUser;
        m_strPassword = strPassword;
        m_sUrl.set_author(m_strUser,m_strPassword);
    }

    COnVifDeviceMngmt* getDeviceMngmt() {
        return this;
    } //return &m_sDeviceMngmt;}
    COnVifEvent*       getEvent()      {
        return &m_sEvents;
    }
    COnVifImagingConf* getImagingCon() {
        return &m_sImagingConf;
    }
    COnVifMediaConf*   getMediaConf()  {
        return &m_sMediaConf;
    }
    COnVifPTZ*         getPTZ()        {
        return &m_sVifPTZ;
    }
};
#endif
