
#include "CSocketIPv4.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <net/if.h>            // for IFNAMSIZ & IFHWADDRLEN
#include <errno.h>
#include <sys/ioctl.h>

struct timeval gTimeout;

CSocketIPv4::CSocketIPv4(Type type)
    :fSocketTool (IPv4)
{
    m_socket = -1;
    m_bAttachSocket = 0;
    m_StreamType=type;
}
CSocketIPv4::~CSocketIPv4()
{

}
fSocketTool *CSocketIPv4::new_socket_tool(void)
{
    return new CSocketIPv4();
}

void CSocketIPv4::delete_socket_tool(fSocketTool *ptr)
{
    CSocketIPv4 *del_ptr = (CSocketIPv4 *)ptr;
    if(del_ptr!=NULL) {
        delete del_ptr;
    }
}

int CSocketIPv4::create()
{
    try {
        hTSocket_t s;
        if(m_StreamType==Stream) {
            if ((s = ::socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                perror("SOCK_STREAM:socket");
                throw InternalError;
            }
        }
        else {
            if ((s = ::socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("SOCK_DGRAM:socket");
                throw InternalError;
            }
            if(is_broadcast_mode()!=0) {
                int broadcastFlag = 1;
                int broad_sock = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastFlag, sizeof(broadcastFlag));
                if(broad_sock < 0) {
                    hLog("ERROR:  setsockopt( SO_BROADCAST ) = %d \n", errno);
                    destory();
                    return -1;
                }
            }
        }
        /*int sock_error;
        struct timeval timeout;

        timeout.tv_sec  = 3;
        timeout.tv_usec = 0;
        sock_error=setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        if(sock_error<0)
        	return -1;*/

        /*sock_error=setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        if(sock_error<0)
        	return -1;*/

        m_socket = s;
        m_bAttachSocket = 0;
        return 0;
    }
    catch (enum Error Err) {
        m_errLastStatus = Err;
        hLog("CHtSocketIPv4::create fail => m_StreamType(%d) !",m_StreamType);
    }
    return -1;
}

int CSocketIPv4::destory()
{
    if(m_socket >= 0) {
        shutdown(m_socket,SHUT_RDWR);
        close(m_socket);
        m_socket = -1;
        m_bAttachSocket = 0;
    }
    return 0;
}
int CSocketIPv4::is_valid () const
{
    if(m_socket >= 0) {
        return 1;
    }
    else
        return 0;
}
void CSocketIPv4::set_type(Type type)
{
    m_StreamType = 	type;
}

void CSocketIPv4::attach_socket ( hTSocket_t socket, Type type )
{
    m_socket     = socket;
    m_StreamType = type;
    m_bAttachSocket = 1;
}

void CSocketIPv4::set_blocking ( int enable )
{
    m_bBlocking = enable;
}

int CSocketIPv4::connect ( std::string & strAddr, unsigned int port )
{
    struct sockaddr_in addr;
    int status = 0;
    struct addrinfo hints,*result,*rp;

    if(strAddr.size()==0 || port < 0 )
        return -1;

    memset(&addr, 0, sizeof(addr));
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags =0;
    hints.ai_family=AF_INET;

    //strAddr="tw.yahoo.com";
    status=getaddrinfo(strAddr.c_str(),NULL,&hints,&result);
    if(status!=0) { // 表示無法取得DNS位址
        //hDebug("CSocketIPv4::connect--A\n");
        addr.sin_addr.s_addr = inet_addr(strAddr.c_str());
        if(addr.sin_addr.s_addr==INADDR_NONE) {
            hLog("invalid addr\n. = %s\n",strAddr.c_str());
            return -1;
        }
        addr.sin_family = PF_INET;
        addr.sin_port  = htons(port);


        gTimeout.tv_sec  = 1;
        gTimeout.tv_usec = 0;
        setsockopt(socket(),SOL_SOCKET,SO_RCVTIMEO,&gTimeout,sizeof(gTimeout));
        setsockopt(socket(),SOL_SOCKET,SO_SNDTIMEO,&gTimeout,sizeof(gTimeout));

        if(::connect( socket(), (struct sockaddr *)&addr, sizeof(addr))!=-1) {
            hDebug("connect success ip:%s\n",strAddr.c_str());
            return 0;
        }
        else
        {
            hDebug("connect fail timeout ip:%s\n",strAddr.c_str());
            return -1;
        }
    }
    else { //表示由DNS取得實體位址
        for (rp = result; rp != NULL; rp = rp->ai_next) {


            struct  sockaddr_in *remote_addr =(struct  sockaddr_in *)rp->ai_addr;
            remote_addr->sin_family = PF_INET;
            remote_addr->sin_port  = htons(port);
            //printf("A=%s P=%d\n",inet_ntoa(remote_addr->sin_addr),remote_addr->sin_port);
            gTimeout.tv_sec  = 1;
            gTimeout.tv_usec = 0;
            setsockopt(socket(),SOL_SOCKET,SO_RCVTIMEO,&gTimeout,sizeof(gTimeout));
            setsockopt(socket(),SOL_SOCKET,SO_SNDTIMEO,&gTimeout,sizeof(gTimeout));

            if (::connect(socket(),(sockaddr*) rp->ai_addr, rp->ai_addrlen) != -1) {
                /* Success */
                freeaddrinfo(result);
                //printf("OK\n");
                // hDebug("connect success ip:%s\n",strAddr.c_str());
                return 0;
            }
            else
            {
                freeaddrinfo(result);
                hDebug("connect fail timeout ip:%s\n",strAddr.c_str());
                return -1;
            }
        }
        freeaddrinfo(result);
    }
    return -1;
}

int CSocketIPv4::bind ( std::string & address, unsigned int port )
{
    int nREUSEADDR = 1;
    struct sockaddr_in saun;
    int s32RetTry = 5;
    memset(&saun, 0, sizeof(saun));
    // printf("bind port port = %d\n",port);
    saun.sin_family = PF_INET;
    saun.sin_port	= htons(port);
    saun.sin_addr.s_addr=htonl(INADDR_ANY);

    while(s32RetTry--) {
        if ( setsockopt( socket(), SOL_SOCKET, SO_REUSEADDR, (char*) &nREUSEADDR, sizeof(nREUSEADDR) ) < 0 ) {
            hLog("ERROR:  setsockopt(SO_REUSEADDR) failed, errno = %d \n", errno);
            return 2;
        }

        if (::bind(socket(), (struct sockaddr *)&saun, sizeof(saun)) < 0) {
            perror("CHtSocketIPv4::bind");
            hLog("CHtSocketIPv4::bind ==> fail!");
            //return 1;
            sleep(1);
        }
        else
            break;

    }
    return 0;
}

int CSocketIPv4::listen ( int backlog )
{
    hDebug("CHtSocketIPv4::listen ==> %d\n",backlog);
    if (::listen(socket(), backlog) < 0) {
        perror("CHtSocketIPv4: listen");
        hLog("CHtSocketIPv4::listen ==> fail!");
        return 1;
    }
    return 0;
}

fSocketTool *CSocketIPv4::accept ()
{
    struct sockaddr_in fsaun;
    socklen_t fromlen = sizeof(fsaun);
    int ns=-1;
    if ((ns = ::accept(socket(), (sockaddr*)&fsaun,(socklen_t*) &fromlen)) < 0) {
        hDebug("socket()=%d ns=%d\n",socket(),ns);
        perror("CHtSocketIPv4: accept ");
        return NULL;
    }

    CSocketIPv4 *pRemoteSocket = new CSocketIPv4;
    pRemoteSocket->attach_socket ( ns, type());
    memcpy(&pRemoteSocket->m_fsaun,&fsaun,fromlen);
    pRemoteSocket->m_fromlen = fromlen;
    return (fSocketTool *)pRemoteSocket;
}
long CSocketIPv4::wait_for_more ( int msecs, int * timeout  ) const
{
    return 0;
}

int CSocketIPv4::set_udp_dest_addr(std::string strAddr, unsigned int port)
{
    struct sockaddr_in addr;
    int status = 0;
    struct addrinfo hints,*result,*rp;

    if(strAddr.size()==0 || port < 0 )
        return -1;

    memset(&addr, 0, sizeof(addr));
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags =0;
    hints.ai_family=AF_INET;
    //strAddr="tw.yahoo.com";
    status=getaddrinfo(strAddr.c_str(),NULL,&hints,&result);
    if(status!=0) { // 表示無法取得DNS位址
        //hDebug("CSocketIPv4::connect--A\n");
        addr.sin_addr.s_addr = inet_addr(strAddr.c_str());
        if(addr.sin_addr.s_addr==INADDR_NONE) {
            hLog("invalid addr\n. = %s\n",strAddr.c_str());
            return -1;
        }
        addr.sin_family = PF_INET;
        addr.sin_port  = htons(port);
        fSocketTool::set_udp_dest_addr((const struct sockaddr *)&addr, sizeof(addr));
    }
    else { //表示由DNS取得實體位址
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            struct  sockaddr_in *remote_addr =(struct  sockaddr_in *)rp->ai_addr;
            remote_addr->sin_family = PF_INET;
            if(remote_addr->sin_family ==PF_INET) {
                remote_addr->sin_port  = htons(port);
                fSocketTool::set_udp_dest_addr((const struct sockaddr *) rp->ai_addr, rp->ai_addrlen);
                //printf("A=%s P=%d\n",inet_ntoa(remote_addr->sin_addr),remote_addr->sin_port);
                break;
            }

        }
        freeaddrinfo(result);
    }
    return 0;
}

