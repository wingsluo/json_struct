/****************************************************************************
//说明：linux宏配置
//作者：罗羽
//日期：2019-9
****************************************************************************/
#ifndef __CONFIG_LINUX_H_INCLUDE__
#define __CONFIG_LINUX_H_INCLUDE__

#include <features.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdarg.h>
#include <dirent.h>
#include <pwd.h>
#include <cxxabi.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
//----------------------------------------------------------
//类型及宏定义
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;

typedef int					BOOL;
typedef unsigned int UINT;
typedef int INT;
typedef unsigned long ULONG;

typedef long long __int64;

typedef int        SOCKET;
#ifndef NULL
#define NULL (0)
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef const char* LPCTSTR;
typedef char*  LPTSTR;
#ifndef TCHAR
  typedef char      TCHAR;
#endif

#if !defined (PATH_MAX)
#  if defined (_MAX_PATH)
#    define PATH_MAX _MAX_PATH
#  elif defined (MAX_PATH)
#    define PATH_MAX MAX_PATH
#  else /* !_MAX_PATH */
#    define PATH_MAX 1024
#  endif /* _MAX_PATH */
#endif /* !PATH_MAX */

#ifndef MAX_PATH
#  define MAX_PATH PATH_MAX
#endif

#ifndef Sleep
#define Sleep(x) usleep(1000*(x))
#endif

#define last_error() errno
#ifndef GETS
#define GETS    gets
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#define closesocket close


#endif // ifndef __CONFIG_LINUX_H_INCLUDE__
