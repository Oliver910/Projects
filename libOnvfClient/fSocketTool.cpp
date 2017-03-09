#include "fSocketTool.h"


const unsigned long SELECT_TIMEOUT_MSEC = 100;
bool fSocketTool::m_bSocketEnvInit = false;


//�غc�l
fSocketTool::fSocketTool(Protocol protocol)
    : m_StreamType		(InvalidType)
    , m_Protocol		(protocol)
    , m_errLastStatus	(NoError)
    , m_socket			(INVALID_SOCKET)
    , m_bBlocking       (0)
    , m_bAddressReusable(0)
    , m_u32Backlog      (0)
    , m_s32PeerPort		(0)
{
    m_strAddr     ="";
    m_strPeerAddr ="";
    m_s32Port = 0 ;
    m_bBroadcastMode = 0;

    m_nDestAddrLen = sizeof(struct sockaddr) ;
    memset(&m_socketDestAddr,0,m_nDestAddrLen);

    m_nSrcAddrLen = sizeof(struct sockaddr) ;
    memset(&m_socketSrcAddr,0,m_nSrcAddrLen);

}


//�Ѻc�l
fSocketTool::~fSocketTool (void)
{

}


//��SocketTool�إߥX�s��Socket�\��A�Ҧp�Aipv4������sockettool�N�|���ͷs��sockettool����
fSocketTool *fSocketTool::new_socket_tool(void)
{
    return NULL;
}


//�g��socketTool��new_socket_tool���ͪ�����A�ݭn��delete_socket_tool������O����
void fSocketTool::delete_socket_tool(fSocketTool *ptr)
{

}


//�إ�socket�A���P����socket(...)���O�A�إߥi��socket ()���osocket��Handle
int fSocketTool::create(void)
{
    return 0;
}


//����socket�A�p�P����closesocket
int fSocketTool::destory(void)
{
    shutdown(m_socket, 2);
    close(m_socket);
    m_socket=-1;
    return 0;
}


//Socket �g�Lcreate�Pattach_socket�A�Ҳ��ͪ�socket�O�_�w�g���ͦ��Ī�handle
int fSocketTool::is_valid (void) const
{
    return 0;
}

//�]�w��udp,��TCP
void fSocketTool::set_type(Type type)
{
}

//�^�Ǭ�udp,��TCP
fSocketTool::Type fSocketTool::type (void) const
{
    return m_StreamType;
}


//�ҫإߪ���k�A�p�Genum Protocol�ҩw�q
fSocketTool::Protocol fSocketTool::protocol (void) const
{
    return m_Protocol;
}


//���osocket handle
hTSocket_t fSocketTool::socket (void) const
{
    return 	m_socket;
}

//�]�w�~��socket�w�g�إߧ����̪���socktool���ɭ��C
void fSocketTool::attach_socket ( hTSocket_t socket, Type type )
{
}


//�^�ǬO�_��block�Ҧ��C
int fSocketTool::blocking (void) const
{
    return m_bBlocking;
}

//�]�w�O�_��block�Ҧ��C
void fSocketTool::set_blocking ( int enable )
{
    if(enable==0)
        m_bBlocking = 0;
    else
        m_bBlocking = 1;
}
int fSocketTool::address_reusable (void) const
{
    return m_bAddressReusable;
}


//�]�w���ШϥμҦ��A�άO�W���Ҧ�
void fSocketTool::set_address_reusable ( int enable )
{
    if(enable==0)
        m_bAddressReusable = 0;
    else
        m_bAddressReusable = 1;
}
//client�ݫإ߳s�u�A���w��}�Pport
int fSocketTool::connect ( std::string & addr, unsigned int port )
{
    return -1;
}


//�]�wServer���w�ɭ��Pport
int fSocketTool::bind ( std::string & address, unsigned int port )
{
    return 0;
}


//�]�wServer��ť�ӼơC
int fSocketTool::listen ( int backlog )
{
    return 0;
}


//Server Accept�A�ȡA���H�s�u�i�ӮɡA�|�ۤv����Sockettool�A�����ɻݨϥ�delete_socket_tool����C
fSocketTool *fSocketTool::accept (void)
{
    return NULL;
}