int CSocketIPv4::set_udp_src_addr(std::string strAddr, unsigned int port)
{
    struct sockaddr_in addr;
    int status = 0;
    struct addrinfo hints,*result,*rp;

    if(strAddr.size()==0 || port < 0 )
        return -1;

    memset(&addr, 0, sizeof(addr));
    memset(&hints, 0, sizeof(hints));

    hints.ai_flags =0;
    hints.ai_family=AF_INET;

    //strAddr="tw.yahoo.com";
    status=getaddrinfo(strAddr.c_str(),NULL,&hints,&result);
    if(status!=0) { // 表示無法取得DNS位址
        //hDebug("CSocketIPv4::connect--A\n");
        addr.sin_addr.s_addr = inet_addr(strAddr.c_str());
        if(addr.sin_addr.s_addr==INADDR_NONE) {
            hLog("invalid addr\n. = %s\n",strAddr.c_str());
            return -1;
        }
        addr.sin_family = PF_INET;
        addr.sin_port  = htons(port);
        fSocketTool::set_udp_src_addr((const struct sockaddr *)&addr, (socklen_t)sizeof(addr));
        return 0;
    }
    else { //表示由DNS取得實體位址
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            struct  sockaddr_in *remote_addr =(struct  sockaddr_in *)rp->ai_addr;
            remote_addr->sin_family = PF_INET;
            if(remote_addr->sin_family ==PF_INET) {
                remote_addr->sin_port  = htons(port);
                fSocketTool::set_udp_src_addr((const struct sockaddr *) rp->ai_addr, (socklen_t)rp->ai_addrlen);
                freeaddrinfo(result);
                return 0;
            }
            //printf("A=%s P=%d\n",inet_ntoa(remote_addr->sin_addr),remote_addr->sin_port);
        }
        freeaddrinfo(result);
    }
    return -1;
}
int CSocketIPv4::add_membership(std::string strInterface, std::string strMultiAddr)
{
    struct in_addr addrInterface;
    struct in_addr addrMultiAddr;
    addrInterface.s_addr = inet_addr(strInterface.c_str());
    addrMultiAddr.s_addr = inet_addr(strMultiAddr.c_str());
    return add_membership(&addrInterface, &addrMultiAddr);

}
int CSocketIPv4::add_membership(struct in_addr *addrInterface, struct in_addr *addrMultiAddr)
{
    int nRet;
    struct ip_mreq mcastAddr;
    memset((void *)&mcastAddr, 0, sizeof(struct ip_mreq));
    memcpy(&mcastAddr.imr_interface,addrInterface,sizeof(struct in_addr));
    memcpy(&mcastAddr.imr_multiaddr,addrMultiAddr,sizeof(struct in_addr));
    nRet = setsockopt(socket(), IPPROTO_IP, IP_ADD_MEMBERSHIP,(char *)&mcastAddr, sizeof(struct ip_mreq));
    if (nRet < 0) {
        //printf("ERROR:	setsockopt( IP_ADD_MEMBERSHIP ) = %d \n", errno);
        perror("ERROR:	setsockopt( IP_ADD_MEMBERSHIP ) =>");
    }
    return nRet;
}


