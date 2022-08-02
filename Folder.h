#pragma once
class CFolder
{
public:
	CFolder(void);
	~CFolder(void);
	void FileArraySortbySystemTime(CStringArray& _arr);
	CString GetFileLastModifyTime(LPCTSTR strPath);
	void FindDirFile(CString strdir, CStringArray& _arr);
	DWORD GetFileSize(LPCTSTR strPath);
	DWORD GetFilesSize(CString strdir);
};