//�]�w�ǰe�P���������ݮɶ�
long fSocketTool::wait_for_more ( int msecs, int * timeout) const
{
    return 0;
}


//�ѳs�u��Ū�^�@�����
long fSocketTool::read_block ( char * data, HT_U32 maxlen, HT_U32 *pTimeOut)
{
    if(pTimeOut)
    {
        if(type() == Stream) {
            return RecvOnceFunc(socket(), (unsigned char *)data, maxlen, *pTimeOut);
        }
        else {
            return RecvFromOnceFunc(socket(),(unsigned char *)data,maxlen,*pTimeOut);
        }
    }
    else
    {
        if(type() == Stream) {
            return ::recv(socket(), data,maxlen,0);
        }
        else {
            return ::recvfrom(socket(),data,maxlen,0,&m_socketDestAddr,&m_nDestAddrLen);
        }
    }
}

//�ǰe�@�����
long fSocketTool::write_block ( const char * data, HT_U32 len, HT_U32 *pTimeOut)
{
    if(pTimeOut)
    {   //hDebug("SendBufFunc1\n");
        if(type() == Stream) {
            return SendBufFunc(socket(), data, len, *pTimeOut);
        }
        else {
            return SendToBufFunc(socket(), data, len, *pTimeOut);
        }
    }
    else
    {
        //hDebug("SendBufFunc2\n");
        if(type() == Stream) {
            return ::send( socket(), data,	len, 0 );
        }
        else {
#if 0
            struct sockaddr_in *adr = (struct sockaddr_in *)&m_socketDestAddr;
            hDebug("sin_family=%d\n",adr->sin_family);
            hDebug("sin_port=%d\n",adr->sin_port);
            hDebug("sin_addr:%s\n",inet_ntoa(adr->sin_addr));
            hDebug("socket():%d\n",socket());
            hDebug("data:%p\n",data);
            hDebug("len:%d\n",len);
#endif
            return ::sendto(socket(), data, len, 0,&m_socketDestAddr, m_nDestAddrLen);
        }
    }
}
long fSocketTool::read( char * data, HT_U32 maxlen, HT_U32 *pTimeOut)
{

    if(pTimeOut)
    {

        if(socket() == INVALID_SOCKET) {
            return SOCKET_ERROR;
        }
        if(data == NULL) {
            return -1;
        }

        int iRecvSize = 0;
        unsigned int ulTimeout = *pTimeOut;
        fd_set sReadSet, sErrorSet;
        timeval tTimeout;
        tTimeout.tv_sec = 0;
        tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
        int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
        int iTimeoutCount = 0;
        int iRes = 0;
        int totalRecvLen = 0;
        iTimeoutCount = 0;

        do
        {
            if(++iTimeoutCount > iTimeoutLimit) {
                return iRes;
            }

            FD_ZERO(&sReadSet);
            FD_SET(socket(), &sReadSet);
            FD_ZERO(&sErrorSet);
            FD_SET(socket(), &sErrorSet);

            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;

            iRes = select(socket()+1, &sReadSet, NULL, &sErrorSet, &tTimeout );
            if( iRes > 0 ) {
                if(FD_ISSET(socket(), &sReadSet))
                    break;
                if(FD_ISSET(socket(), &sErrorSet)) {
                    return SOCKET_ERROR;
                }
            }
            else if( iRes == 0 ) {
                continue;
            }
            else {
                return SOCKET_ERROR;
            }
        } while(TRUE);

        if(type() == Stream) {
            iRecvSize = ::recv(socket(),(char *)data, maxlen, 0);
        }
        else {
            iRecvSize = ::recvfrom(socket(),data,maxlen,0,&m_socketDestAddr,&m_nDestAddrLen);
        }
        if(iRecvSize < 0) {
            return SOCKET_ERROR;
        }
        return iRecvSize;

    }
    else
    {
        if(type() == Stream) {
            return ::recv(socket(), data,maxlen,0);
        }
        else {
            return ::recvfrom(socket(),data,maxlen,0,&m_socketDestAddr,&m_nDestAddrLen);
        }
    }

}
long fSocketTool::write( const char * data, HT_U32 len, HT_U32 *pTimeOut)
{
    if(pTimeOut) {
        if(socket() == INVALID_SOCKET)
            return SOCKET_ERROR;
        if(data == NULL)
            return -1;
        unsigned int ulTimeout = *pTimeOut;
        fd_set sWriteSktSet, sErrSktSet;
        timeval tTimeout;
        tTimeout.tv_sec = 0;
        tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
        int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
        int iTimeoutCount = 0;

        int iRetValue;
        unsigned long ulSentLen = 0;
        iTimeoutCount = 0;
        do
        {
            if(++iTimeoutCount > iTimeoutLimit)
                return SOCKET_ERROR;//IPCAM_CGI_SEND_TIMEOUT_ER;

            FD_ZERO(&sWriteSktSet);
            FD_SET (socket(), &sWriteSktSet);
            FD_ZERO(&sErrSktSet);
            FD_SET (socket(), &sErrSktSet);

            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
            iRetValue = select(socket()+1, NULL, &sWriteSktSet, &sErrSktSet, &tTimeout);
            if( iRetValue > 0 )
            {
                if(FD_ISSET(socket(), &sErrSktSet))
                    return SOCKET_ERROR;
                else
                    break;
            }
            else if( iRetValue == 0 )
            {
                continue;
            }
            else// if( iRetValue == SOCKET_ERROR )
                return SOCKET_ERROR;
        } while(TRUE);

        if(type() == Stream) {
            iRetValue = ::send(socket(), data, len, 0);
        } else {
            iRetValue = ::sendto(socket(), data, len, 0,&m_socketDestAddr, m_nDestAddrLen);
        }
        if(iRetValue == SOCKET_ERROR || iRetValue < 0)
            return SOCKET_ERROR;//IPCAM_CGI_SEND_ER;

        return iRetValue;
    }
    else
    {
        if(type() == Stream) {
            return ::send( socket(), data,	len, 0 );
        }
        else {
            return ::sendto(socket(), data, len, 0,&m_socketDestAddr, m_nDestAddrLen);
        }
    }

}


