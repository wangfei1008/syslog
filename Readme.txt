========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : syslog
========================================================================


AppWizard has created this syslog DLL for you.  This DLL not only
demonstrates the basics of using the Microsoft Foundation classes but
is also a starting point for writing your DLL.

This file contains a summary of what you will find in each of the files that
make up your syslog DLL.

syslog.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

syslog.cpp
    This is the main DLL source file that contains the definition of
	DllMain().


syslog.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

syslog.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\syslog.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.

syslog.def
    This file contains information about the DLL that must be
    provided to run with Microsoft Windows.  It defines parameters
    such as the name and description of the DLL.  It also exports
	functions from the DLL.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named syslog.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
版本
1.0.2.4版本
时间：
2013/08/21
说明：
1，初始化写时没有对全局标志处理，可能导致关闭后的线程不完全退出。

版本
1.0.2.5版本
时间：
2014/02/25
说明：
1, 在日志中添加线程id

版本
1.0.2.6版本
时间：
2014/03/18
说明：
1, logclose函数释放错误，导致写文件线程无法正常退出，系统退出异常。


版本
1.0.2.7版本
时间：
2014/04/01
说明：
1, 增加写文件速度保证写书完成性

版本
1.0.2.8版本
时间：
2014/04/11
说明：
1, 写文件线程修改为自动在dll释放是销毁

版本
1.0.2.8版本
时间：
2014/06/28
说明：
1, 自动在dll释放是销毁失败，超时退出.

版本
1.0.2.10版本
时间：
2014/06/28
说明：
1, 时间精确到毫秒

版本
1.0.2.10版本
时间：
2021/08/17
说明：
1, 添加文件一次最大为2兆，日志最大数为100