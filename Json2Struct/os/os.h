/****************************************************************************
//说明：操作系统头文件处理，用于跨平台编译
//作者：罗羽
//日期：2019-9
****************************************************************************/

#ifndef __OS_H_INCLUDE_
#define __OS_H_INCLUDE_

#if defined(WIN32) || defined(_WIN64)
	#include "config-win32.h"

	#ifndef COMMON_DLL
		#define COMMON_FUNC _declspec(dllimport)
	#else 
		#define COMMON_FUNC _declspec(dllexport)
	#endif
#else
	#include "config-linux.h"
	#define COMMON_FUNC
#endif

#include "os_type.h"

# define BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define SET_BITS(WORD, BITS) (WORD |= (BITS))
# define CLR_BITS(WORD, BITS) (WORD &= ~(BITS))


//c runtime library
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <float.h>


//standard c++ library and STL
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <set>
#include <queue>
#include <algorithm>
#include <utility>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <exception>
#include <cassert>
#include <typeinfo>
#include <mutex>
#include <codecvt>
#include <locale>
#include <thread>
#include <chrono>

#include <mutex>

using std::mutex;

using std::string;
using std::wstring;
using std::set;
using std::list;
using std::vector;
using std::map;
using std::multimap;
using std::unordered_map;
using std::unordered_set;
using std::stack;
using std::exception;
using std::make_pair;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;


#ifndef OUT
#define OUT
#endif // !OUT

#ifndef IN
#define IN
#endif // !OUT

#ifndef OPTIONAL
#define OPTIONAL
#endif// !OPTIONAL

#define min_func(x, y) ((x) < (y) ? (x) : (y))

#define max_func(x, y) ((x) > (y) ? (x) : (y))


#endif // ifndef __OS_H_INCLUDE_
