#ifndef _CURLTOOL_H_
#define _CURLTOOL_H_

#include <string>
#include <list>
#include "CSocketIPv4.h"

class CUrlTool
{
public:
    enum enConnType {
        enAuto,
        enNone,
        enBasic,
        enDegist,
        enSSL,
    };
    enum enMethod {
        enGET,
        enPOST,
        enPUT,
        enDELETE,
    };
protected:
    int m_s32DbgLevel;
    std::string m_strHttpVersion;
    std::string m_strUser;
    std::string m_strPassword;
    int  m_s32Port;
    std::string m_strHost;
    std::string m_strMothod;

    std::list<std::string> m_lstHeader;
    enum enMethod m_enMethod;
    enum enConnType m_enConnType;

    std::string m_strUrl;
    std::string m_strformBody;
    std::string m_strResponseHeader;

    CSocketIPv4 *m_pSocket;
    void send_request();
    void recv_response();
    int recv_response_header();
    int recv_response_body(int s32RecvLen);
    std::string get_method()
    {
        switch(m_enMethod) {
        case enGET:
            return std::string("GET");
        case enPOST:
            return std::string("POST");
        case enPUT:
            return std::string("PUT");
        case enDELETE:
            return std::string("DEL");
        default:
            break;
        }
        return std::string("");
    }

public:
    std::string m_strResponse;
    CUrlTool();
    virtual ~CUrlTool();
    void set_host(std::string strHost,int s32Port=80)
    {
        m_strHost = strHost;
        m_s32Port = s32Port;
    }
    void set_author(std::string strUser,std::string strPassword)
    {
        m_strUser     = strUser;
        m_strPassword = strPassword;
    }
    void append_header(std::string strHeader)
    {
        m_lstHeader.push_back(strHeader);
    }
    void set_url(std::string strUrl)
    {
        m_strUrl = strUrl;
    }
    void clear_header()
    {
        m_lstHeader.clear();
    }
    void set_mothod(enum enMethod enMethod)
    {
        m_enMethod = enMethod;
    }
    void set_set_form_body(std::string strBody)
    {
        m_strformBody = strBody;
    }
    int perform();
    int termination();
    int lock() {}
    int unlock() {}
    std::string get_ws_username_token();
};
#endif