//���a�ݪ�port
unsigned int fSocketTool::port (void) const
{
    return m_s32Port;
}


//���誺port
unsigned int fSocketTool::peerPort (void) const
{
    return m_s32PeerPort;
}


//���a�誺��}
std::string fSocketTool::address (void) const
{
    return m_strAddr;
}


//���誺��}
std::string fSocketTool::peer_address (void) const
{

    //return m_strPeerAddr;
    std::string strPeerAddr;
    struct sockaddr_in addr;
    int fd = socket();
    socklen_t addr_len = sizeof(addr);	// this is what's missing
    int ret = getpeername(fd,(struct sockaddr*) &addr, &addr_len);
    //hDebug("check_block ret = %d %s\n",ret,inet_ntoa(addr.sin_addr));
    if(ret ==0) {
        strPeerAddr = std::string(inet_ntoa(addr.sin_addr));
    }
    return strPeerAddr;
}


//���o�̫���~�X�^��
fSocketTool::Error fSocketTool::error (void) const
{
    return NoError;
}



// init for socket library
bool fSocketTool::Init(void)
{
    if(!m_bSocketEnvInit)
    {
        m_bSocketEnvInit = true;
    }
    return true;
}


// deinit for socket library
void fSocketTool::DeInit(void)
{
    if(m_bSocketEnvInit)
    {
        // do something to deinit socket library
        m_bSocketEnvInit = false;
    }
}


