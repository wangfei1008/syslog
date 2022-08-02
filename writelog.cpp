
#include "StdAfx.h"
#include "queue.h"
#include "syslog.h"
#include <windows.h>
#include "Folder.h"
#include <Shellapi.h>
#include <imagehlp.h>

#pragma comment(lib,"imagehlp.lib")

extern queue handle;

#define SYSLOG_TYPE_INIT          -1
#define SYSLOG_TYPE_OPEN           0
#define SYSLOG_TYPE_REQ_REL        1
#define SYSLOG_TYPE_REQ_SUC        2 

static unsigned long g_filesize = 0;
static int closetype = SYSLOG_TYPE_INIT;

static void czero(void* src, int len)
{
	memset(src, '\0', len);
}

static char* getcurrenttime()
{
	static char timestr[64];
	time_t newtime;
	struct tm* curtime;
	
	czero(timestr, 64);
	time(&newtime);
	curtime = localtime(&newtime);
	sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d", curtime->tm_year + 1900, curtime->tm_mon + 1,
		curtime->tm_mday, curtime->tm_hour, curtime->tm_min, curtime->tm_sec);
	
	return timestr;
}

int CreateFolder(const char* dir)
{
	CString string;
	string.Format("%s",dir);
	if ((string.ReverseFind('\\') + 1) != string.GetLength())
	{
		string += "\\";
	}
	
	return MakeSureDirectoryPathExists(string);
}

void RemoveFiles()
{
	char str[256];
	CStringArray stringarr;

	czero(str, 256);
	GetEnvironmentVariable("USERPROFILE", str, 256);
	int len = strlen(str);
	sprintf(str + len, "\\Media\\Log\\");

	CFolder folder;
	folder.FindDirFile(str, stringarr);
	folder.FileArraySortbySystemTime(stringarr);
	while( stringarr.GetSize() >= 100)
	{
		CString str = stringarr.GetAt(0);
		DeleteFile(str);
	}
}

int GetFileSize(char* filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp) return -1;
    fseek(fp,0L,SEEK_END);
    int size=ftell(fp);
    fclose(fp);
    
    return size;
}
FILE* logopen(FILE* pfile)
{
	static char sysfile[256];
	int len = 0, index = 0;
	char str[256];
	time_t newtime;
    struct tm* curtime;

	czero(sysfile, 256);
	czero(str, 256);
	time(&newtime);
	curtime = localtime(&newtime);
	GetEnvironmentVariable("USERPROFILE", str, 256);
	len = strlen(str);
	sprintf(str + len, "\\Media\\Log");
	CreateFolder(str);
	len = strlen(str);

	index = g_filesize/2090000;
	sprintf(str + len, "\\HKSPlayer.%04d%02d%02d.%d.log", curtime->tm_year + 1900, curtime->tm_mon + 1,curtime->tm_mday, index);

	if (strcmp(str, sysfile) != 0 || pfile == NULL)
	{
		if(pfile != NULL) fclose(pfile);
		
		pfile = fopen(str, "a");
		if(pfile == NULL) return NULL;
		strcpy(sysfile, str);

		RemoveFiles();

		g_filesize = GetFileSize(sysfile);
	}
	
	return pfile;
}

void* writethread(void* param)
{
	char* buffer = NULL;
	FILE* pfile = NULL;
	CTime ct = CTime::GetCurrentTime();
	while (1)
	{
		if (closetype == SYSLOG_TYPE_REQ_REL) break;

		if (lengthqueue(&handle) == 0)
		{
			Sleep(1000);
			continue;
		}
		else
		{	
			if(NULL == (pfile = logopen(pfile)))
			{
				Sleep(100);
				continue;
			}
			while(buffer = (char*)popqueue(&handle))
			{
				g_filesize += strlen(buffer);
				fprintf(pfile, "%s", buffer);
				fprintf(pfile, "\n\n");
				fflush(pfile);
				free(buffer);
				buffer = NULL;
			//	Sleep(10);
			}
		}

		CTimeSpan  stime = CTime::GetCurrentTime() - ct;
		if(stime.GetTotalHours() > 24)
		{
			ct = CTime::GetCurrentTime();
			g_filesize = 0;
		}
	}
	closetype = SYSLOG_TYPE_REQ_SUC;
	return NULL;
}

void initwrite()
{
	pthread_t threadid;
	pthread_attr_t attr;

	if(closetype != SYSLOG_TYPE_INIT) return;

	initqueue(&handle);	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&threadid, &attr, writethread, NULL);
	closetype = SYSLOG_TYPE_OPEN;
}

void logclose()
{
// 	if(closetype == SYSLOG_TYPE_OPEN)
// 	{
// 		closetype = SYSLOG_TYPE_REQ_REL;
// 		while (closetype == SYSLOG_TYPE_REQ_REL) {
// 			Sleep(50);
// 		}
// 	}
// 	clearqueue(&handle);
// 	closetype = SYSLOG_TYPE_INIT;
}

void releasewrite()
{
	int inc = 0;
	if(closetype == SYSLOG_TYPE_OPEN)
	{
		closetype = SYSLOG_TYPE_REQ_REL;
		while (closetype == SYSLOG_TYPE_REQ_REL && inc < 25) {
			inc++;
			Sleep(50);
		}
	}
	clearqueue(&handle);
	closetype = SYSLOG_TYPE_INIT;
}