//#include "stdafx.h"
#include "CDigestInfo.h"
#include "hParserResponseField.h"


CDigestInfo::CDigestInfo(void)
{
    init();
}


CDigestInfo::CDigestInfo(const CDigestInfo &rSrc)
{
    memcpy(pcAlg, rSrc.pcAlg, 32);
    memcpy(pcNonce, rSrc.pcNonce, 128);
    memcpy(pcNonceCount, rSrc.pcNonceCount, 32);
    memcpy(pcCNonce, rSrc.pcCNonce, 64);
    memcpy(pcRealm, rSrc.pcRealm, 64);
    memcpy(pcQop, rSrc.pcQop, 64);
    memcpy(pcMethod, rSrc.pcMethod, 32);

    iNonceCount         = rSrc.iNonceCount;
    memcpy(HA1, rSrc.HA1, HASHHEXLEN+1);
    memcpy(HA2, rSrc.HA2, HASHHEXLEN+1);
    memcpy(Response, rSrc.Response, HASHHEXLEN+1);

    bAvailable          = rSrc.bAvailable ;

}


CDigestInfo::~CDigestInfo(void)
{

}


void CDigestInfo::init(void)
{
    sprintf(pcAlg,"%s","md5");
    *pcNonce = '\0';
    *pcNonceCount = '\0';
    *pcCNonce = '\0';
    *pcRealm = '\0';
    *pcQop  = '\0';
    *pcMethod = '\0';
    *HA1 = '\0';
    *HA2 = '\0';
    *Response = '\0';
    iNonceCount = 1;
    bAvailable = false;
}


void CDigestInfo::setNonceCount(unsigned int i)
{
    iNonceCount = i;
}

void CDigestInfo::setMethod(char *pcData)
{
    sprintf(pcMethod,"%s",pcData);
}


bool CDigestInfo::IsAvailable(void)
{
    return bAvailable;
}


bool CDigestInfo::check(char *pcResp)
{
    char *pcFind = strstr(pcResp,"WWW-Authenticate: Digest");
    if(pcFind != NULL)
    {
        char pcBuf[256];
        pcBuf[0]='\0';
        pcRealm[0] = '\0';
        ParserResponseField(pcFind, "realm=\"", pcBuf,"\"");
        if(strlen(pcBuf) > 0)
            sprintf(pcRealm,"%s",pcBuf);
        pcBuf[0]='\0';
        pcQop[0] = '\0';
        ParserResponseField(pcFind, "qop=\"", pcBuf,"\"");
        if(strlen(pcBuf) > 0)
            sprintf(pcQop,"%s",pcBuf);
        pcBuf[0]='\0';
        pcNonce[0]= '\0';
        ParserResponseField(pcFind, "nonce=\"", pcBuf,"\"");
        if(strlen(pcBuf) > 0)
            sprintf(pcNonce,"%s",pcBuf);
        if(pcRealm[0] != '\0' && pcNonce != '\0')
            bAvailable = true;
        else
            bAvailable = false;
        iNonceCount = 1;

    }
    else
        bAvailable = false;

    return bAvailable;
}


void CDigestInfo::RTSPDigest(char *pcUser,char *pcPassword,char *pcPath,char *pcOutput)
{
    sprintf(pcNonceCount,"%08x",iNonceCount++);
    sprintf(pcCNonce,"%16x",iNonceCount);
    DigestCalcHA1(pcAlg,pcUser,pcRealm,pcPassword,pcNonce,pcCNonce,HA1);
    char *pcUri = new char[strlen(pcPath)+2];
    if(pcPath[0] == '/')
        sprintf(pcUri,"%s",pcPath);
    else
        sprintf(pcUri,"/%s",pcPath);
    DigestCalcResponse(HA1,pcNonce, pcNonceCount, pcCNonce, pcQop,pcMethod, pcUri, HA2, Response);
    sprintf(pcOutput,"Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"\r\n"
            //sprintf(pcOutput,"Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\", qop=%s, nc=%s, cnonce=\"%s\"\r\n"
            ,pcUser,pcRealm,pcNonce,pcUri,Response/*,pcQop,pcNonceCount,pcCNonce*/);
    delete [] pcUri;
}


void CDigestInfo::HTTPDigest(char *pcUser, char *pcPassword, char *pcPath, char *pcOutput)
{
    sprintf(pcNonceCount,"%08x",iNonceCount++);
    sprintf(pcCNonce,"%16x",iNonceCount);
    DigestCalcHA1(pcAlg,pcUser,pcRealm,pcPassword,pcNonce,pcCNonce,HA1);
    char *pcUri = new char[strlen(pcPath)+2];
    if(pcPath[0] == '/')
        sprintf(pcUri,"%s",pcPath);
    else
        sprintf(pcUri,"/%s",pcPath);
    DigestCalcResponse(HA1,pcNonce, pcNonceCount, pcCNonce, pcQop,pcMethod, pcUri, HA2, Response);
    //sprintf(pcOutput,"Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"\r\n"
    sprintf(pcOutput,"Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\", qop=%s, nc=%s, cnonce=\"%s\"\r\n"
            ,pcUser,pcRealm,pcNonce,pcUri,Response,pcQop,pcNonceCount,pcCNonce);

#ifdef _DEBUG
    int iLen = strlen(pcOutput);
#endif
    delete [] pcUri;
}


void CDigestInfo::CopyDigestInfo(CDigestInfo &rDst, const CDigestInfo &rSrc)
{
    memcpy(rDst.pcAlg, rSrc.pcAlg, 32);
    memcpy(rDst.pcNonce, rSrc.pcNonce, 128);
    memcpy(rDst.pcNonceCount, rSrc.pcNonceCount, 32);
    memcpy(rDst.pcCNonce, rSrc.pcCNonce, 64);
    memcpy(rDst.pcRealm, rSrc.pcRealm, 64);
    memcpy(rDst.pcQop, rSrc.pcQop, 64);
    memcpy(rDst.pcMethod, rSrc.pcMethod, 32);

    rDst.iNonceCount         = rSrc.iNonceCount;
    memcpy(rDst.HA1, rSrc.HA1, HASHHEXLEN+1);
    memcpy(rDst.HA2, rSrc.HA2, HASHHEXLEN+1);
    memcpy(rDst.Response, rSrc.Response, HASHHEXLEN+1);

    rDst.bAvailable          = rSrc.bAvailable ;
}


CDigestInfo & CDigestInfo::operator=(const CDigestInfo & rsh )
{
    if(this != &rsh)
    {
        CopyDigestInfo(*this, rsh);
    }

    return *this;
}