int fSocketTool::RecvBufFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32 RecvSize, HT_U32 ulTimeout)
{
    if(mSocket == INVALID_SOCKET)
        return SOCKET_ERROR;
    if(buf == NULL)
        return -1;
    if(RecvSize == 0)
        return 0;

    HT_U32 recvSize_temp = 0;
    int iRecvSize = 0;

    fd_set sReadSet;
    timeval tTimeout;
    tTimeout.tv_sec = 0;
    tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
    int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
    int iTimeoutCount = 0;
    int iRes = 0;

    fd_set sErrorSet;

    while(recvSize_temp < RecvSize)
    {
        iTimeoutCount = 0;
        do
        {
            if(ulTimeout != 0 && ++iTimeoutCount > iTimeoutLimit)
                return iRes;

            FD_ZERO(&sReadSet);
            FD_SET(mSocket, &sReadSet);

            FD_ZERO(&sErrorSet);
            FD_SET(mSocket, &sErrorSet);

            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
            iRes = select(mSocket+1, &sReadSet, NULL, &sErrorSet, &tTimeout );

            if( iRes > 0 )
            {
                if(FD_ISSET(mSocket, &sReadSet))
                    break;
                if(FD_ISSET(mSocket, &sErrorSet))
                    return SOCKET_ERROR;
            }
            else if( iRes == 0 )
                continue;
            else //if( iRes == SOCKET_ERROR )
                return SOCKET_ERROR;
        } while(TRUE);

        iRecvSize = recv(mSocket,(char *)(buf+recvSize_temp), RecvSize - recvSize_temp, 0);
        if(iRecvSize == 0)
        {
            return (recvSize_temp == 0)?SOCKET_ERROR:recvSize_temp;
        }
        else if(iRecvSize < 0)
        {
            return SOCKET_ERROR;
        }

        iTimeoutCount = 0;
        recvSize_temp += iRecvSize;
    }

    return recvSize_temp;
}


int fSocketTool::RecvOnceFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32 uiBufSize, HT_U32 ulTimeout)
{

    if(mSocket == INVALID_SOCKET)
    {
        return SOCKET_ERROR;
    }
    if(buf == NULL)
    {
        return -1;
    }

    int iRecvSize = 0;

    fd_set sReadSet, sErrorSet;
    timeval tTimeout;
    tTimeout.tv_sec = 0;
    tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
    int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
    int iTimeoutCount = 0;
    int iRes = 0;

    int totalRecvLen = 0;

    do//while(recvSize_temp < uiBufSize)
    {
        iTimeoutCount = 0;
        do
        {
            if(++iTimeoutCount > iTimeoutLimit)
            {
                return iRes;
            }

            FD_ZERO(&sReadSet);
            FD_SET(mSocket, &sReadSet);

            FD_ZERO(&sErrorSet);
            FD_SET(mSocket, &sErrorSet);

            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
            //iRes = select(1, &sReadSet, NULL, NULL, &tTimeout );
            iRes = select(mSocket+1, &sReadSet, NULL, &sErrorSet, &tTimeout );
            if( iRes > 0 )
            {
                if(FD_ISSET(mSocket, &sReadSet))
                    break;
                if(FD_ISSET(mSocket, &sErrorSet))
                {
                    return SOCKET_ERROR;
                }
            }
            else if( iRes == 0 )
            {
                continue;
            }
            else //if( iRes == SOCKET_ERROR )
            {
                return SOCKET_ERROR;
            }
        } while(TRUE);

        iRecvSize = recv(mSocket,(char *)buf + totalRecvLen, uiBufSize-totalRecvLen, 0);

        if(iRecvSize < 0)
        {
            return SOCKET_ERROR;
        } else if (iRecvSize==0) {

            break;
        }
//     	hDebug("recv size = %d\n", iRecvSize);
        totalRecvLen += iRecvSize;


    } while(totalRecvLen < uiBufSize);

    return totalRecvLen;
}

