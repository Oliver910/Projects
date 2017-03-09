#ifndef _HDEBUGONGCC_H_
#define _HDEBUGONGCC_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <stdarg.h>

#define DBG_LOG_PATH "/var/log/debug.log"
#define DEBUG_MAX_LINE_LEN 512
inline void log_printf(const char* msg, ...)
{
    if (msg)
    {
        char buf[DEBUG_MAX_LINE_LEN + 1];
        va_list argp;
        va_start(argp, msg);
        vsnprintf(buf, DEBUG_MAX_LINE_LEN, msg, argp);
        va_end(argp);

        FILE *debugFp=fopen(DBG_LOG_PATH,"a");
        fprintf(debugFp,buf);
        fclose(debugFp);
        // TODO write it to the log file or whatever
    }
}

#define hPrint(format,args...)  do{ printf("PRI[%s:%d]",__FILE__, __LINE__);  printf(format, ##args);}while(0)
#define hLog(format,args...)
#define hInfo(format,args...)
#define hTRACE(format,args...)
#define hDebug(format,args...)
#define fDebug(format,args...)

#define hPRINT_TTID()do{ printf("[%s:%d:%s]ppid=%d pid=%d tid=>%d\n",__FILE__, __LINE__,__FUNCTION__,getppid(),getpid(),gettid());}while(0)


#endif

