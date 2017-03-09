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

#define CGI_RESPONSE_BUF_SIZE 32768 /*20480*/  // 20110914  20480 -> 32768 (Truen TCAM-570 吐回來的值就高達 23K "Content-Length: 23194")
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
#define CGI_RESPONSE_BUF_SIZE 32768 /*20480*/  // 20110914  20480 -> 32768 (Truen TCAM-570 吐回來的值就高達 23K "Content-Length: 23194")
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
 *   \brief 提供Socket使用方法的標準界面。
 *   提供Socket的基礎類別，可以使用這個類別的標準界面，來實作IPv4, IPv6, IPC ,IPv4SSL,IPv6SSL....等同訊協定。
 *   將來運用於IPCamModule中，可以實現在windows與linux平台之中。
 *
 */
class LIBIPCAMNET_CLASS fSocketTool
{
public:
    enum  Type     {
        Stream      ///TCP型態
        ,Datagram    ///UDP型態
        ,InvalidType ///無效類型
    };

    enum  Protocol {
        DUMMY       ///假的空協定。
        , IPv4        ///IPv4的通訊協定。
        , IPv6        ///IPv6的通訊協定。
        , IPC         ///Unix Socket通訊協定。
        , IPv4SSL     ///SSL over IPv4的通訊方法。
        , IPv6SSL     ///SSL over IPv6的通訊方法。
    };

    enum  Error {
        NoError                           /** 未發生錯誤等同於Success  */
        , Success = NoError               /** 成功                     */
        , AlreadyBound                    /** 已經完成邦定             */
        , Inaccessible                    /** 無法存取可能有防火牆有關 */
        , NoResources                     /** 無法取得資源             */
        , InternalError                   /** 內部錯誤等同於Bug        */
        , Bug = InternalError             /** 程式Bug                  */
        , Impossible                      /** 不可能發生               */
        , NoFiles                         /** Socket檔案指標異常       */
        , ConnectionRefused               /** 連線被拒決。             */
        , NetworkFailure                  /** 網路裝置失敗             */
        , UnknownError 		              /** 不知明的錯誤             */
    };
    enum Direct {
        DIR_NONE=0,
        DIR_RECV=1,
        DIR_SEND=2,
        DIR_BOTH=3,
    };
protected:
    /**記錄Stream(TCP)/UDP類型/ */
    Type         m_StreamType;
    /**記錄通訊協定類型IPv4,IPv6......*/
    Protocol     m_Protocol;
    /**記錄最後發生錯誤的資訊*/
    Error        m_errLastStatus;
    /**記錄Socket的檔案指標。*/
    hTSocket_t   m_socket;
    /**記錄Socket是否為Block型態。*/
    int          m_bBlocking;
    /**記錄Socket是否設定為重覆資源失用*/
    int          m_bAddressReusable;
    /**記錄Socket是否為BroadCastMode*/
    int          m_bBroadcastMode;
    /**記錄Listen同時間可供連線之數量。*/
    int          m_u32Backlog;
    /**記錄遠端使用的Port*/
    int          m_s32PeerPort;
    /**記錄本地端的Port*/
    int          m_s32Port;
    /**記錄本地端的位址*/
    std::string  m_strAddr;
    /**記錄遠端的位址*/
    std::string  m_strPeerAddr;

    struct sockaddr m_socketDestAddr;
    socklen_t       m_nDestAddrLen ;

    struct sockaddr m_socketSrcAddr;
    socklen_t       m_nSrcAddrLen ;

