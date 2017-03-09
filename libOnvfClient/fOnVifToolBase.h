#ifndef _FONVIFTOOLBASE_H_
#define _FONVIFTOOLBASE_H_

#include "CUrlTool.h"
#include "COnVifConfDef.h"
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

class fOnVifToolBase
{
protected:
    CUrlTool *m_pUrl;
    std::string m_strCmdUrl;
public:
    static const char *szOnVifConType_header;
    fOnVifToolBase(CUrlTool *pUrl);
    virtual ~fOnVifToolBase();
    CUrlTool *get_url() {
        return m_pUrl;
    }
    bool send_onvif_cmd(std::string strOnvifSOAP,std::string &strResponse);
    bool get_xml_string(xmlNodePtr pNode,std::string &strVal);
    bool get_xml_attr(xmlNodePtr pNode,std::string strAttr,std::string &strVal);
    xmlDocPtr get_onvif_body(std::string m_sUrl,xmlNodePtr &pBodyNode);
    bool is_fault(xmlNodePtr pNode, TOnVif_FaultResponse &f);
};
#endif
