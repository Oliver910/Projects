#ifndef _FSOCKETTOOL_H_
#define _FSOCKETTOOL_H_




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

#include <string>

#include "hPlatform.h"
#include "hDebug.h"
//#include "hIPCamConfig.h"

#ifdef BUILDING_ON_MSVC
#ifdef LIBIPCAMNET_STATICLIB
#define LIBIPCAMNET_API 	extern "C"
#define LIBIPCAMNET_CLASS
#elif defined LIBIPCAMNET_EXPORTS
#define LIBIPCAMNET_API 	extern "C" __declspec(dllexport)
#define LIBIPCAMNET_CLASS  __declspec(dllexport)
#else
#define LIBIPCAMNET_API 	extern "C" __declspec(dllimport)
#define LIBIPCAMNET_CLASS  __declspec(dllimport)
#endif

#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET hTSocket_t;
#define SOCKET_SEND_DEFAULT_FLAG 0
//#define INVALID_SOCKET  (SOCKET)(~0)        // defined on winsock2.h

#define CGI_RESPONSE_BUF_SIZE 32768 /*20480*/  // 20110914  20480 -> 32768 (Truen TCAM-570 �R�^�Ӫ��ȴN���F 23K "Content-Length: 23194")
#define COMMAND_BUF_SIZE      8192
#else
#define LIBIPCAMNET_API 	extern "C"
#define LIBIPCAMNET_CLASS

#include <sys/socket.h>
typedef int hTSocket_t;
#define INVALID_SOCKET (-1)

#ifndef SOCKET_ERROR
#define SOCKET_ERROR   (-1)
#endif

#define SOCKET_SEND_DEFAULT_FLAG MSG_NOSIGNAL
#define CGI_RESPONSE_BUF_SIZE 32768 /*20480*/  // 20110914  20480 -> 32768 (Truen TCAM-570 �R�^�Ӫ��ȴN���F 23K "Content-Length: 23194")
#define COMMAND_BUF_SIZE      8192
#endif //end ifdef ifdef BUILDING_ON_MSVC

#define SOCKET_TOOL_INADDR_ANY "INADDR_ANY"

enum HTTP_REQUEST_METHOD {
    POST = 0
    ,   GET
    ,   PUT
    ,	DELETE
    ,   __HTTP_REQUEST_METHOD_BOUNDARY = 0xFFFFFFFF
};

//LIBIPCAMNET_API int test(void);

/*! \class fSocketTool
 *
 *   \brief ����Socket�ϥΤ�k���зǬɭ��C
 *   ����Socket����¦���O�A�i�H�ϥγo�����O���зǬɭ��A�ӹ�@IPv4, IPv6, IPC ,IPv4SSL,IPv6SSL....���P�T��w�C
 *   �N�ӹB�Ω�IPCamModule���A�i�H��{�bwindows�Plinux���x�����C
 *
 */
class LIBIPCAMNET_CLASS fSocketTool
{
public:
    enum  Type     {
        Stream      ///TCP���A
        ,Datagram    ///UDP���A
        ,InvalidType ///�L������
    };

    enum  Protocol {
        DUMMY       ///�����Ũ�w�C
        , IPv4        ///IPv4���q�T��w�C
        , IPv6        ///IPv6���q�T��w�C
        , IPC         ///Unix Socket�q�T��w�C
        , IPv4SSL     ///SSL over IPv4���q�T��k�C
        , IPv6SSL     ///SSL over IPv6���q�T��k�C
    };

    enum  Error {
        NoError                           /** ���o�Ϳ��~���P��Success  */
        , Success = NoError               /** ���\                     */
        , AlreadyBound                    /** �w�g�������w             */
        , Inaccessible                    /** �L�k�s���i�঳�������� */
        , NoResources                     /** �L�k���o�귽             */
        , InternalError                   /** �������~���P��Bug        */
        , Bug = InternalError             /** �{��Bug                  */
        , Impossible                      /** ���i��o��               */
        , NoFiles                         /** Socket�ɮ׫��в��`       */
        , ConnectionRefused               /** �s�u�Q�ڨM�C             */
        , NetworkFailure                  /** �����˸m����             */
        , UnknownError 		              /** �����������~             */
    };
    enum Direct {
        DIR_NONE=0,
        DIR_RECV=1,
        DIR_SEND=2,
        DIR_BOTH=3,
    };
protected:
    /**�O��Stream(TCP)/UDP����/ */
    Type         m_StreamType;
    /**�O���q�T��w����IPv4,IPv6......*/
    Protocol     m_Protocol;
    /**�O���̫�o�Ϳ��~����T*/
    Error        m_errLastStatus;
    /**�O��Socket���ɮ׫��СC*/
    hTSocket_t   m_socket;
    /**�O��Socket�O�_��Block���A�C*/
    int          m_bBlocking;
    /**�O��Socket�O�_�]�w�����и귽����*/
    int          m_bAddressReusable;
    /**�O��Socket�O�_��BroadCastMode*/
    int          m_bBroadcastMode;
    /**�O��Listen�P�ɶ��i�ѳs�u���ƶq�C*/
    int          m_u32Backlog;
    /**�O�����ݨϥΪ�Port*/
    int          m_s32PeerPort;
    /**�O�����a�ݪ�Port*/
    int          m_s32Port;
    /**�O�����a�ݪ���}*/
    std::string  m_strAddr;
    /**�O�����ݪ���}*/
    std::string  m_strPeerAddr;

