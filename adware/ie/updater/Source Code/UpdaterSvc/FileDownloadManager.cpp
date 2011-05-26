// FileDownloadManager.cpp: implementation of the CFileDownloadManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileDownloadManager.h"
#include <comdef.h>
#include <wininet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFileDownloadManager::CFileDownloadManager()
{
	
}

CFileDownloadManager::~CFileDownloadManager()
{

}

void CFileDownloadManager::SetURLToDownload(_bstr_t cr_sURL)
{
	m_sURL	=	cr_sURL;
}

void CFileDownloadManager::GetDownloadedFilePath(_bstr_t& cr_sFilePath)
{
	cr_sFilePath	=	m_sLocalFilePath;
}

BOOL CFileDownloadManager::DoDownload()
{
	BOOL vl_bRetVal	=	FALSE;
	
	
	UpdateLocalFilePath();

	//Let's Download the File , In case Could not Download, Fill in the Error	
	HRESULT vl_hRetVal	=	URLDownloadToFile(0,m_sURL,m_sLocalFilePath,0,0);

	DeleteUrlCacheEntry(m_sURL);

	if(FAILED(vl_hRetVal))
	{
		_com_error	vl_oError(vl_hRetVal);

		m_sErrorDescription	=	vl_oError.Description();
	}
	else
	{
		vl_bRetVal	=	TRUE;
	}
	
	return vl_bRetVal;
}

void CFileDownloadManager::GetLastError(_bstr_t&	cr_sError)
{
	cr_sError	=	m_sErrorDescription;
}

void CFileDownloadManager::UpdateLocalFilePath()
{
	TCHAR vl_sTempFolder[MAX_PATH+1];
	TCHAR vl_sFilePath[MAX_PATH+1];

	::GetTempPath(MAX_PATH,vl_sTempFolder);

	::GetTempFileName(vl_sTempFolder,"IRF",0,vl_sFilePath);

	m_sLocalFilePath	=	vl_sFilePath;

	::DeleteFile(m_sLocalFilePath);
}

BOOL	CFileDownloadManager::IsFileDownloadedOK(_bstr_t cr_sFilePath)
{
	return FALSE;	
}