int fSocketTool::RecvFromOnceFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32 uiBufSize, HT_U32 ulTimeout)
{
    if(mSocket == INVALID_SOCKET)
    {
        return SOCKET_ERROR;
    }
    if(buf == NULL)
    {
        return -1;
    }

    int iRecvSize = 0;

    fd_set sReadSet, sErrorSet;
    timeval tTimeout;
    tTimeout.tv_sec = 0;
    tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
    int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
    int iTimeoutCount = 0;
    int iRes = 0;

    int currentSize = 0;
    //while(recvSize_temp < uiBufSize)
    do
    {
        iTimeoutCount = 0;
        do
        {
            if(++iTimeoutCount > iTimeoutLimit)
            {
                //hDebug("iRes = %d iTimeoutCount=%d iTimeoutLimit =%d\n",iRes,iTimeoutCount,iTimeoutLimit);
                return iRes;
            }

            FD_ZERO(&sReadSet);
            FD_SET(mSocket, &sReadSet);

            FD_ZERO(&sErrorSet);
            FD_SET(mSocket, &sErrorSet);

            //iRes = select(1, &sReadSet, NULL, NULL, &tTimeout );
            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
            iRes = select(mSocket+1, &sReadSet, NULL, &sErrorSet, &tTimeout );
            //hDebug("iRes = %d iTimeoutCount=%d iTimeoutLimit =%d\n",iRes,iTimeoutCount,iTimeoutLimit);
            if( iRes > 0 )
            {
                if(FD_ISSET(mSocket, &sReadSet))
                    break;
                if(FD_ISSET(mSocket, &sErrorSet))
                {
                    return SOCKET_ERROR;
                }
            }
            else if( iRes == 0 )
            {
                continue;
            }
            else //if( iRes == SOCKET_ERROR )
            {
                return SOCKET_ERROR;
            }
        } while(TRUE);

        iRecvSize = recvfrom(mSocket,(char *)buf + currentSize, uiBufSize - currentSize, 0,&m_socketSrcAddr,&m_nSrcAddrLen);

        if(iRecvSize < 0)
            return SOCKET_ERROR;
        else if(iRecvSize == 0)
            break;

        currentSize += iRecvSize;

        if(iRecvSize < 0)
        {
            return SOCKET_ERROR;
        }

    } while(currentSize < uiBufSize);

    return currentSize;
}


int fSocketTool::SendBufFunc(hTSocket_t mSocket, const char *buf, HT_U32 uiBufSize, HT_U32 ulTimeout)
{
    if(mSocket == INVALID_SOCKET)
        return SOCKET_ERROR;
    if(buf == NULL)
        return -1;

    fd_set sWriteSktSet, sErrSktSet;
    timeval tTimeout;
    tTimeout.tv_sec = 0;
    tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
    int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
    int iTimeoutCount = 0;

    int iRetValue;
    unsigned long ulCmdLen = uiBufSize;
    unsigned long ulSentLen = 0;
    do
    {
        iTimeoutCount = 0;
        do
        {
            if(++iTimeoutCount > iTimeoutLimit)
                return SOCKET_ERROR;//IPCAM_CGI_SEND_TIMEOUT_ER;

            FD_ZERO(&sWriteSktSet);
            FD_SET (mSocket, &sWriteSktSet);
            FD_ZERO(&sErrSktSet);
            FD_SET (mSocket, &sErrSktSet);

            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
            iRetValue = select(mSocket+1, NULL, &sWriteSktSet, &sErrSktSet, &tTimeout);
            if( iRetValue > 0 )
            {
                if(FD_ISSET(mSocket, &sErrSktSet))
                    return SOCKET_ERROR;
                else
                    break;
            }
            else if( iRetValue == 0 )
            {
                continue;
            }
            else// if( iRetValue == SOCKET_ERROR )
                return SOCKET_ERROR;
        } while(TRUE);

        iRetValue = send(mSocket, buf+ulSentLen, ulCmdLen-ulSentLen, 0);

        if(iRetValue == SOCKET_ERROR || iRetValue < 0)
            return SOCKET_ERROR;//IPCAM_CGI_SEND_ER;
        else if(iRetValue == 0)
            break;
        ulSentLen += iRetValue;
    } while(ulSentLen < ulCmdLen);

    return ulSentLen;
}

