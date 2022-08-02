#include "StdAfx.h"
#include "Folder.h"


CFolder::CFolder(void)
{
}


CFolder::~CFolder(void)
{
}



//排序
void CFolder::FileArraySortbySystemTime(CStringArray& _arr)
{
	int len = _arr.GetCount();
	for(int i=0; i<len-1; i++)		
	{		
		int min=i;		
		for(int j=i+1; j<len; j++)		
		{	
 
			CString strtime1=GetFileLastModifyTime(_arr[min]);
			CString strtime2=GetFileLastModifyTime(_arr[j]);
 
			if(lstrcmpi(strtime1,strtime2)>0)
			{
				min=j;	
			}
		}					
		CString t=_arr[min];		
		_arr[min]=_arr[i];		
		_arr[i]=t;		
	}
 
 
}
 
//获取时间
 CString CFolder::GetFileLastModifyTime(LPCTSTR strPath)
{
	if (!::PathFileExists(strPath))
	{
		return _T("");
	}
 
	WIN32_FIND_DATA ffd ;  
	HANDLE hFind = FindFirstFile(strPath,&ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return _T("");
	}
	SYSTEMTIME st = {0};  
	FILETIME LocalFileTimel;
 
	FileTimeToLocalFileTime(&ffd.ftLastWriteTime, &LocalFileTimel);
	FileTimeToSystemTime(&LocalFileTimel, &st);
 
	CString strFormat;
	strFormat.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
 
	FindClose(hFind);
	return strFormat;
}
 
 
//查找目录下的文件
void CFolder::FindDirFile(CString strdir, CStringArray& _arr)
{
    CString filename = _T("");
    CString fullname = _T("");
	CString tempstrdir = strdir;

	TCHAR szPath[MAX_PATH] = {0};
	memcpy(szPath, strdir.GetBuffer(0), 2*strdir.GetLength());
	::PathRemoveFileSpec(szPath);
    CFileFind find;//_T("/HKSPlayer*.*")
    BOOL IsFind = find.FindFile(strdir);
 
    while (IsFind)
    {
        IsFind = find.FindNextFile();
        if (find.IsDots())
		{
            continue;
		}
        else
		{
            filename = find.GetFileName();
            fullname = szPath;
			fullname = fullname + L"\\";
			fullname = fullname + filename;
			_arr.Add(fullname);
		}
	}
}

DWORD CFolder::GetFileSize(LPCTSTR strPath)
{
	if (!::PathFileExists(strPath))
	{
		return -1;
	}

	DWORD size = 0;
	WIN32_FIND_DATA ffd ;  
	HANDLE hFind = FindFirstFile(strPath,&ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return  -1;
	}
	size = ffd.nFileSizeHigh * MAXDWORD + ffd.nFileSizeLow;
	FindClose(hFind);
	return size;
}

DWORD CFolder::GetFilesSize(CString strdir)
{
	DWORD size = 0;
	CStringArray strarry;

	FindDirFile(strdir, strarry);
	for (int i = 0; i < strarry.GetSize(); i++)
	{
		CString str = strarry.GetAt(i);
		DWORD len = GetFileSize(str);
		if (len>0)
			size += len;
	}

	return size;
}
