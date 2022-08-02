
#include "StdAfx.h"
#include "syslog.h"
#include "queue.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

queue handle;
static char loginput = 0;

typedef enum leveltype
{
	LEVLE_NONE = 0,    
	LEVLE_ERROR,      
	LEVLE_DEBUG,       
	LEVLE_INFO = 4    
}leveltype;

static void czero(void* src, int len)
{
	memset(src, '\0', len);
}

static char* getcurrenttime()
{
	static char timestr[64];
	
	SYSTEMTIME sys; 
	GetLocalTime(&sys);

	czero(timestr, 64);
	sprintf(timestr, "time:%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds);
	
	return timestr;
}

unsigned int pthreadselfid()
{
#ifdef WIN32
	return::GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

int logwrite(int level, const char* filename, const int line, const char* funname, const char* fmt, ...)
{
	char* buffer;
	int length = 0;
	if(loginput & level) return -1;

	if(NULL == (buffer = (char*)malloc(sizeof(char) * MAX_LOG_BUFFER)))
		return -1;

	czero(buffer, MAX_LOG_BUFFER);
	sprintf(buffer, "[%s--%s(%d):%s(%d)]", getcurrenttime(), filename, line, funname,  pthreadselfid());
	switch(level)
	{
	case LEVLE_NONE:
		break;
	case LEVLE_ERROR:
		strcat(buffer, "LOG ERROR:");
		break;
	case LEVLE_DEBUG:
		strcat(buffer, "LOG DEBUG:");
		break;
	case LEVLE_INFO:
		strcat(buffer, "LOG INFO:");
		break;
	}

	length = strlen(buffer);
	va_list arg;
	va_start(arg, fmt);
	vsprintf(buffer + length, fmt, arg);
	if(!(loginput & 0x08))
		vprintf(fmt, arg);
	va_end(arg);

	pushqueue(&handle, buffer);

	return 0;
}


void logsetparm(char type)
{
	loginput = type;
}
