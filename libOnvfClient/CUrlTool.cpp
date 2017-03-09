
#include "CUrlTool.h"
CUrlTool::CUrlTool()
    :m_enMethod (enPOST),m_enConnType(enAuto)
{
    m_pSocket = NULL;
    m_strHttpVersion="HTTP/1.1";
    m_s32DbgLevel=0;
}
CUrlTool::~CUrlTool()
{
    termination();
}
void CUrlTool::send_request()
{
    int s32Len;
    std::list<std::string>::iterator it;
    char szHeader[8192];
    char szCmdBuf[4096];
    memset(szHeader,0,sizeof(szHeader));
    memset(szCmdBuf,0,sizeof(szHeader));

    sprintf(szHeader,"%s /%s %s\r\n",get_method().c_str(),m_strUrl.c_str(),m_strHttpVersion.c_str());
    sprintf(szCmdBuf,"Host:%s\r\n",m_strHost.c_str());
    strcat(szHeader,szCmdBuf);
    for(it=m_lstHeader.begin(); it!=m_lstHeader.end(); ++it) {
        sprintf(szCmdBuf,"%s\r\n",it->c_str());
        strcat(szHeader,szCmdBuf);
    }
    if(m_strformBody.size()>0) {
        sprintf(szCmdBuf,"Content-Length: %lu\r\n",m_strformBody.size());
        strcat(szHeader,szCmdBuf);
    }
    strcat(szHeader,"\r\n");
    if(m_s32DbgLevel) {
        printf("%s",szHeader);
        if(m_enMethod == enPOST) {
            printf("%s",m_strformBody.c_str());
        }
    }
    s32Len = m_pSocket->write_block (szHeader,strlen(szHeader));
    if(m_enMethod == enPOST) {
        if(m_strformBody.length()>0) {
            s32Len = m_pSocket->write_block (m_strformBody.c_str(),m_strformBody.length());
        }
    }
    //send data;
}
int CUrlTool::recv_response_header()
{
    int s32ContentLength=-1;
    std::string strLine;
    char szBuf[2];
    int s32HeaderLen = 0, s32Len = 0;
    m_strResponseHeader.clear();
    while(1) {
        strLine.clear();
        while(1) {
            szBuf[1]='\0';
            s32Len = m_pSocket->read(szBuf,1);
            if(s32Len<=0) {
                break;
            }
            strLine += szBuf;
            if(szBuf[0]=='\n') {
                break;
            }
        }
        //printf("%s",strLine.c_str());
        m_strResponseHeader += strLine;
        if(strLine=="\r\n" || strLine=="\n") {
            break;
        }
        if(strncasecmp( strLine.c_str(), "Content-Length:", 15 ) == 0) {
            s32ContentLength = atoi(strLine.substr(15).c_str());
        }
    }
    //printf("s32ContentLength==>%d\n",s32ContentLength);
    if(m_s32DbgLevel) {
        printf("%s",m_strResponseHeader.c_str());
    }

    recv_response_body(s32ContentLength);
    if(m_s32DbgLevel) {
        printf("%s",m_strResponse.c_str());
    }
    return 0;
}
int CUrlTool::recv_response_body(int s32RecvLen)
{
    int s32Len = 0;
    HT_U32 s32TimeOut=10000;
    m_strResponse.clear();
    if(s32RecvLen > 0) {
        char *pszBuf = new char [s32RecvLen+1];
        if(pszBuf) {
            s32Len = m_pSocket->read_block(pszBuf,s32RecvLen,&s32TimeOut);
            pszBuf[s32Len]='\0';
            m_strResponse = pszBuf;
            delete [] pszBuf;
        }
        //printf("s32RecvLen=%d s32Len=%d\n",s32RecvLen,s32Len);
        return s32Len;
    }
    else {
        char szBuf[1025];
        while(1) {
            s32Len = m_pSocket->read_block(szBuf,1024);
            if(s32Len<=0)break;
            szBuf[s32Len]='\0';
            m_strResponse += szBuf;
        }
    }
    return m_strResponse.length();
}
void CUrlTool::recv_response()
{
    recv_response_header();
    //printf("%s",m_strResponse.c_str());
}
int CUrlTool::perform()
{
    if(m_pSocket==NULL);
    {
        m_pSocket = new CSocketIPv4(fSocketTool::Stream);
        m_pSocket->create();
        m_pSocket->connect(m_strHost, m_s32Port);
    }
    send_request();
    recv_response();
    return 0;
}
int CUrlTool::termination()
{
    if(m_pSocket!=NULL) {
        m_pSocket->destory();
        m_pSocket=NULL;
    }
    return 0;
}

std::string CUrlTool::get_ws_username_token()
{
    const char *fmt_ws_token=
        "<s:Header xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">"
        "<wsse:Security xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" "
        "xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">"
        " <wsse:UsernameToken>"
        "<wsse:Username>admin</wsse:Username>"
        "<wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">v+QqkQtQcYWolaIJiflr42VrnkY=</wsse:Password>"
        "<wsse:Nonce>eDKPl6N7r0dFwTW0mHZLvA==</wsse:Nonce>"
        "<wsu:Created>2017-01-22T05:09:55Z</wsu:Created>"
        "</wsse:UsernameToken>"
        "</wsse:Security>"
        "</s:Header>";
    return std::string("");
}

