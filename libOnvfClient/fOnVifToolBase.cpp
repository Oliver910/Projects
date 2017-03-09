#include "fOnVifToolBase.h"


const char *fOnVifToolBase::szOnVifConType_header="Content-Type: application/soap+xml; charset=utf-8";
fOnVifToolBase::fOnVifToolBase(CUrlTool *pUrl)
    :m_pUrl(pUrl)
{
}
fOnVifToolBase::~fOnVifToolBase()
{
}
bool fOnVifToolBase::get_xml_string(xmlNodePtr pNode,std::string &strVal)
{
    xmlChar *pXmlVal = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
    if(pXmlVal!=NULL) {
        //¨ú¥X­È¨Ó
        strVal = std::string((const char *)pXmlVal);
        xmlFree(pXmlVal);
        pXmlVal=NULL;
        return true;
    }
    return false;
}
bool fOnVifToolBase::get_xml_attr(xmlNodePtr pNode,std::string strAttr,std::string &strVal)
{
    xmlChar *pXmlVal = xmlGetProp(pNode,BAD_CAST strAttr.c_str());
    if(pXmlVal!=NULL) {
        strVal = std::string((const char *)pXmlVal);
        xmlFree(pXmlVal);
        pXmlVal = NULL;
        return true;
    }
    return false;
}


xmlDocPtr fOnVifToolBase::get_onvif_body(std::string m_sUrl,xmlNodePtr &pBodyNode)
{
    xmlNodePtr rootNode;
    xmlDocPtr   xmlReqDoc =	 xmlReadMemory		(m_sUrl.c_str(),m_sUrl.length(), NULL,"UTF-8",0);
    pBodyNode = NULL;
    if(xmlReqDoc) {
        xmlNodePtr rootNode;
        rootNode = xmlDocGetRootElement(xmlReqDoc);
        if(rootNode!=NULL) {
            if(xmlStrcmp  (rootNode->name,BAD_CAST "Envelope")!=0) {
                return NULL;
            }
            xmlNodePtr pBody = rootNode->children;
            while(pBody!=NULL) {
                if(xmlStrcmp  (pBody->name,BAD_CAST "Body")==0) {
                    pBodyNode = 	pBody;
                    return xmlReqDoc;
                }
                pBody =  pBody->next;
            }
        }
    }
    return NULL;
}
bool fOnVifToolBase::send_onvif_cmd(std::string strOnvifSOAP,std::string &strResponse)
{
    get_url()->lock();
    get_url()->set_mothod(CUrlTool::enPOST);
    get_url()->append_header(szOnVifConType_header);
    get_url()->append_header("Connection: Keep-Alive");
    get_url()->set_url(m_strCmdUrl);
    get_url()->set_set_form_body(strOnvifSOAP);
    get_url()->perform();
    strResponse = get_url()->m_strResponse;
    get_url()->termination();
    get_url()->unlock();
    return true;
}
bool fOnVifToolBase::is_fault(xmlNodePtr pBody, TOnVif_FaultResponse &f)
{
    xmlNodePtr pFaultNode = pBody->children;
    if( xmlStrcmp(pFaultNode->name,BAD_CAST "Fault")==0 ) {
        xmlChar *doc_txt_ptr=NULL;
        int  doc_txt_len=0;
        xmlDocDumpFormatMemory(pBody->doc,&doc_txt_ptr,&doc_txt_len,1);
        if(doc_txt_ptr!=NULL) {
            f.m_enResult = TOnVif_FaultResponse::enFAULT;
            f.m_strFaultBody = (const char *)(doc_txt_ptr);
            xmlFree(doc_txt_ptr);
            doc_txt_ptr = NULL;
        }
        return true;
    }
    return false;
}