    struct sockaddr m_socketDestAddr;
    socklen_t       m_nDestAddrLen ;

    struct sockaddr m_socketSrcAddr;
    socklen_t       m_nSrcAddrLen ;

    /*!  \brief Socket������ơA���]�ATimeout���ɭ��A�o�Ӭɭ��Opretected�ɭ��A�i���~�Ӫ̨ϥ�
    *
    *  \param hTSocket_t mSocket�A��ھާ@��Socket�ɭ�
    *  \param unsigned char *buf�A�n��������ƫ��СC
    *  \param unsigned int RecvSize�A�]�w�n������ƪ�����
    *  \param unsigned long ulTimeout�A�]�w���ݮɶ��A��쬰 msec
    *
    *  \return ���\�|�^�ǡA�ǰeSize�A���Ѧ^��-1��SOCKET_ERROR���ѡC
    */
    int RecvBufFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32/*unsigned int*/ RecvSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief �����GSocket������ơA�u�@�@��recv�A���h�ֺ�h�֡C���]�ATimeout���ɭ��A�o�Ӭɭ��Opretected�ɭ��A�i���~�Ӫ̨ϥ�
     *
     *  \param hTSocket_t mSocket�A��ھާ@��Socket�ɭ�
     *  \param unsigned char *buf�A�n��������ƫ��СC
     *  \param unsigned int uiBufSize�A�]�w�n������ƪ�����
     *  \param unsigned long ulTimeout�A�]�w���ݮɶ��A��쬰 msec
     *
     *  \return ���\�|�^�ǡA�ǰeSize�A���Ѧ^��-1��SOCKET_ERROR���ѡC
     */
    int RecvOnceFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief �����GSocket������ơA�u�@�@��recvfrom�A���h�ֺ�h�֡C���]�ATimeout���ɭ��A�o�Ӭɭ��Opretected�ɭ��A�i���~�Ӫ̨ϥ�
     *
     *  \param hTSocket_t mSocket�A��ھާ@��Socket�ɭ�
     *  \param unsigned char *buf�A�n��������ƫ��СC
     *  \param unsigned int uiBufSize�A�]�w�n������ƪ�����
     *  \param unsigned long ulTimeout�A�]�w���ݮɶ��A��쬰 msec
     *
     *  \return ���\�|�^�ǡA�ǰeSize�A���Ѧ^��-1��SOCKET_ERROR���ѡC
     */
    int RecvFromOnceFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief �����GSocket�ǰe��ơA�g��Select�����O�_�w�@�n�ǰe���ǳơA��o�eUDP��ơC���]�ATimeout���ɭ��A�o�Ӭɭ��Opretected�ɭ��A�i���~�Ӫ̨ϥ�
     *
     *  \param hTSocket_t mSocket�A��ھާ@��Socket�ɭ�
     *  \param unsigned char *buf�A�n�ǰe����ƫ��СC
     *  \param unsigned int uiBufSize�A�]�w�n������ƪ�����
     *  \param unsigned long ulTimeout�A�]�w���ݮɶ��A��쬰 msec
     *
     *  \return ���\�|�^�ǡA�ǰeSize�A���Ѧ^��-1��SOCKET_ERROR���ѡC
     */
    int SendToBufFunc(hTSocket_t mSocket, const char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief �����GSocket�ǰe��ơA�g��Select�����O�_�w�@�n�ǰe���ǳơA��o�eTCP��ơC���]�ATimeout���ɭ��A�o�Ӭɭ��Opretected�ɭ��A�i���~�Ӫ̨ϥ�
     *
     *  \param hTSocket_t mSocket�A��ھާ@��Socket�ɭ�
     *  \param unsigned char *buf�A�n�ǰe����ƫ��СC
     *  \param unsigned int uiBufSize�A�]�w�n������ƪ�����
     *  \param unsigned long ulTimeout�A�]�w���ݮɶ��A��쬰 msec
     *
     *  \return ���\�|�^�ǡA�ǰeSize�A���Ѧ^��-1��SOCKET_ERROR���ѡC
     */
    int SendBufFunc(hTSocket_t mSocket, const char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

private:
    static bool  m_bSocketEnvInit;

public:

    /*! \brief �����G�غc�l
     *
     *  \param protocol �ݴ��ѭn�غc���q�T��w�����C
     */
    fSocketTool (Protocol protocol = DUMMY);


    /*! \brief �����G�Ѻc�l
     *
     */
    virtual ~fSocketTool (void);

    /*! \brief �����G��SocketTool�Ӳ��ͥXSocketTool����
     *
     *  ��fSocketTool���ͿW�ߪ��@�ӬۦPProtocol��fSocketTool���СF
     *  �Ҧp�G�ڭ̥i�H���wServer�ϥ�IPv4�ӻݨϥ�2��port�ӧ@�A�ȡA�ҥH�b�J��o�˱��p�ɡA
     *  Server�i�H�ѳ]�w��fSocketTool�Ӳ��ͻݭn��Socket�ɭ��C
    */
    virtual fSocketTool *new_socket_tool(void);

    /*! \brief �����G�g��socketTool��new_socket_tool���ͪ�����A�ݭn��delete_socket_tool������O����
     *
     *  \parm ptr�A�ǤJ�n������fSocketTool���Ьɭ��A�Τ]�i�H�ϥ�delete�ӳB�z��new_socket_tool���ͪ����СC
    */
    virtual void delete_socket_tool(fSocketTool *ptr);

    /*! \brief �����G�إ�socket�A���P����socket(...)���O�A�إߥi��socket ()���osocket��Handle
     *
     *  �bcreate()���t�ݭn���wset_type()���A(TCP/UDP)�C����create()����A�bclass�����|���ͤ@�կu����socket�ɭ��C
    */
    virtual int create(void);

    /*! \brief �����G����socket�A�p�P����closesocket
     *
     *  ����socket�A�p�G�n�A�ϥ�socket�ɭ��A�ݭn�A�s��create()��A�N�i�H�A�ϥ�socket�\��F�C
    */
    virtual int destory(void);

    /*! \brief �����GSocket �g�Lcreate�Pattach_socket�A�Ҳ��ͪ�socket�O�_�w�g���ͦ��Ī�handle�A�D�n�O�d�O�_��INVALID_SOCKET��L�L�ġC
     *
    */
    virtual int is_valid(void) const;

    /*! \brief �]�w��udp,��TCP
     *
    */
    virtual void set_type(Type type);

    /*! \brief �^�Ǭ�udp,��TCP
     *
    */
    Type type (void) const;

    /*! \brief �ҫإߪ���k�A�p�Genum Protocol�ҩw�q
     *
    */
    Protocol protocol (void) const;

    /*! \brief ���oSocket���ɮ׫���
     *
    */
    hTSocket_t socket (void) const;

    /*! \brief �]�w�~��socket�w�g�إߧ����̪���socktool���ɭ��A�����ϥΦ������ɭ��\��C
     *
     *  \param hTSocket_t socket�A�ǤJ�n�̪���Socket�ɮ׫��СA�ë��w����(TCP/UDP)
    */
    virtual void attach_socket ( hTSocket_t socket, Type type );


    /*! \brief ���o�ثe�]�wSocket��Blocking���A
     *
    */
    int blocking (void) const;


    /*! \brief �]�wSocket��Blocking���A
     *
    */
    virtual void set_blocking ( int enable );

    /*! \brief ���o�ثeSocket�O�_�H�W���έ��ШϥΪ��ŧi�C
     *
    */
    int address_reusable (void) const;

    /*! \brief �]�wSocket�O�_�H�W���έ��ШϥΪ��ŧi�C
     *
    */
    virtual void set_address_reusable ( int enable );

    /*! \brief ���o�ثe�]�w�O�_��broadcast�Ҧ��C
     *
    */
    virtual int  is_broadcast_mode();

    /*! \brief �]�wbroadcast�Ҧ��C
     *  \param int enable�A0���D�A��L���ҰʡC
    */
    virtual void set_broadcast_mode(int enable);


    /*! \brief client�ݫإ߳s�u�A���w��}�Pport
     *
     *  \param std::string & addr �A���w�s�u�D���W�٩�IP
     *  \param unsigned int port�A���n�s�u�D����port�A�ȡC
    */
    virtual int connect ( std::string & addr, unsigned int port );

    /*! \brief �]�wServer���w�ɭ��Pport
     *
     *  \param std::string & address�A���ɭ�
     *  \param unsigned int port�A�n���w��Port�A��client�s�u�C
    */
    virtual int bind ( std::string & address, unsigned int port );

    /*! \brief �]�wServer��ť�ӼơC
     *
     *  \param int backlog�A���w�P�@�ɶ��̦h�s�uclient
    */
    virtual int listen ( int backlog );

    /*! \brief Server Accept�A�ȡA���H�s�u�i�ӮɡA�|�ۤv����Sockettool�A�����ɻݨϥ�delete_socket_tool����C
     *
     *  \return �^��fSocketTool* ���СA���s�s�u�i�ӮɡA�|���ͷs��fSocketTool�ɭ������СC
    */
    virtual fSocketTool *accept (void);


    ///�ثe���ϥ�
    virtual long wait_for_more ( int msecs, int * timeout = 0 ) const;

    /*! \brief �ѳs�u��Ū�^recv�A����udp/tcp ��timeout�P�S��timeout���ɭ��A�D�n�\��Precv/recvfrom
     *
     *  \param char * data �A��Ʊ����Ϫ����СC
     *  \param unsigned long maxlen�AŪ����ƪ��̪����סC
     *  \param unsigned long *pTimeOut = NULL�A�мg�A�i����J�A��줸m sec
     *  \return long��ơA����ڳB�z��ƪ��סC
    */
    virtual long read_block ( char * data, HT_U32/*unsigned long*/ maxlen, HT_U32/*unsigned long*/ *pTimeOut = NULL);

    /*! \brief �ѳs�u���ǰe�A����udp/tcp ��timeout�P�S��timeout���ɭ��A�D�n�\��Psend/sendto
     *
     *  \param char * data �A�ǰe��ưϪ����СC
     *  \param unsigned long len�A�n�ǰe��ƪ����סC
     *  \param unsigned long *pTimeOut = NULL�A�мg�A�i����J�A��줸m sec
     *  \return long��ơA����ڳB�z��ƪ��סC
    */
    virtual long write_block ( const char * data, HT_U32/*unsigned long*/ len, HT_U32/*unsigned long*/ *pTimeOut = NULL);

    virtual long read( char * data, HT_U32 maxlen, HT_U32 *pTimeOut=NULL);
    virtual long write( const char * data, HT_U32/*unsigned long*/ len, HT_U32/*unsigned long*/ *pTimeOut = NULL);

    /*! \brief �]�w�ǰeUDP��ƶǰe���ت��a
     *
     *  \param �Hchar�����ǤJ�ت��a��}�A�����|�ۤv�ഫ���T�w���榡�C
     *  \return int��ơA0�A�����\�A��L�����ѡC
    */
    virtual int set_udp_dest_addr(std::string strDest_addr, unsigned int port);
    virtual int set_udp_dest_addr(const struct sockaddr *dest_addr, socklen_t addrlen);

    /*! \brief �]�w����UDP��ƶǰe���ت��a
     *
     *  \param �Hchar�����ǤJ�ت��a��}�A�����|�ۤv�ഫ���T�w���榡�C
     *  \return int��ơA0�A�����\�A��L�����ѡC
    */
    virtual int set_udp_src_addr(std::string strSrc_addr, unsigned int port);
    virtual int set_udp_src_addr(const struct sockaddr *src_addr, socklen_t addrlen);

    /*! \brief �bsocket�ɭ����[�Jmulticast���������\��A���P��ϥ�
    *   setsockopt(pSocket->socket(), IPPROTO_IP, IP_ADD_MEMBERSHIP,(char *)&ssdpMcastAddr, sizeof(struct ip_mreq));
    *   �ثe���[�Jadd�A�ثe�S���[�Jremove���\��C���᭱�A�@2013/04/23
    *
    *    \param  std::string strInterface�A�]�w�n���X�s�����ɭ��C
    *    \param  std::string strMultiAddr�A�s����������}�C
    *    \return 0:��ܦ��\�A��L��ܥ��ѡC
    */
    virtual int add_membership(std::string strInterface, std::string strMultiAddr);
    virtual int add_membership(struct in_addr *addrInterface, struct in_addr *addrMultiAddr);


    virtual int set_timeout(enum Direct,HT_U32 u32mSec);



    /*! \brief ���oSocket�ϥΪ�port
     *
     *  \return int �APort�ƭ�
     */
    unsigned int port (void) const;

    /*! \brief ���o���誺port
     *
     *  \return int �APort�ƭ�
     */
    unsigned int peerPort (void) const;

    /*! \brief ���o���a�誺��}�C
     *
     *  \return std::string ��r�Ʀ�}
     */
    std::string address (void) const;

    /*! \brief ���o���誺��}�C
     *
     *  \return std::string ��r�Ʀ�}
     */
    std::string peer_address (void) const;


    /*! \brief ���o�̫���~�X�^���C
     *
     *  \return �^��Error���~�N�X
     */
    virtual Error error (void) const;

    /*! \brief �ثe�ΨӳB�zwindows��Winsock�Ұʪ��\��C
     */
    static bool Init(void);

    /*! \brief �ثe�ΨӳB�zwindows��Winsock����\��C
    */
    static void DeInit(void);

    virtual int ping_for_autoget(char *host,int nEthernetNum);

    virtual int in_cksum_for_autoget(unsigned short *buf, int sz);
};


#endif
