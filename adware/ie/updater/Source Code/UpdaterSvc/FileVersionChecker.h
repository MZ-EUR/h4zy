// FileVersionChecker.h: interface for the CFileVersionChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEVERSIONCHECKER_H__45BD73C1_017B_4A6C_9A1D_725F2D10F68D__INCLUDED_)
#define AFX_FILEVERSIONCHECKER_H__45BD73C1_017B_4A6C_9A1D_725F2D10F68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileVersionChecker  
{
	_bstr_t	m_sRemoteBHOFilePath;//This is remote BHO File Path loaded from the ini file 

	_bstr_t	m_sRemoteFileVersionCheckPath;//This is the remote .txt file path to check for latest version

	_bstr_t	m_sRemoteVersion,m_sLocalVersion;
	
	_bstr_t	m_sBHOFilePath;//This is the bho file path

	_bstr_t	m_sDownloadedBHOFilePath;//This is the File Path of the Downloaded file


	BOOL LoadFileVersion(_bstr_t cr_sFilePath, _bstr_t& cr_sFileVersion);


	void GetAppFolderPath(HINSTANCE cr_hInstance,TCHAR*	cr_sPath);
public:
	void UpdateBHOToNewerVersion();
	BOOL DownloadLatestVersion();
	BOOL IsFileUpdateRequired();

	BOOL UpdateLatestVersionFromRemote();
	BOOL UpdateFileVersionFromLocal();

	BOOL LoadConfigurableParametersFromINIFile();
	CFileVersionChecker();
	virtual ~CFileVersionChecker();

};

#endif // !defined(AFX_FILEVERSIONCHECKER_H__45BD73C1_017B_4A6C_9A1D_725F2D10F68D__INCLUDED_)
