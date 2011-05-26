// FileDownloadManager.h: interface for the CFileDownloadManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDOWNLOADMANAGER_H__92A0629C_02C7_4777_9BBE_A045162FDF4D__INCLUDED_)
#define AFX_FILEDOWNLOADMANAGER_H__92A0629C_02C7_4777_9BBE_A045162FDF4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileDownloadManager
{
	_bstr_t m_sURL;
	_bstr_t m_sLocalFilePath;
	_bstr_t m_sErrorDescription;

	void UpdateLocalFilePath();

	BOOL	IsFileDownloadedOK(_bstr_t cr_sFilePath);

public:
	
	void GetLastError(_bstr_t& cr_sError);
	BOOL DoDownload();
	void GetDownloadedFilePath(_bstr_t& cr_sFilePath);

	void SetURLToDownload(_bstr_t cr_sURL);
	CFileDownloadManager();
	virtual ~CFileDownloadManager();

};

#endif // !defined(AFX_FILEDOWNLOADMANAGER_H__92A0629C_02C7_4777_9BBE_A045162FDF4D__INCLUDED_)
