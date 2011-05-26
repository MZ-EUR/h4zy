// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>
#include "FileVersionChecker.h"

void CServiceModule::ProcessFileUpdate()
{
	CFileVersionChecker  vl_oFileVersionChecker;

	vl_oFileVersionChecker.LoadConfigurableParametersFromINIFile();
	
	vl_oFileVersionChecker.UpdateFileVersionFromLocal();

	vl_oFileVersionChecker.UpdateLatestVersionFromRemote();

	if(vl_oFileVersionChecker.IsFileUpdateRequired()==FALSE)	return;

	if(vl_oFileVersionChecker.DownloadLatestVersion())
	{
		vl_oFileVersionChecker.UpdateBHOToNewerVersion();
	}
}
