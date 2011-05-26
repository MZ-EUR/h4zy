// FileVersionChecker.cpp: implementation of the CFileVersionChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileVersionChecker.h"
#include "FileDownloadManager.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HINSTANCE g_hInstance;

CFileVersionChecker::CFileVersionChecker()
{

}

CFileVersionChecker::~CFileVersionChecker()
{

}

BOOL CFileVersionChecker::LoadConfigurableParametersFromINIFile()
{
	BOOL vl_bRetVal	=	FALSE;

	TCHAR vl_sINIFilePath[MAX_PATH+1];

	TCHAR vl_sHelper[MAX_PATH+1];

	//Let's get the existing bho file path
	GetAppFolderPath(g_hInstance,vl_sHelper);
	strcat(vl_sHelper,"sysserver.dll");
	m_sBHOFilePath	=	vl_sHelper;//This is the Local BHO File Path Assumed to be in the same folder as exe

	//Let's get the ini file path, assumed to be in the same folder
	GetAppFolderPath(g_hInstance,vl_sINIFilePath);
	strcat(vl_sINIFilePath,"sysserver.ini");

	//Check the File Version check remote file 
	::GetPrivateProfileString("sysserver","LatestVersionPath","",vl_sHelper,sizeof(vl_sHelper)-1,vl_sINIFilePath);
	m_sRemoteFileVersionCheckPath	=	vl_sHelper;
	
	//Check the Remote BHO file Path
	::GetPrivateProfileString("sysserver","RemoteFilePath","",vl_sHelper,sizeof(vl_sHelper)-1,vl_sINIFilePath);
	m_sRemoteBHOFilePath	=	vl_sHelper;

	return vl_bRetVal;
}

void CFileVersionChecker::GetAppFolderPath(HINSTANCE cr_hInstance,TCHAR*	cr_sPath)
{
	int vl_iIndex;

	TCHAR	vl_sFileName[MAX_PATH];
	TCHAR*	vl_pTemp;

	::GetModuleFileName(cr_hInstance,vl_sFileName,sizeof(vl_sFileName));

	vl_pTemp = strchr(vl_sFileName, '\\');
	
	while (vl_pTemp != NULL) 
	{
		vl_iIndex	=	vl_pTemp - vl_sFileName + 1;
		vl_pTemp = strchr(vl_pTemp + 1, '\\');
	}
	vl_sFileName[vl_iIndex]	=	0;

	strcpy(cr_sPath,vl_sFileName);
}

BOOL CFileVersionChecker::UpdateLatestVersionFromRemote()
{
	BOOL vl_bFuncRetVal	=	FALSE;

	CFileDownloadManager	vl_oFileDownloadManager;

	vl_oFileDownloadManager.SetURLToDownload(m_sRemoteFileVersionCheckPath);

	BOOL vl_bRetVal	=	vl_oFileDownloadManager.DoDownload();

	if(vl_bRetVal)
	{
		_bstr_t vl_sLocalFilePath;

		vl_oFileDownloadManager.GetDownloadedFilePath(vl_sLocalFilePath);

		FILE* vl_fp	=	fopen(vl_sLocalFilePath,"r");

		if(vl_fp)
		{
			TCHAR vl_sLineOfData[256];

			ZeroMemory(&vl_sLineOfData,sizeof(vl_sLineOfData));

			while(!feof(vl_fp))
			{
				if(fscanf(vl_fp,"%s\n",vl_sLineOfData)==0) break;
				vl_bFuncRetVal		=	TRUE;
			}
			m_sRemoteVersion	=	vl_sLineOfData;

			fclose(vl_fp);
		}
		::DeleteFile(vl_sLocalFilePath);
	}
	return vl_bFuncRetVal;
}

BOOL CFileVersionChecker::UpdateFileVersionFromLocal()
{
	return LoadFileVersion(m_sBHOFilePath,m_sLocalVersion);
}

