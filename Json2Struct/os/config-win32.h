/****************************************************************************
//说明：win32宏配置
//作者：罗羽
//日期：2019-9
****************************************************************************/

#ifndef __CONFIG_WIN32_H_INCLUDE__
#define __CONFIG_WIN32_H_INCLUDE__


#if defined ( _MSC_VER ) && ( _MSC_VER > 1000 )
//#  pragma warning (disable:4819)	
#  pragma warning (disable:4996)	//未使用_s函数报错
#  pragma warning (disable:4200)	//未使用_s函数报错
#  pragma warning (disable:4715)    //不是所有的控件路径都返回值
#endif

#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#  define WINVER 0x0500		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#  define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#if  !defined _WINSOCKAPI_ && !defined _WINSOCK2API_
#define FD_SETSIZE 1024
#include <winsock2.h>
#endif

#if  !defined _INC_WINDOWS && !defined _AFX_ALL_WARNINGS
//#include <windows.h>
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

#include <process.h>	// getpid() ?

#include <direct.h>
#include <tchar.h>
#include <io.h>

#include <crtdbg.h>
#ifndef strcasecmp
#define strcasecmp   stricmp
#endif
#ifndef last_error
#define last_error() GetLastError()
#endif
#ifndef sleep
#define sleep(x) Sleep((x)*1000)
#endif
#pragma comment(lib, "ws2_32.lib")

#ifndef GETS
#define GETS    gets_s
#endif

#define socklen_t int
#include <codecvt>

//#define RED ""
//#define GREEN ""
//#define YELLOW ""
//#define BLUE ""
//#define PURPLE ""
//#define CYAN ""
//#define CLEAR ""
#endif // ifndef __CONFIG_WIN32_H_INCLUDE__