int fSocketTool::SendToBufFunc(hTSocket_t mSocket, const char *buf, HT_U32 uiBufSize, HT_U32 ulTimeout)
{
    if(mSocket == INVALID_SOCKET)
        return SOCKET_ERROR;
    if(buf == NULL)
        return -1;

    fd_set sWriteSktSet, sErrSktSet;
    timeval tTimeout;
    tTimeout.tv_sec = 0;
    tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
    int iTimeoutLimit = ulTimeout / SELECT_TIMEOUT_MSEC + 1;
    int iTimeoutCount = 0;

    int iRetValue;
    unsigned long ulCmdLen = uiBufSize;
    unsigned long ulSentLen = 0;
    do
    {
        iTimeoutCount = 0;
        do
        {
            if(++iTimeoutCount > iTimeoutLimit)
                return SOCKET_ERROR;//IPCAM_CGI_SEND_TIMEOUT_ER;

            FD_ZERO(&sWriteSktSet);
            FD_SET(mSocket, &sWriteSktSet);
            FD_ZERO(&sErrSktSet);
            FD_SET(mSocket, &sErrSktSet);

            tTimeout.tv_sec = 0;
            tTimeout.tv_usec = SELECT_TIMEOUT_MSEC * 1000;
            iRetValue = select(mSocket+1, NULL, &sWriteSktSet, &sErrSktSet, &tTimeout);
            if( iRetValue > 0 )
            {
                if(FD_ISSET(mSocket, &sErrSktSet))
                    return SOCKET_ERROR;
                else
                    break;
            }
            else if( iRetValue == 0 )
                continue;
            else// if( iRetValue == SOCKET_ERROR )
                return SOCKET_ERROR;
        } while(TRUE);


        iRetValue = sendto(mSocket, buf+ulSentLen, ulCmdLen-ulSentLen, SOCKET_SEND_DEFAULT_FLAG, &m_socketDestAddr, m_nDestAddrLen);

        if(iRetValue == SOCKET_ERROR || iRetValue < 0)
            return SOCKET_ERROR;//IPCAM_CGI_SEND_ER;
        else if(iRetValue == 0)
            break;
        ulSentLen += iRetValue;
    } while(ulSentLen < ulCmdLen);

    return ulSentLen;
}
void fSocketTool::set_broadcast_mode ( int enable )
{
    if(enable==0) {
        m_bBroadcastMode = 0;
    }
    else {
        m_bBroadcastMode = 1;
    }

}
int fSocketTool::is_broadcast_mode()
{
    return m_bBroadcastMode;
}

int fSocketTool::set_udp_dest_addr(std::string strDest_addr, unsigned int port)
{
    return -1;
}
int fSocketTool::set_udp_dest_addr(const struct sockaddr *dest_addr, socklen_t addrlen)
{
    m_nDestAddrLen = addrlen ;
    memcpy(&m_socketDestAddr,dest_addr,m_nDestAddrLen);
    return 0;
}
int fSocketTool::set_udp_src_addr(std::string strSrc_addr, unsigned int port)
{
    return -1;
}
int fSocketTool::set_udp_src_addr(const struct sockaddr *src_addr, socklen_t addrlen)
{
    m_nSrcAddrLen = addrlen ;
    memcpy(&m_socketSrcAddr,src_addr,m_nSrcAddrLen);
    return 0;
}
int fSocketTool::add_membership(std::string strInterface, std::string strMultiAddr)
{
    hTRACE("fSocketTool::add_membership 1 not support\n");
    return -1;
}
int fSocketTool::add_membership(struct in_addr *addrInterface, struct in_addr *addrMultiAddr)
{
    hTRACE("fSocketTool::add_membership 2 not support\n");
    return -1;
}
int fSocketTool::set_timeout(enum Direct sockDir,HT_U32 u32mSec)
{
    if(socket()!=INVALID_SOCKET) {
        struct timeval timeout;
        timeout.tv_sec  =  (u32mSec/1000);
        timeout.tv_usec =  (u32mSec %1000)*1000;

        if((sockDir & DIR_RECV)!=0) {
            if (setsockopt (socket(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
                hLog("setsockopt failed\n");
        }

        if((sockDir & DIR_SEND)!=0) {
            if (setsockopt (socket(), SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
                hLog("setsockopt failed\n");
        }
    }
    return sockDir;
}


int fSocketTool::ping_for_autoget(char *host,int nEthernetNum)
{
    printf("fSocketTool::ping_for_autoget\n");
    return -1;
}


int fSocketTool::in_cksum_for_autoget(unsigned short *buf, int sz)
{
    int nleft = sz;
    int sum = 0;
    unsigned short *w = buf;
    unsigned short ans = 0;
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *) (&ans) = *(unsigned char *) w;
        sum += ans;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    ans = ~sum;

    return (ans);
}


