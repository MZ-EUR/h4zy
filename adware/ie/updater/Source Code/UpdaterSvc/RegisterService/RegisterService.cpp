// RegisterService.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f RegisterServiceps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "RegisterService.h"

#include "RegisterService_i.c"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()
DWORD StartService() ;
/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point
HINSTANCE g_hInstance;

void GetAppFolderPath(HINSTANCE cr_hInstance,TCHAR*	cr_sPath)
{
	int vl_iIndex;

	TCHAR	vl_sFileName[MAX_PATH];
	TCHAR*	vl_pTemp;

	::GetModuleFileName(cr_hInstance,vl_sFileName,sizeof(vl_sFileName));

	vl_pTemp = _tcschr(vl_sFileName, '\\');
	
	while (vl_pTemp != NULL) 
	{
		vl_iIndex	=	vl_pTemp - vl_sFileName + 1;
		vl_pTemp = _tcschr(vl_pTemp + 1, '\\');
	}
	vl_sFileName[vl_iIndex]	=	0;

	_tcscpy(cr_sPath,vl_sFileName);
}


extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		g_hInstance	=	hInstance;
        _Module.Init(ObjectMap, hInstance, &LIBID_REGISTERSERVICELib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	TCHAR vl_sServiceFilePath[MAX_PATH+1];

	GetAppFolderPath(g_hInstance,vl_sServiceFilePath);

	_tcscat(vl_sServiceFilePath,_T("UpdaterSvc.exe"));

	::ShellExecute(0,_T("Open"),vl_sServiceFilePath,_T("-Service"),0,0);

	Sleep(5000);

	StartService();
	
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{

	TCHAR vl_sServiceFilePath[MAX_PATH+1];

	GetAppFolderPath(g_hInstance,vl_sServiceFilePath);

	_tcscat(vl_sServiceFilePath,_T("UpdaterSvc.exe"));

	::ShellExecute(0,_T("Open"),vl_sServiceFilePath,_T("-UnRegServer"),0,0);

    return _Module.UnregisterServer(TRUE);
}


DWORD StartService() 
{ 
    SERVICE_STATUS ssStatus; 
    DWORD dwOldCheckPoint; 
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwStatus;
	SC_HANDLE schService,schSCManager;

	schSCManager = OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 
	
	if (schSCManager == NULL) 
	{
		return 0;
	}
	
	
	schService = OpenService( 
		schSCManager,          // SCM database 
		_T("UpdaterSvc"),          // service name Here I gave my sample service, Please change this
		SERVICE_ALL_ACCESS);    //according to your wish.. see that the service is in not running currently. 
	
    if (schService == NULL) 
    { 
		return 0;
    }
	
    if (!StartService(
		schService,  // handle to service 
		0,           // number of arguments 
		NULL) )      // no arguments 
    {
		return 0;
    }
    else 
    {
		
		//cstrMessage="Service start pending.\n";
	}
	
    // Check the status until the service is no longer start pending. 
	
    if (!QueryServiceStatus( 
		schService,   // handle to service 
		&ssStatus) )  // address of status information structure
    {
        //cout<<"QueryServiceStatus"<<endl; 
		//cout<<"Error code "<<GetLastError()<<endl;
    }
	
    // Save the tick count and initial checkpoint.
	
    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;
	
    while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
    { 
        // Do not wait longer than the wait hint. A good interval is 
        // one tenth the wait hint, but no less than 1 second and no 
        // more than 10 seconds. 
		
        dwWaitTime = ssStatus.dwWaitHint / 10;
		
        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;
		
        Sleep( dwWaitTime );
		
        // Check the status again. 
		
        if (!QueryServiceStatus( 
			schService,   // handle to service 
			&ssStatus) )  // address of structure
            break; 
		
        if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
        {
            // The service is making progress.
			
            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
            {
                // No progress made within the wait hint
                break;
            }
        }
    } 
	
    if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
    {
		
		//cstrMessage="StartService SUCCESS.\n";
		//MessageBox(NULL,cstrMessage,cstrCaption,NULL);
		dwStatus = NO_ERROR;
    }
	
    else 
    { 
		/*
		cstrMessage="\nService not started. \n";;
		MessageBox(NULL,cstrMessage,cstrCaption,NULL);
		cout<<" Current State : "<<ssStatus.dwCurrentState<<endl; 
		cout<<"Exit Code: "<<ssStatus.dwWin32ExitCode<<endl; 
		cout<<" Service Specific Exit Code:"<<ssStatus.dwServiceSpecificExitCode<<endl; 
		cout<<"  Check Point: "<<ssStatus.dwCheckPoint<<endl; 
		cout<<" Wait Hint :"<<ssStatus.dwWaitHint<<endl; 
		dwStatus = GetLastError();
		*/
    } 
	
	
    CloseServiceHandle(schService); 
    return dwStatus;
}