    /*!  \brief Socket接收資料，有包括Timeout的界面，這個界面是pretected界面，可由繼承者使用
    *
    *  \param hTSocket_t mSocket，實際操作的Socket界面
    *  \param unsigned char *buf，要接收的資料指標。
    *  \param unsigned int RecvSize，設定要接收資料的長度
    *  \param unsigned long ulTimeout，設定等待時間，單位為 msec
    *
    *  \return 成功會回傳，傳送Size，失敗回傳-1或SOCKET_ERROR失敗。
    */
    int RecvBufFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32/*unsigned int*/ RecvSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief 說明：Socket接收資料，只作一次recv，收多少算多少。有包括Timeout的界面，這個界面是pretected界面，可由繼承者使用
     *
     *  \param hTSocket_t mSocket，實際操作的Socket界面
     *  \param unsigned char *buf，要接收的資料指標。
     *  \param unsigned int uiBufSize，設定要接收資料的長度
     *  \param unsigned long ulTimeout，設定等待時間，單位為 msec
     *
     *  \return 成功會回傳，傳送Size，失敗回傳-1或SOCKET_ERROR失敗。
     */
    int RecvOnceFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief 說明：Socket接收資料，只作一次recvfrom，收多少算多少。有包括Timeout的界面，這個界面是pretected界面，可由繼承者使用
     *
     *  \param hTSocket_t mSocket，實際操作的Socket界面
     *  \param unsigned char *buf，要接收的資料指標。
     *  \param unsigned int uiBufSize，設定要接收資料的長度
     *  \param unsigned long ulTimeout，設定等待時間，單位為 msec
     *
     *  \return 成功會回傳，傳送Size，失敗回傳-1或SOCKET_ERROR失敗。
     */
    int RecvFromOnceFunc(hTSocket_t mSocket, unsigned char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief 說明：Socket傳送資料，經由Select偵測是否已作好傳送的準備，後發送UDP資料。有包括Timeout的界面，這個界面是pretected界面，可由繼承者使用
     *
     *  \param hTSocket_t mSocket，實際操作的Socket界面
     *  \param unsigned char *buf，要傳送的資料指標。
     *  \param unsigned int uiBufSize，設定要接收資料的長度
     *  \param unsigned long ulTimeout，設定等待時間，單位為 msec
     *
     *  \return 成功會回傳，傳送Size，失敗回傳-1或SOCKET_ERROR失敗。
     */
    int SendToBufFunc(hTSocket_t mSocket, const char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

    /*! \brief 說明：Socket傳送資料，經由Select偵測是否已作好傳送的準備，後發送TCP資料。有包括Timeout的界面，這個界面是pretected界面，可由繼承者使用
     *
     *  \param hTSocket_t mSocket，實際操作的Socket界面
     *  \param unsigned char *buf，要傳送的資料指標。
     *  \param unsigned int uiBufSize，設定要接收資料的長度
     *  \param unsigned long ulTimeout，設定等待時間，單位為 msec
     *
     *  \return 成功會回傳，傳送Size，失敗回傳-1或SOCKET_ERROR失敗。
     */
    int SendBufFunc(hTSocket_t mSocket, const char *buf, HT_U32/*unsigned int*/ uiBufSize, HT_U32/*unsigned long*/ ulTimeout);

private:
    static bool  m_bSocketEnvInit;

public:

    /*! \brief 說明：建構子
     *
     *  \param protocol 需提供要建構的通訊協定類型。
     */
    fSocketTool (Protocol protocol = DUMMY);


    /*! \brief 說明：解構子
     *
     */
    virtual ~fSocketTool (void);

    /*! \brief 說明：由SocketTool來產生出SocketTool指標
     *
     *  由fSocketTool產生獨立的一個相同Protocol的fSocketTool指標；
     *  例如：我們可以指定Server使用IPv4而需使用2個port來作服務，所以在遇到這樣情況時，
     *  Server可以由設定的fSocketTool來產生需要的Socket界面。
    */
    virtual fSocketTool *new_socket_tool(void);

    /*! \brief 說明：經由socketTool的new_socket_tool產生的物件，需要由delete_socket_tool來釋放記憶體
     *
     *  \parm ptr，傳入要消毀的fSocketTool指標界面，或也可以使用delete來處理由new_socket_tool產生的指標。
    */
    virtual void delete_socket_tool(fSocketTool *ptr);

    /*! \brief 說明：建立socket，等同執行socket(...)指令，建立可由socket ()取得socket的Handle
     *
     *  在create()之差需要先定set_type()型態(TCP/UDP)。執行create()之後，在class內部會產生一組真正的socket界面。
    */
    virtual int create(void);

    /*! \brief 說明：關閉socket，如同執行closesocket
     *
     *  關閉socket，如果要再使用socket界面，需要再叫用create()後，就可以再使用socket功能了。
    */
    virtual int destory(void);

    /*! \brief 說明：Socket 經過create與attach_socket，所產生的socket是否已經產生有效的handle，主要是查是否為INVALID_SOCKET其他無效。
     *
    */
    virtual int is_valid(void) const;

    /*! \brief 設定為udp,或TCP
     *
    */
    virtual void set_type(Type type);

    /*! \brief 回傳為udp,或TCP
     *
    */
    Type type (void) const;

    /*! \brief 所建立的方法，如果enum Protocol所定義
     *
    */
    Protocol protocol (void) const;

    /*! \brief 取得Socket的檔案指標
     *
    */
    hTSocket_t socket (void) const;

    /*! \brief 設定外來socket已經建立完成依附於socktool之界面，直接使用此類型界面功能。
     *
     *  \param hTSocket_t socket，傳入要依附的Socket檔案指標，並指定類型(TCP/UDP)
    */
    virtual void attach_socket ( hTSocket_t socket, Type type );


    /*! \brief 取得目前設定Socket的Blocking狀態
     *
    */
    int blocking (void) const;


    /*! \brief 設定Socket的Blocking狀態
     *
    */
    virtual void set_blocking ( int enable );

    /*! \brief 取得目前Socket是否以獨佔或重覆使用的宣告。
     *
    */
    int address_reusable (void) const;

    /*! \brief 設定Socket是否以獨佔或重覆使用的宣告。
     *
    */
    virtual void set_address_reusable ( int enable );

    /*! \brief 取得目前設定是否為broadcast模式。
     *
    */
    virtual int  is_broadcast_mode();

    /*! \brief 設定broadcast模式。
     *  \param int enable，0為非，其他為啟動。
    */
    virtual void set_broadcast_mode(int enable);


    /*! \brief client端建立連線，指定位址與port
     *
     *  \param std::string & addr ，指定連線主的名稱或IP
     *  \param unsigned int port，指要連線主的機port服務。
    */
    virtual int connect ( std::string & addr, unsigned int port );

    /*! \brief 設定Server邦定界面與port
     *
     *  \param std::string & address，指界面
     *  \param unsigned int port，要邦定的Port，供client連線。
    */
    virtual int bind ( std::string & address, unsigned int port );

    /*! \brief 設定Server監聽個數。
     *
     *  \param int backlog，指定同一時間最多連線client
    */
    virtual int listen ( int backlog );

    /*! \brief Server Accept服務，當有人連線進來時，會自己產生Sockettool，結束時需使用delete_socket_tool釋放。
     *
     *  \return 回傳fSocketTool* 指標，當有新連線進來時，會產生新的fSocketTool界面的指標。
    */
    virtual fSocketTool *accept (void);


    ///目前不使用
    virtual long wait_for_more ( int msecs, int * timeout = 0 ) const;

    /*! \brief 由連線中讀回recv，提供udp/tcp 有timeout與沒有timeout的界面，主要功能同recv/recvfrom
     *
     *  \param char * data ，資料接收區的指標。
     *  \param unsigned long maxlen，讀取資料的最長長度。
     *  \param unsigned long *pTimeOut = NULL，覆寫，可不輸入，單位元m sec
     *  \return long資料，為實際處理資料長度。
    */
    virtual long read_block ( char * data, HT_U32/*unsigned long*/ maxlen, HT_U32/*unsigned long*/ *pTimeOut = NULL);

    /*! \brief 由連線中傳送，提供udp/tcp 有timeout與沒有timeout的界面，主要功能同send/sendto
     *
     *  \param char * data ，傳送資料區的指標。
     *  \param unsigned long len，要傳送資料的長度。
     *  \param unsigned long *pTimeOut = NULL，覆寫，可不輸入，單位元m sec
     *  \return long資料，為實際處理資料長度。
    */
    virtual long write_block ( const char * data, HT_U32/*unsigned long*/ len, HT_U32/*unsigned long*/ *pTimeOut = NULL);

    virtual long read( char * data, HT_U32 maxlen, HT_U32 *pTimeOut=NULL);
    virtual long write( const char * data, HT_U32/*unsigned long*/ len, HT_U32/*unsigned long*/ *pTimeOut = NULL);

    /*! \brief 設定傳送UDP資料傳送的目的地
     *
     *  \param 以char型式傳入目的地位址，內部會自己轉換為固定的格式。
     *  \return int資料，0，為成功，其他為失敗。
    */
    virtual int set_udp_dest_addr(std::string strDest_addr, unsigned int port);
    virtual int set_udp_dest_addr(const struct sockaddr *dest_addr, socklen_t addrlen);

    /*! \brief 設定接收UDP資料傳送的目的地
     *
     *  \param 以char型式傳入目的地位址，內部會自己轉換為固定的格式。
     *  \return int資料，0，為成功，其他為失敗。
    */
    virtual int set_udp_src_addr(std::string strSrc_addr, unsigned int port);
    virtual int set_udp_src_addr(const struct sockaddr *src_addr, socklen_t addrlen);

    /*! \brief 在socket界面中加入multicast的成員的功能，等同於使用
    *   setsockopt(pSocket->socket(), IPPROTO_IP, IP_ADD_MEMBERSHIP,(char *)&ssdpMcastAddr, sizeof(struct ip_mreq));
    *   目前先加入add，目前沒有加入remove的功能。等後面再作2013/04/23
    *
    *    \param  std::string strInterface，設定要提出廣播的界面。
    *    \param  std::string strMultiAddr，廣播成員的位址。
    *    \return 0:表示成功，其他表示失敗。
    */
    virtual int add_membership(std::string strInterface, std::string strMultiAddr);
    virtual int add_membership(struct in_addr *addrInterface, struct in_addr *addrMultiAddr);


    virtual int set_timeout(enum Direct,HT_U32 u32mSec);



    /*! \brief 取得Socket使用的port
     *
     *  \return int ，Port數值
     */
    unsigned int port (void) const;

    /*! \brief 取得遠方的port
     *
     *  \return int ，Port數值
     */
    unsigned int peerPort (void) const;

    /*! \brief 取得本地方的位址。
     *
     *  \return std::string 文字化位址
     */
    std::string address (void) const;

    /*! \brief 取得遠方的位址。
     *
     *  \return std::string 文字化位址
     */
    std::string peer_address (void) const;


    /*! \brief 取得最後錯誤碼回報。
     *
     *  \return 回傳Error錯誤代碼
     */
    virtual Error error (void) const;

    /*! \brief 目前用來處理windows中Winsock啟動的功能。
     */
    static bool Init(void);

    /*! \brief 目前用來處理windows中Winsock停止的功能。
    */
    static void DeInit(void);

    virtual int ping_for_autoget(char *host,int nEthernetNum);

    virtual int in_cksum_for_autoget(unsigned short *buf, int sz);
};


#endif
