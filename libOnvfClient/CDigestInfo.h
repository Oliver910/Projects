#ifndef _CDIGESTINFO_H_
#define _CDIGESTINFO_H_

#include "fSocketTool.h"
#include "auth/digcalc.h"


class LIBIPCAMNET_CLASS CDigestInfo {

private:
    char pcAlg[32];
    char pcNonce[128];
    char pcNonceCount[32];
    char pcCNonce[64];
    char pcRealm[64];
    char pcQop[64];
    char pcMethod[32];
    unsigned int iNonceCount;
    HASHHEX HA1;
    HASHHEX HA2;
    HASHHEX Response;
    bool bAvailable;

protected:
    void init(void);

public:
    CDigestInfo(void);
    CDigestInfo(const CDigestInfo &);
    virtual ~CDigestInfo(void);

    bool check(char *pcResp);
    void setNonceCount(unsigned int i);
    void setMethod(char *pcData);
    bool IsAvailable(void);


    void RTSPDigest(char *pcUser,char *pcPassword,char *pcPath,char *pcOutput);
    void HTTPDigest(char *pcUser, char *pcPassword, char *pcPath, char *pcOutput);

    CDigestInfo & operator=(const CDigestInfo & rsh );
    static void CopyDigestInfo(CDigestInfo &rDst, const CDigestInfo &rSrc);


};    // end of class CDigestInfo{

#endif    // end of #ifndef _HDIESTINFO_H_

