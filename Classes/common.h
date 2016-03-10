#ifndef COMMON_H
#define COMMON_H


#include <string.h>
#include <time.h>
#include <stdio.h>

#include <stdarg.h>

#include <iostream>
#include <fstream>
#include <map>
#include <deque>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#include <process.h>
#include <tchar.h>
#include <WINSOCK2.H>
#include <Mswsock.h>
#include <windows.h>
#ifdef _DEBUG
#include <assert.h>
#endif

#include <io.h>
#include <winsock.h>
#include <wininet.h>

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#pragma comment(lib,"WS2_32")
#pragma comment(lib,"odbc32")

#else
#include<pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */

#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */

#endif

using namespace std;

#ifndef SZ_LENGTH
#define SZ_LENGTH 128
#endif

#ifndef SZ_S_LENGTH
#define SZ_S_LENGTH 64
#endif

#ifndef SZ_L_LENGTH
#define SZ_L_LENGTH 512
#endif

#ifndef SZ_8_LENGTH
#define SZ_8_LENGTH 8
#endif

#ifndef SZ_16_LENGTH
#define SZ_16_LENGTH 16
#endif

#ifndef SZ_32_LENGTH
#define SZ_32_LENGTH 32
#endif

#ifndef SZ_1024_LENGTH
#define SZ_1024_LENGTH 1024
#endif

#ifndef SZ_2048_LENGTH
#define SZ_2048_LENGTH 2048
#endif

#ifndef SZ_4096_LENGTH
#define SZ_4096_LENGTH 4096
#endif



// 定义类中属性的访问方法（可以减少编写重复的代码）
#ifndef PROPERTY_INIT
#define PROPERTY_INIT(xtype,xname,proper)\
	void	Set##xname(xtype val)\
{\
	proper = val;\
}\
	xtype Get##xname()\
{\
	return proper;\
}\

#endif

#endif // COMMON_H