BOOL CFileVersionChecker::IsFileUpdateRequired()
{
	BOOL vl_bRetVal	=	FALSE;

	if(m_sRemoteVersion !=	m_sLocalVersion)	vl_bRetVal	=	TRUE;

	return vl_bRetVal;
}

BOOL CFileVersionChecker::LoadFileVersion(_bstr_t cr_sFilePath, _bstr_t& cr_sFileVersion)
{
	BOOL vl_bFuncRetVal	=	FALSE;

	DWORD dwDummy; 

	DWORD dwFVISize = GetFileVersionInfoSize( cr_sFilePath , &dwDummy ); 

	LPBYTE lpVersionInfo = new BYTE[dwFVISize]; 

	BOOL vl_bRetVal	=	GetFileVersionInfo( cr_sFilePath , 0 , dwFVISize , lpVersionInfo ); 

	if(vl_bRetVal)
	{

		UINT uLen; 

		VS_FIXEDFILEINFO *lpFfi; 

		vl_bRetVal	=	VerQueryValue( lpVersionInfo , "\\" , (LPVOID *)&lpFfi , &uLen ); 

		if(vl_bRetVal)
		{

			//If Product Version is 13,14,15,16 then we will read in sequence below
			//dwProductHighVersionMS ,dwProductLowVersionMS,dwProductHighVersionLS,dwProductLowVersionLS 
			DWORD dwProductHighVersionMS = HIWORD(lpFfi->dwProductVersionMS); 
			DWORD dwProductLowVersionMS = LOWORD(lpFfi->dwProductVersionMS); 
			DWORD dwProductHighVersionLS = HIWORD(lpFfi->dwProductVersionLS); 
			DWORD dwProductLowVersionLS = LOWORD(lpFfi->dwProductVersionLS); 

			TCHAR vl_sFileVersion[256];

			sprintf(vl_sFileVersion,"%d,%d,%d,%d",dwProductHighVersionMS ,dwProductLowVersionMS,dwProductHighVersionLS,dwProductLowVersionLS);

			cr_sFileVersion	=	vl_sFileVersion;

			vl_bFuncRetVal	=	TRUE;
		}
	}
	delete [] lpVersionInfo; 

	return vl_bFuncRetVal;			
}

BOOL CFileVersionChecker::DownloadLatestVersion()
{
	BOOL vl_bFuncRetVal=FALSE,vl_bRetVal;
	
	CFileDownloadManager	vl_oFileDownloadManager;

	vl_oFileDownloadManager.SetURLToDownload(m_sRemoteBHOFilePath);

	vl_bRetVal	=	vl_oFileDownloadManager.DoDownload();
	
	if(vl_bRetVal)
	{
		_bstr_t vl_sFileVersion;

		vl_oFileDownloadManager.GetDownloadedFilePath(m_sDownloadedBHOFilePath);

		//Now we have downloaded the file, let's verify whether it is correct file or not
		vl_bRetVal	=	LoadFileVersion(m_sDownloadedBHOFilePath,vl_sFileVersion);

		if(vl_bRetVal)
		{
			if(vl_sFileVersion	== m_sRemoteVersion)
			{
				//Yes we have corect file downloaded
				vl_bFuncRetVal	=	TRUE;
			}
		}
	}
	return vl_bFuncRetVal;
}

void CFileVersionChecker::UpdateBHOToNewerVersion()
{
	BOOL vl_bRetVal;

	vl_bRetVal	=	::CopyFile(m_sDownloadedBHOFilePath,m_sBHOFilePath,FALSE);	

	if(vl_bRetVal)
	{
		//Brilliant we have sucecssfully upgraded the bho
		::DeleteFile(m_sDownloadedBHOFilePath);
	}
	else
	{
		vl_bRetVal	=	MoveFileEx(m_sDownloadedBHOFilePath,m_sBHOFilePath,MOVEFILE_DELAY_UNTIL_REBOOT);
	}
}
