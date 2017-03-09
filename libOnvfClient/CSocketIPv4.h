#ifndef _CSOCKETIPV4_H_
#define _CSOCKETIPV4_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string>

#include "fSocketTool.h"

class LIBIPCAMNET_CLASS CSocketIPv4
    : public fSocketTool
{
    int  m_bAttachSocket;
    struct sockaddr_in m_fsaun;
    int m_fromlen;
public:
    CSocketIPv4 (Type type = Stream );
    virtual ~CSocketIPv4();
    virtual fSocketTool *new_socket_tool(void);
    virtual void delete_socket_tool(fSocketTool *ptr);
    virtual int create();
    virtual int destory();
    virtual int is_valid () const;
    virtual void set_type(Type type);
    virtual void attach_socket ( hTSocket_t socket, Type type );
    virtual void set_blocking ( int enable );
    virtual int connect ( std::string & addr, unsigned int port );
    virtual int bind ( std::string & address, unsigned int port );
    virtual int listen ( int backlog );
    virtual fSocketTool *accept ();
    virtual long wait_for_more ( int msecs, int * timeout = 0 ) const;

    virtual int set_udp_dest_addr(std::string strAddr, unsigned int port);
    virtual int set_udp_src_addr (std::string strAddr, unsigned int port);

    virtual int add_membership(std::string strInterface, std::string strMultiAddr);
    virtual int add_membership(struct in_addr *addrInterface, struct in_addr *addrMultiAddr);

};

#endif
